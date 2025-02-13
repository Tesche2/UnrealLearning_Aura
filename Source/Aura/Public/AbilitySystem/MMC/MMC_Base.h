// CopyrightTesche

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Base.generated.h"

#define CAPTURE_ATTRIBUTE(AttributeName)\
	CapturedAttributeDef.AttributeToCapture = UAuraAttributeSet::Get##AttributeName##Attribute();\
	CapturedAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;\
	CapturedAttributeDef.bSnapshot = false;\
	RelevantAttributesToCapture.Add(CapturedAttributeDef);

/**
 * 
 */
UCLASS()
class AURA_API UMMC_Base : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
	static FAggregatorEvaluateParameters SetEvaluationParameters(const FGameplayEffectSpec& Spec);
	void GetAttributeAndLevel(
		float& Attribute,
		int32& PlayerLevel,
		const FGameplayEffectSpec& Spec) const;
	FGameplayEffectAttributeCaptureDefinition CapturedAttributeDef;
};
