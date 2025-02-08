// CopyrightTesche


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// Get Ability System Component from Target Actor.
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(!TargetASC) return;

	// Create Context Handle and Spec Handle.
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle  = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(
		GameplayEffectClass, 1.f, EffectContextHandle);

	// Apply Effect, and get Effect Handle.
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(
		*EffectSpecHandle.Data.Get());

	// Store Handles for Duration and Infinite effect types.
	if(EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

void AAuraEffectActor::RemoveEffectFromTarget(AActor* TargetActor)
{
	// Get Actor's AbilitySystemComponent
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(!IsValid(TargetASC)) return;

	// Create Array to mark Effect Handles we wish to remove from our ActiveEffectHandles Map
	TArray<FActiveGameplayEffectHandle> HandlesToRemove;

	// For all Effects in our map, if the target contains such an Effect, remove it
	for(TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
	{
		if(TargetASC == HandlePair.Value)
		{
			TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
			HandlesToRemove.Add(HandlePair.Key);
		}
	}
	
	// Remove marked EffectHandles from map
	for(FActiveGameplayEffectHandle& Handle : HandlesToRemove)
	{
		ActiveEffectHandles.FindAndRemoveChecked(Handle);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	// Apply all effects defined with ApplyOnOverlap
	if(InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if(DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if(InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	// Apply all effects defined with ApplyOnEndOverlap
	if(InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if(DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if(InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	// Remove all effects defined with RemoveOnEndOverlap
	if(InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		RemoveEffectFromTarget(TargetActor);
	}
}


