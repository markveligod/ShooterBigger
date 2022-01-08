﻿#pragma once
#include "CharacterDataTypes.generated.h"

// The basic state move of the character
UENUM()
enum class EStateMoveCharacter : uint8
{
	None = 0,
	Idle,
	Walk,
	Running
};

// The state of the player when aiming or from the hip
UENUM()
enum class EStateAim : uint8
{
	None = 0,
	Hip,
	Aiming
};

// The state of the weapon that is in the pseudo inventory
UENUM()
enum class EStateWeapon : uint8
{
	None = 0,
	Pistol,
	Rifle
};
