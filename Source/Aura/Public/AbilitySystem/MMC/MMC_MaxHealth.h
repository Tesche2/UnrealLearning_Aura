// CopyrightTesche

#pragma once

#include "CoreMinimal.h"
#include "MMC_Base.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UMMC_Base
{
	GENERATED_BODY()

public:
	UMMC_MaxHealth();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
