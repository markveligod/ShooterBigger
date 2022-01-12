
#include "Weapons/WeaponBase.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponBase, All, All);

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
	UGameplayStatics::SpawnEmitterAttached(
		this->EmitterEffect, this->MeshWeapon, this->SocketMuzzle, FVector(ForceInit), this->DeltaMuzzleRot);

	// Make hit
	FVector StartLine = this->MeshWeapon->GetSocketLocation(this->SocketMuzzle);
	const APlayerCameraManager* Camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector EndLine = Camera->GetCameraLocation() + Camera->GetCameraRotation().Vector() * this->DistanceShot;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(this->HitResult, StartLine, EndLine, ECC_Visibility, CollisionQueryParams);

	if (this->HitResult.IsValidBlockingHit())
	{
		AActor* HitActor = this->HitResult.GetActor();
		if (HitActor)
		{
			UE_LOG(LogWeaponBase, Display, TEXT("Name weapon: %s | Name owner: %s | Name Damage Actor: %s | Amount Damage: %f"), *GetName(),
				*GetOwner()->GetName(), *HitActor->GetName(), this->AmountDamage);
			HitActor->TakeDamage(this->AmountDamage, this->PointDamageInfo, GetOwner()->GetInstigatorController(), this);
		}

		// Debug trace
		if (this->bEnableDebugShot)
		{
			DrawDebugLine(GetWorld(), StartLine, this->HitResult.ImpactPoint, this->ColorTrace, false, this->DrawTime, 0, this->Thickness);
			DrawDebugSphere(GetWorld(), this->HitResult.ImpactPoint, this->RadiusHit, this->SegmentsHit, this->ColorHit, false,
				this->DrawTime, 0, this->Thickness);
		}
	}
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	this->AmmunitionCurrent = this->AmmunitionMax;
}
