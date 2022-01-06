

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GamePlayMode.generated.h"

class APlayerCharacter;
/**
 *
 */
UCLASS()
class SHOOTERBIGGER_API AGamePlayMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGamePlayMode();

	// Getting singleton pointer on current class
	UFUNCTION(BlueprintCallable, Category = "AGamePlayMode")
	static AGamePlayMode* Get(UWorld* World) { return Cast<AGamePlayMode>(World->GetAuthGameMode()); }

	// Getting current pointer on APlayerCharacter class
	UFUNCTION(BlueprintCallable, Category = "AGamePlayMode")
	FORCEINLINE APlayerCharacter* GetPlayerCharacter() const { return this->PlayerCharacter; }

protected:
	virtual void BeginPlay() override;

private:
	// Current pointer on APlayerCharacter class
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;
};
