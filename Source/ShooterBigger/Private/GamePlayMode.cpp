

#include "GamePlayMode.h"
#include "Characters/Player/PlayerCharacter.h"
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

	this->GameHUD = Cast<AGameHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	checkf(this->GameHUD, TEXT("Game HUD is nullptr"));
}
