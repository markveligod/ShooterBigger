
#include "Items/TargetRising.h"

#include "Items/TrashItem.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogTargetRising, All, All);

// Sets default values
ATargetRising::ATargetRising()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	this->Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(this->Root);

	this->MeshTarget = CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal Mesh target");
	this->MeshTarget->SetupAttachment(this->Root);
}

// Called when the game starts or when spawned
void ATargetRising::BeginPlay()
{
	Super::BeginPlay();

	OnTakePointDamage.AddDynamic(this, &ATargetRising::RegisterPointTakeDamage);
	OnTakeRadialDamage.AddDynamic(this, &ATargetRising::RegisterRadialTakeDamage);
	this->StateTarget = EStateTarget::Ready;
}

void ATargetRising::RegisterPointTakeDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
	UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	if (this->StateTarget != EStateTarget::Ready) return;

	this->StateTarget = EStateTarget::Reload;
	this->MeshTarget->PlayAnimation(this->AnimTargetDown, false);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), this->SoundTargetDown, GetActorLocation(), FRotator::ZeroRotator);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle, this, &ATargetRising::ResetReloadTarget, FMath::RandRange(this->RangeReload.Min, this->RangeReload.Max), false);

	const FString NameDamagedActor = (DamagedActor) ? DamagedActor->GetName() : "NULL";
	const FString NameDamageType = (DamageType) ? DamageType->GetName() : "NULL";
	const FString NameInstigatedBy = (InstigatedBy) ? InstigatedBy->GetName() : "NULL";
	const FString NameDamageCauser = (DamageCauser) ? DamageCauser->GetName() : "NULL";

	UE_LOG(LogTargetRising, Display,
		TEXT("RegisterPointTakeDamage | Name damage actor: %s | Amount damage: %f | Hit Location: %s | Name damage type: %s |"
			 "Instigated controller: %s | Damage Causer : %s"),
		*NameDamagedActor, Damage, *HitLocation.ToString(), *NameDamageType, *NameInstigatedBy, *NameDamageCauser);
}

void ATargetRising::RegisterRadialTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin,
	FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	this->StateTarget = EStateTarget::Destroy;

	for (const auto Mesh : this->ArrayPartTarget)
	{
		const ATrashItem* TempTrashItem =
			GetWorld()->SpawnActor<ATrashItem>(GetActorLocation(), GetActorRotation(), FActorSpawnParameters());
		TempTrashItem->GetMesh()->SetStaticMesh(Mesh);
	}

	const FString NameDamagedActor = (DamagedActor) ? DamagedActor->GetName() : "NULL";
	const FString NameDamageType = (DamageType) ? DamageType->GetName() : "NULL";
	const FString NameInstigatedBy = (InstigatedBy) ? InstigatedBy->GetName() : "NULL";
	const FString NameDamageCauser = (DamageCauser) ? DamageCauser->GetName() : "NULL";

	UE_LOG(LogTargetRising, Display,
		TEXT("RegisterRadialTakeDamage | Name damage actor: %s | Amount damage: %f | Name damage type: %s | "
			 "Instigated controller: %s | Damage Causer : %s"),
		*NameDamagedActor, Damage, *NameDamageType, *NameInstigatedBy, *NameDamageCauser);
	Destroy();
}

void ATargetRising::ResetReloadTarget()
{
	if (this->StateTarget == EStateTarget::Destroy) return;

	this->MeshTarget->PlayAnimation(this->AnimTargetUp, false);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), this->SoundTargetUp, GetActorLocation(), FRotator::ZeroRotator);
	this->StateTarget = EStateTarget::Ready;
}
