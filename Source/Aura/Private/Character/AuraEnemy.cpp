// CopyrightTesche


#include "Character/AuraEnemy.h"

void AAuraEnemy::HighlightActor()
{
	bIsHighlighted = true;
}

void AAuraEnemy::UnHighlightActor()
{
	bIsHighlighted = false;
}

AAuraEnemy::AAuraEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAuraEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(bIsHighlighted)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), 50.f, 12, FColor::Red);
	}
}


