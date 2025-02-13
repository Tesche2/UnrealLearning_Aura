// CopyrightTesche


#include "AbilitySystem/MMC/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	CAPTURE_ATTRIBUTE(Intelligence);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float Intelligence = 0.f;
	int32 PlayerLevel = 0.f;
	GetAttributeAndLevel(Intelligence, PlayerLevel, Spec);

	return 50 + 2.f * Intelligence + 15.f * PlayerLevel;
}
