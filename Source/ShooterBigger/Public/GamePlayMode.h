

#pragma once

#include "CoreMinimal.h"
#include "GamePlayDataTypes.h"
#include "Characters/Player/PlayerCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "HUD/GameHUD.h"
#include "GamePlayMode.generated.h"

class AGameHUD;
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

	// Signature on state game play
	FOnStateGamePlayChangedSignature OnStateGamePlayChanged;

	// Function for notification of EStateGamePlay changes
	UFUNCTION(BlueprintCallable, Category = "AGamePlayMode")
	void ChangeStateGamePlay(EStateGamePlay NewState);
	
	// Getting singleton pointer on current class
	UFUNCTION(BlueprintCallable, Category = "AGamePlayMode")
	static AGamePlayMode* Get(UWorld* World) { return Cast<AGamePlayMode>(World->GetAuthGameMode()); }

	// Getting current pointer on APlayerCharacter class
	UFUNCTION(BlueprintCallable, Category = "AGamePlayMode")
	FORCEINLINE APlayerCharacter* GetPlayerCharacter() const { return this->PlayerCharacter; }

#pragma region ExecuteCheats

	// Setup new state for displaying info about Control
	UFUNCTION(Exec, Category = "Cheats|DebugManagment")
	void DebugHUDControl(const bool State) const { this->GameHUD->SetupDebugInfoControl(State); }

	// Setup new state for displaying info about State
	UFUNCTION(Exec, Category = "Cheats|DebugManagment")
	void DebugHUDState(const bool State) const { this->GameHUD->SetupDebugInfoState(State); }

	// Setup new state for displaying info about Weapon
	UFUNCTION(Exec, Category = "Cheats|DebugManagment")
	void DebugHUDWeapon(const bool State) const { this->GameHUD->SetupDebugInfoWeapon(State); }

	// Setup new state for debug trace shot
	UFUNCTION(Exec, Category = "Cheats|DebugManagment")
	void DebugTraceShot(const bool State) const { this->PlayerCharacter->SetupDebugTraceShot(State); }

#pragma endregion

protected:
	virtual void BeginPlay() override;
	virtual void StartPlay() override;

private:
	// Current pointer on APlayerCharacter class
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	UPROPERTY()
	AGameHUD* GameHUD;

	// Current state game play
	EStateGamePlay StateGamePlay = EStateGamePlay::None;
};
