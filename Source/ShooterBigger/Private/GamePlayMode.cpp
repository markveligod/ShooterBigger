

#include "GamePlayMode.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogGamePlayMode, All, All);

AGamePlayMode::AGamePlayMode()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	HUDClass = AGameHUD::StaticClass();
}

void AGamePlayMode::ChangeStateGamePlay(EStateGamePlay NewState)
{
	if (this->StateGamePlay == NewState)
	{
		UE_LOG(LogGamePlayMode, Warning, TEXT("Current state game play mode equal new state: %s"),
			*UEnum::GetValueAsString(NewState));
		return;
	}

	this->StateGamePlay = NewState;
	this->OnStateGamePlayChanged.Broadcast(NewState);
	UE_LOG(LogGamePlayMode, Display, TEXT("New State game play: %s"), *UEnum::GetValueAsString(NewState));
}

void AGamePlayMode::BeginPlay()
{
	Super::BeginPlay();

	this->PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	checkf(this->PlayerCharacter, TEXT("Player character is nullptr"));

	this->GameHUD = Cast<AGameHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	checkf(this->GameHUD, TEXT("Game HUD is nullptr"));

}

void AGamePlayMode::StartPlay()
{
	Super::StartPlay();
	
	this->ChangeStateGamePlay(EStateGamePlay::GameProcess);
}
