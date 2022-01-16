

#pragma once

#include "CoreMinimal.h"
#include "GamePlayDataTypes.h"
#include "HUD/DebugHUD.h"
#include "GameHUD.generated.h"

class UUserWidgetBase;
class APlayerCharacter;
class AGamePlayMode;
/**
 *
 */
UCLASS()
class SHOOTERBIGGER_API AGameHUD : public ADebugHUD
{
	GENERATED_BODY()

public:
	// Setup new state for displaying info about Control
	UFUNCTION(BlueprintCallable, Category = "AGameHUD|Managment")
	void SetupDebugInfoControl(const bool NewState) { this->bEnableControl = NewState; }

	// Setup new state for displaying info about State
	UFUNCTION(BlueprintCallable, Category = "AGameHUD|Managment")
	void SetupDebugInfoState(const bool NewState) { this->bEnableState = NewState; }

	// Setup new state for displaying info about Weapon
	UFUNCTION(BlueprintCallable, Category = "AGameHUD|Managment")
	void SetupDebugInfoWeapon(const bool NewState) { this->bEnableWeapon = NewState; }

protected:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

private:
	// Current pointer on AGamePlayMode class
	UPROPERTY()
	AGamePlayMode* GamePlayMode;
	// Current pointer on APlayerCharacter class
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

#pragma region Debug
	// Displaying debug information about Control
	UPROPERTY(EditDefaultsOnly, Category = "Debug|Managment")
	bool bEnableControl = true;

	// Displaying debug information about State
	UPROPERTY(EditDefaultsOnly, Category = "Debug|Managment")
	bool bEnableState = true;

	// Displaying debug information about Weapon
	UPROPERTY(EditDefaultsOnly, Category = "Debug|Managment")
	bool bEnableWeapon = true;
#pragma endregion

	// Game play widget
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidgetBase> GamePlayWidget;
	
	// Storing basic game widgets in the TMap
	TMap<EStateGamePlay, UUserWidgetBase*> GameWidgets;

	// Visible gameplay widget
	UPROPERTY()
	UUserWidgetBase* VisibleWidget = nullptr;
	
	// The basic gameplay has changed
	void OnChangedStateGamePlay(EStateGamePlay NewState);
};
