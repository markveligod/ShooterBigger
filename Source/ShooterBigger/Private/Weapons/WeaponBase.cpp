
#include "Weapons/WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create root component for mesh weapon
	this->MeshWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Mesh weapon"));
	SetRootComponent(this->MeshWeapon);

	// Create Mesh Ironsights
	this->MeshIronsights = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh Ironsights"));
	this->MeshIronsights->SetupAttachment(this->MeshWeapon, FName("SOCKET_Default"));

	// Create Mesh Magazine
	this->MeshMagazine = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh Magazine"));
	this->MeshMagazine->SetupAttachment(this->MeshWeapon, FName("SOCKET_Magazine"));
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}
