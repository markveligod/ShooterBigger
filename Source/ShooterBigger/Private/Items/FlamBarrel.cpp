
#include "Items/FlamBarrel.h"

#include "DrawDebugHelpers.h"
#include "Items/TrashItem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogFlamBarrel, All, All);

// Sets default values
AFlamBarrel::AFlamBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	this->Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(this->Root);

	this->MeshBarrel = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Barrel");
	this->MeshBarrel->SetupAttachment(this->Root);
}

// Called when the game starts or when spawned
void AFlamBarrel::BeginPlay()
{
	Super::BeginPlay();
	OnTakeAnyDamage.AddDynamic(this, &AFlamBarrel::RegisterTakeDamage);
	this->RadiusExp = FMath::RandRange(this->RadiusExplosion.Min, this->RadiusExplosion.Max);
}

void AFlamBarrel::RegisterTakeDamage(
	AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bIsStartDestroy) return;
	this->bIsStartDestroy = true;

	const FString NameDamagedActor = (DamagedActor) ? DamagedActor->GetName() : "NULL";
	const FString NameDamageType = (DamageType) ? DamageType->GetName() : "NULL";
	const FString NameInstigatedBy = (InstigatedBy) ? InstigatedBy->GetName() : "NULL";
	const FString NameDamageCauser = (DamageCauser) ? DamageCauser->GetName() : "NULL";

	UE_LOG(LogFlamBarrel, Display,
		TEXT("Name damage actor: %s | Amount damage: %f | Name damage type: %s | Instigated controller: %s | Damage Causer : %s"),
		*NameDamagedActor, Damage, *NameDamageType, *NameInstigatedBy, *NameDamageCauser);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AFlamBarrel::StartExplosion, this->DelayExplosion, false);
}

void AFlamBarrel::StartExplosion()
{
	TArray<AActor*> IgnoreActors;

	// Spawn trash items from barrel
	TArray<ATrashItem*> ArrayTrashItems;
	for (const auto Mesh : ArrayPartExp)
	{
		ATrashItem* TempTrashItem = GetWorld()->SpawnActor<ATrashItem>(
			this->MeshBarrel->GetComponentLocation(), this->MeshBarrel->GetComponentRotation(), FActorSpawnParameters());
		TempTrashItem->GetMesh()->SetStaticMesh(Mesh);
		ArrayTrashItems.Add(TempTrashItem);
		IgnoreActors.Add(TempTrashItem);
	}

	// Take radial damage
	UGameplayStatics::ApplyRadialDamage(
		GetWorld(), 1.0f, this->MeshBarrel->GetComponentLocation(), this->RadiusExp, this->DamageTypeBarrel, IgnoreActors);

	// Physical explosion boom-boom
	for (const auto Item : ArrayTrashItems)
	{
		Item->GetMesh()->AddRadialImpulse(this->MeshBarrel->GetComponentLocation(), this->RadiusExp,
			FMath::RandRange(this->StrengthExplosion.Min, this->StrengthExplosion.Max), ERadialImpulseFalloff::RIF_Linear);
	}

	// spawn emitter effect
	FTransform Transform;
	Transform.SetLocation(this->MeshBarrel->GetComponentLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), this->EffectExplosion, Transform);

	// Spawn Decal
	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), this->DecalExplosion,
		FVector(FMath::RandRange(this->DecalExplosionSize.Min, this->DecalExplosionSize.Max)), this->MeshBarrel->GetComponentLocation(),
		this->OffsetRotation);

	// Spawn Sound
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), this->SoundBoom, this->MeshBarrel->GetComponentLocation());

	if (this->bIsEnableDebug)
	{
		DrawDebugSphere(GetWorld(), this->MeshBarrel->GetComponentLocation(), this->RadiusExp, this->CountSegments, this->Color, false,
			this->LifeTime, 0, this->Thickness);
	}

	Destroy();
}
