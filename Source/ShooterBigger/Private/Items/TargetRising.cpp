
#include "Items/TargetRising.h"
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
		TEXT("Name damage actor: %s | Amount damage: %f | Hit Location: %s | Name damage type: %s | Instigated controller: %s | Damage "
			 "Causer : %s"),
		*NameDamagedActor, Damage, *HitLocation.ToString(), *NameDamageType, *NameInstigatedBy, *NameDamageCauser);
}

void ATargetRising::ResetReloadTarget()
{
	this->MeshTarget->PlayAnimation(this->AnimTargetUp, false);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), this->SoundTargetUp, GetActorLocation(), FRotator::ZeroRotator);
	this->StateTarget = EStateTarget::Ready;
}
