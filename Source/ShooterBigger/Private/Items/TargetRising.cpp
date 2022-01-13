
#include "Items/TargetRising.h"

// Sets default values
ATargetRising::ATargetRising()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATargetRising::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATargetRising::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
