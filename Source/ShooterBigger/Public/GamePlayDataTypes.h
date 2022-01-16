#pragma once
#include "GamePlayDataTypes.generated.h"

UENUM(BlueprintType)
enum class EStateGamePlay : uint8
{
	None = 0,
	GameProcess
};

// Signature on changed base game process in EStateGamePlay
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateGamePlayChangedSignature, EStateGamePlay);
