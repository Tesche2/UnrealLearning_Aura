// CopyrightTesche


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

int32 AAuraCharacterBase::GetPlayerLevel() const
{
	return ICombatInterface::GetPlayerLevel();
}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	const float Level = GetPlayerLevel();
	ApplyEffectToSelf(DefaultPrimaryAttributes, Level);
	ApplyEffectToSelf(DefaultSecondaryAttributes, Level);
	ApplyEffectToSelf(DefaultVitalAttributes, Level);
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	
	// Only run on the server
	if(!HasAuthority()) return;

	AuraASC->AddCharacterAbilities(StartupAbility);
}

void AAuraCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> AttributeClass, const float Level) const
{
	check(IsValid(AbilitySystemComponent));
	check(AttributeClass);

	// Make GE context (who fired skill, who received skill)
	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	
	// Make GE Spec Handle (skill level, caster's attributes)
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		AttributeClass, Level, ContextHandle);

	// Apply GE to target
	AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
		*SpecHandle.Data.Get(), AbilitySystemComponent);
}
