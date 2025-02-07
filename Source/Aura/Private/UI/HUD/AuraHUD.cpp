// CopyrightTesche


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	// Check if Controller exists
	if(!OverlayWidgetController)
	{
		// Create Controller and set its parameters
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);

		// Bind OnChange Events
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// Check if Overlay Classes are initialized, unable to continue if they're not
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class Uninitalized, Please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class Uninitalized, Please fill out BP_AuraHUD"));

	// Create a Widget and cast it to UAuraUserWidget
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	/** Create UOverlayWidgetController with valid:
	 * APlayerController
	 * APlayerState
	 * UAbilitySystemComponent
	 * UAttributeSet
	 */
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	/** Set the OverlayWidget's Controller to Widget Controller, triggering UAuraUserWidget::WidgetControllerSet()
	 * in blueprints
	 */
	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();

	// Add Widget to Viewport (Why not OverlayWidget??)
	Widget->AddToViewport();
}
