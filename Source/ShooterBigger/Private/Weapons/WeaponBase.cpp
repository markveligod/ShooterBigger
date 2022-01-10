
#include "Weapons/WeaponBase.h"

#include "Kismet/GameplayStatics.h"

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

void AWeaponBase::MakeShot()
{
	// Play animation weapon
	this->MeshWeapon->PlayAnimation(this->MontageFire, false);

	// Possible reduction of Ammunition
	this->AmmunitionCurrent = FMath::Clamp(this->AmmunitionCurrent - 1, 0, this->AmmunitionMax);

	// spawn emitter effect
	UGameplayStatics::SpawnEmitterAttached(this->EmitterEffect, this->MeshWeapon, this->SocketMuzzle);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	this->AmmunitionCurrent = this->AmmunitionMax;
}
