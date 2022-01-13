
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrashItem.generated.h"

UCLASS()
class SHOOTERBIGGER_API ATrashItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATrashItem();

	// Getting mesh
	UFUNCTION(BlueprintCallable, Category = "ATrashItem|Data")
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return (this->Mesh); }

protected:
	virtual void BeginPlay() override;

private:
	// root component
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* Root;

	// Base mesh component
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Mesh;
};
