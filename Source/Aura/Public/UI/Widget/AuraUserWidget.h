// CopyrightTesche

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/**
	 * Sets Widget Controller for UAuraUserWidget, and triggers Widget Controller Set event.
	 * 
	 * @param InWidgetController 
	 */
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	/**
	 * Triggered when SetWidgetController() is run.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
