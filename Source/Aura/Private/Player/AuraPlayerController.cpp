// CopyrightTesche


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/TargetInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
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
		if(LastActor)
		{
			LastActor->UnHighlightActor();
		}

		if(ThisActor)
		{
			ThisActor->HighlightActor();
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(!GetASC()) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if(!GetASC()) return;
	GetASC()->AbilityInputTagHeld(InputTag);
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
