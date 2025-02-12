// CopyrightTesche


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"

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
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
}

void AAuraCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> AttributeClass, const float Level) const
{
	check(IsValid(AbilitySystemComponent));
	check(AttributeClass);

	// Make GE context (who fired skill, who received skill)
	const FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();

	// Make GE Spec Handle (skill level, caster's attributes)
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		AttributeClass, Level, ContextHandle);

	// Apply GE to target
	AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
		*SpecHandle.Data.Get(), AbilitySystemComponent);
}
