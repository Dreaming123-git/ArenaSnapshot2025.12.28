// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArenaGameMode.h"
#include "ArenaCharacter.h"
#include "ArenaPlayerState.h"
#include "ArenaGameStateBase.h"
#include "UObject/ConstructorHelpers.h"

AArenaGameMode::AArenaGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}
void AArenaGameMode::StartPlay() 
{
	Super::StartPlay();
	// additional custom logic here
	GetWorld()->GetTimerManager().SetTimer(
		ExplodeHandle,          
		this, &AArenaGameMode::RoundEnd,
		RoundTime*60.f,                   
		false                   
	);
}

TArray<AArenaPlayerState*> AArenaGameMode::GetAlivePlayer()
{
	TArray<AArenaPlayerState*> AlivePlayers;

	if (AArenaGameStateBase* GS = GetGameState<AArenaGameStateBase>())
	{
		for (APlayerState* PS : GS->PlayerArray)
		{
			if (AArenaPlayerState* APS = Cast<AArenaPlayerState>(PS))
			{
				if (!APS->bIsDead)
				{
					AlivePlayers.Add(APS);
				}
			}
		}
	}
	return AlivePlayers;
}

void AArenaGameMode::NotifyPlayerDeath(AArenaPlayerState* DeadPlayer)
{
	if (!DeadPlayer) return;
	TArray<AArenaPlayerState*> AlivePlayers = GetAlivePlayer();
	if (AlivePlayers.Num() <= 1)
	{
		Winner = AlivePlayers.Num() == 1 ? AlivePlayers[0] : nullptr;
		
		RoundEnd();
	}
}

void AArenaGameMode::RoundEnd()
{
	AArenaGameStateBase* GS = GetGameState<AArenaGameStateBase>();
	if (!GS)
	{
		AGameStateBase* GSRaw = GetGameState<AGameStateBase>();
		if (GSRaw)
		{
			UE_LOG(LogTemp, Warning, TEXT("RoundEnd: GameState exists but is not AArenaGameStateBase. Actual: %s"), *GSRaw->GetClass()->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("RoundEnd: GameState is nullptr"));
		}
		return;
	}

	GS->NotifyRoundEnd(Winner);
}


