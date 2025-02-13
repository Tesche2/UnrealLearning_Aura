// CopyrightTesche

#pragma once

#include "CoreMinimal.h"
#include "MMC_Base.h"
#include "MMC_MaxMana.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxMana : public UMMC_Base
{
	GENERATED_BODY()

public:
	UMMC_MaxMana();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
