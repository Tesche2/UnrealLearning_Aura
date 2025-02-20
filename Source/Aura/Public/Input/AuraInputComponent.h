// CopyrightTesche

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	/**
	 * Binds an Input to an Ability, running specified Pressed, Released and Held functions
	 * 
	 * @tparam UserClass 
	 * @tparam PressedFuncType 
	 * @tparam ReleasedFuncType 
	 * @tparam HeldFuncType 
	 * @param InputConfig Config file for Inputs
	 * @param Object Object in which the functions run
	 * @param PressedFunc Function to run on Input press
	 * @param ReleasedFunc Function to run on Input release
	 * @param HeldFunc Function to run every frame while Input is held
	 */
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityAction(
		const UAuraInputConfig* InputConfig,
		UserClass* Object,
		PressedFuncType PressedFunc,
		ReleasedFuncType ReleasedFunc,
		HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityAction(const UAuraInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	// Loop through actions defined in the InputConfig file.
	for (const FAuraInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			// Bind action to each of Pressed, Released and Held functions.
			if(PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if(ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
			
			if(HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}


