// CopyrightTesche


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
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
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	// Link MoveAction to Move function
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
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
