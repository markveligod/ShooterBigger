
#include "Items/TrashItem.h"

// Sets default values
ATrashItem::ATrashItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	this->Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(this->Root);

	this->Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
	this->Mesh->SetupAttachment(this->Root);
	this->Mesh->SetSimulatePhysics(true);
}

void ATrashItem::BeginPlay()
{
	Super::BeginPlay();
}
