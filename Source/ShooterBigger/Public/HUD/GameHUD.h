

#pragma once

#include "CoreMinimal.h"
#include "HUD/DebugHUD.h"
#include "GameHUD.generated.h"

class AGamePlayMode;
/**
 *
 */
UCLASS()
class SHOOTERBIGGER_API AGameHUD : public ADebugHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

private:
	// Current pointer on AGamePlayMode class
	UPROPERTY()
	AGamePlayMode* GamePlayMode;
};
