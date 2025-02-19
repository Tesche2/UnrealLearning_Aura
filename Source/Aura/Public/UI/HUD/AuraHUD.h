// CopyrightTesche

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;
class UAuraUserWidget;

struct FWidgetControllerParams;
/**
 * Retrieves data from UOverlayWidgetController
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	/**
	 * Gets the Overlay Widget Controller of AAuraHUD, creates one if none exists.
	 * 
	 * @param WCParams Parameters to set if no UOverlayWidgetController exists.
	 * @return 
	 */
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	
	/**
	 * Gets the Attribute Menu Widget Controller of AAuraHUD, creates one if none exists.
	 * 
	 * @param WCParams Parameters to set if no UOverlayWidgetController exists.
	 * @return 
	 */
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);

	/**
	 * Initiates the HUD Overlay, given valid parameters
	 * 
	 * @param PC Player Controller
	 * @param PS Player State
	 * @param ASC Ability System Component
	 * @param AS Attribute Set
	 */
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	//UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController")
	//static UAttributeMenuWidgetController* ConstructAttributeMenu(const UObject* WorldContextObject);

private:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY()
	TObjectPtr<UAuraUserWidget> AttributeMenuWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> AttributeMenuWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
};
