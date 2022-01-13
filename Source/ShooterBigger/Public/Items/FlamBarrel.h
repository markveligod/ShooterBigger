

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlamBarrel.generated.h"

class USoundCue;
class ATrashItem;
UCLASS()
class SHOOTERBIGGER_API AFlamBarrel : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFlamBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// root component
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* Root;

	// The base mesh barrel
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshBarrel;

	// A set of pieces explode when spawned
	UPROPERTY(EditDefaultsOnly, Category = "Explosion|Data")
	TArray<UStaticMesh*> ArrayPartExp;

	// The actor of trash items for spawning parts of the barrel
	UPROPERTY(EditDefaultsOnly, Category = "Explosion|Data")
	TSubclassOf<ATrashItem> TrashItem;

	// Explosion radius of the current barrel
	UPROPERTY(EditDefaultsOnly, Category = "Explosion|Data")
	FFloatInterval RadiusExplosion = {270.0f, 300.0f};

	// Barrel explosion Strength to impulse
	UPROPERTY(EditDefaultsOnly, Category = "Explosion|Data")
	FFloatInterval StrengthExplosion = {18000.0f, 25920.0f};

	// Delay before explosion
	UPROPERTY(EditDefaultsOnly, Category = "Explosion|Data", meta = (Units = "s"))
	float DelayExplosion = 0.1f;

	// The type of danger of the barrel
	UPROPERTY(EditDefaultsOnly, Category = "Explosion|Data")
	TSubclassOf<UDamageType> DamageTypeBarrel;

	// The boom boom effect
	UPROPERTY(EditDefaultsOnly, Category = "Explosion|VFX")
	UParticleSystem* EffectExplosion;

	// The imprint of the explosion in the form of a decal on the stage
	UPROPERTY(EditDefaultsOnly, Category = "Explosion|Decal")
	UMaterialInterface* DecalExplosion;

	// Decal explosion size
	UPROPERTY(EditDefaultsOnly, Category = "Explosion|Decal")
	FFloatInterval DecalExplosionSize = {275.0f, 325.0f};

	// Offset of the angle of rotation for the decal
	UPROPERTY(EditDefaultsOnly, Category = "Explosion|Decal")
	FRotator OffsetRotation = FRotator::ZeroRotator;

	// Explosion sound
	UPROPERTY(EditDefaultsOnly, Category = "Explosion|Sound")
	USoundCue* SoundBoom;

	// Enable Debug?
	UPROPERTY(EditDefaultsOnly, Category = "Explosion|Debug")
	bool bIsEnableDebug = true;

	// Color debug
	UPROPERTY(EditDefaultsOnly, Category = "Explosion|Debug")
	FColor Color = FColor::Red;

	// Value segments
	UPROPERTY(EditDefaultsOnly, Category = "Explosion|Debug")
	int32 CountSegments = 8;

	// How much to display debug information
	UPROPERTY(EditDefaultsOnly, Category = "Explosion|Debug", meta = (Units = "s"))
	float LifeTime = 5.0f;

	// the thickness of the segments on the debug information
	UPROPERTY(EditDefaultsOnly, Category = "Explosion|Debug")
	float Thickness = 2.0f;

	// Registering the damage received for the current barrel
	UFUNCTION()
	void RegisterTakeDamage(
		AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	// State begin destroy
	bool bIsStartDestroy = false;

	// Radius explosion
	float RadiusExp;

	// Starting the explosion process
	void StartExplosion();
};
