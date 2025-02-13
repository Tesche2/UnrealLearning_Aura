// CopyrightTesche


#include "AbilitySystem/MMC/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/MMC/MMC_Base.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	CAPTURE_ATTRIBUTE(Vigor);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float Vigor = 0.f;
	int32 PlayerLevel = 1;
	GetAttributeAndLevel(Vigor, PlayerLevel, Spec);

	return 80 + 2.5f * Vigor + 10.f * PlayerLevel;
}
