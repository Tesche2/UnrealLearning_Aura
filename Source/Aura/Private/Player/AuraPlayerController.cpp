// CopyrightTesche


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/TargetInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	AutoRun();
	
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit); // Ray cast

	if(!CursorHit.bBlockingHit) return; // Nothing hit
	
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/**
	 * Line trace from cursor has several scenarios
	 * -----------------------------------------------------
	 * A. LastActor = null && ThisActor = null : DO NOTHING
	 * B. LastActor = null && ThisActor = valid : HIGHLIGHT ThisActor
	 * C. LastActor = valid && ThisActor = null : UNHIGHLIGHT LastActor
	 * D. LastActor = valid && ThisActor = valid && ThisActor != LastActor : UNHIGLIGHT LastActor, HIGHLIGHT ThisActor
	 * E. LastActor = valid && ThisActor = valid && ThisActor == LastActor : DO NOTHING
	 * -----------------------------------------------------
	 */
	if(ThisActor != LastActor)
	{
		if(LastActor) LastActor->UnHighlightActor();
		if(ThisActor) ThisActor->HighlightActor();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
		FollowTime = 0.f;
	}
	
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	// If Input is not LMB or enemy is targeted
	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB) || bTargeting)
	{
		if(!GetASC()) return;
		GetASC()->AbilityInputTagReleased(InputTag);
	}
	else // Else, check if LMB was held or tapped
	{
		StopManualRunning();
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// If Input is not LMB or enemy is targeted, attack
	if(!(InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB)) || bTargeting)
	{
		if(!GetASC()) return;
		GetASC()->AbilityInputTagHeld(InputTag);
	}
	else // Else, move character
	{
		FollowCursor();
	}
	
	
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if(!AuraAbilitySystemComponent)
	{
		// Create AuraASC if it doesn't exist
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
				GetPawn<APawn>()));
	}

	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext); // Check if context was set, crashes if not

	// Get Subsystem. GetLocalPlayer() is only valid for clients, thus we check without crashing
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(AuraContext, 0); // Add context to the Subsystem
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default; // Set Cursor visibility and visuals

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false); // Set Cursor behavior
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Cast default InputComponent to EnhancedInputComponent
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	// Bind MoveAction to Move function
	AuraInputComponent->BindAction(MoveAction,
		ETriggerEvent::Triggered,
		this,
		&AAuraPlayerController::Move);

	// Bind Ability Actions
	AuraInputComponent->BindAbilityAction(
		InputConfig,
		this,
		&ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased,
		&ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>(); // Get Vector2D from InputActionValue
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f); // Get current rotation based on the camera's position

	// Get Forward and Right directions from rotation
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// Move Character in the appropriate directions
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::AutoRun()
{
	if(APawn* ControlledPawn = GetPawn(); bAutoRunning)
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
			ControlledPawn->GetActorLocation(),
			ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(
			LocationOnSpline,
			ESplineCoordinateSpace::World);

		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if(DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::FollowCursor()
{
	FollowTime += GetWorld()->GetDeltaSeconds();

	// Get clicked destination
	if(CursorHit.bBlockingHit)
	{
		CachedDestination = CursorHit.ImpactPoint;
	}

	// Move Pawn to destination
	if(APawn* ControlledPawn = GetPawn())
	{
		const FVector WorldDirection =
			(CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection);
	}
}

void AAuraPlayerController::StopManualRunning()
{
	// If player tapped LMB, auto running should trigger
	if(const APawn* ControlledPawn = GetPawn(); FollowTime <= ShortPressThreshold && ControlledPawn)
	{
		// Pathfind over NavMesh
		if(UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
			this,
			ControlledPawn->GetActorLocation(),
			CachedDestination))
		{
			// Create spline from pathfinding points
			Spline->ClearSplinePoints();
			for(const FVector& PointLoc : NavPath->PathPoints)
			{
				Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Red, false, 5.f);
			}
			// Update destination, so it doesn't try to pathfind into objects
			CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
			bAutoRunning = true;
		}
	}
	FollowTime = 0.f;
	bTargeting = false;
}