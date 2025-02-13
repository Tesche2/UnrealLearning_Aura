// CopyrightTesche


#include "AbilitySystem/MMC/MMC_Base.h"

#include "Interaction/CombatInterface.h"

float UMMC_Base::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	return 0;
}

FAggregatorEvaluateParameters UMMC_Base::SetEvaluationParameters(const FGameplayEffectSpec& Spec)
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Set Evaluation Parameters
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	return EvaluationParameters;
}

void UMMC_Base::GetAttributeAndLevel(
	float& Attribute,
	int32& PlayerLevel,
	const FGameplayEffectSpec& Spec) const
{
	// Set evaluation parameters
	const FAggregatorEvaluateParameters EvaluationParameters = SetEvaluationParameters(Spec);

	// Get Attribute
	GetCapturedAttributeMagnitude(CapturedAttributeDef, Spec, EvaluationParameters, Attribute);
	Attribute = FMath::Max<float>(Attribute, 0.f);

	// Get Level
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	PlayerLevel = CombatInterface->GetPlayerLevel();
}
