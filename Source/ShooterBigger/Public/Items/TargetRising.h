

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetRising.generated.h"

class USoundCue;
UENUM()
enum class EStateTarget
{
	None = 0,
	Ready,
	Reload,
	Destroy
};

UCLASS()
class SHOOTERBIGGER_API ATargetRising : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATargetRising();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// root component
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* Root;

	// Base skeletal mesh for target
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USkeletalMeshComponent* MeshTarget;

	// Range time in sec for Reload state
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Hit")
	FFloatInterval RangeReload = {3.0f, 6.0f};

	// Animation from hitting the target
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Hit|Anim")
	UAnimationAsset* AnimTargetDown;

	// Animation from returning to the starting position
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Hit|Anim")
	UAnimationAsset* AnimTargetUp;

	// Sound from hitting the target
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Hit|Sound")
	USoundCue* SoundTargetDown;

	// Sound from returning to the starting position
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Hit|Sound")
	USoundCue* SoundTargetUp;

	// Registering the damage received for the current Target
	UFUNCTION()
	void RegisterPointTakeDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
		UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);

	// Reset reload to ready
	void ResetReloadTarget();

	// Current state target rising
	EStateTarget StateTarget = EStateTarget::None;
};
