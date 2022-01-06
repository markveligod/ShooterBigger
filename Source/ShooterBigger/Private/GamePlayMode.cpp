

#include "GamePlayMode.h"
#include "Characters/Player/PlayerCharacter.h"
#include "HUD/GameHUD.h"
#include "Kismet/GameplayStatics.h"

AGamePlayMode::AGamePlayMode()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	HUDClass = AGameHUD::StaticClass();
}

void AGamePlayMode::BeginPlay()
{
	Super::BeginPlay();

	this->PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	checkf(this->PlayerCharacter, TEXT("Player character is nullptr"));
}
