// CopyrightTesche


#include "UI/WidgetController/AuraWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
	// Cast AttributeSet* to AuraAttributeSet*
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
	// Cast AttributeSet* to AuraAttributeSet*
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
#define BIND_CALLBACK(AttributeName) \
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( \
	AuraAttributeSet->Get##AttributeName##Attribute()).AddLambda([this](const FOnAttributeChangeData& Data) \
	{ \
	On##AttributeName##Changed.Broadcast(Data.NewValue); \
	});

	BIND_CALLBACK(Health);
	BIND_CALLBACK(MaxHealth);
	BIND_CALLBACK(Mana);
	BIND_CALLBACK(MaxMana);
#undef BIND_CALLBACK
}
