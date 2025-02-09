// CopyrightTesche

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}
	
	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent *TargetASC = nullptr, *SourceASC = nullptr;
	
	UPROPERTY()
	AActor *TargetAvatarActor = nullptr, *SourceAvatarActor = nullptr;
	
	UPROPERTY()
	AController *TargetController = nullptr, *SourceController = nullptr;
	
	UPROPERTY()
	ACharacter *TargetCharacter = nullptr, *SourceCharacter = nullptr;
};

/**
 * Defines Attributes, unknowingly sends data to UOverlayWidgetController
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Runs when an Attribute is changed, right before applying the GameplayEffect
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// Runs right after applying a GameplayEffect
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;


#pragma region Health
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health; // Create attribute
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health); // Generate get/set/init

	UFUNCTION()
	// Automatically replicate when attribute is updated
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth) 
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
	}
#pragma endregion Health

#pragma region MaxHealth
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
	}
#pragma endregion MaxHealth

#pragma region Mana
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldMana)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
	}
#pragma endregion Mana

#pragma region MaxMana
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
	}
#pragma endregion MaxMana

private:
	static void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props);
};
