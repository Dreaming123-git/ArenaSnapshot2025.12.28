// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaPlayerState.h"
#include "ArenaGameMode.h"

AArenaPlayerState:: AArenaPlayerState()
{
	CurrentScore = 0;
	bIsDead = false;
}

void AArenaPlayerState::OnCharacterDeath(AActor* projectile)
{
	
	bIsDead = true;
	// Optionally, you can add logic to update the score or notify other systems about the death event.
	if (UWorld* World = GetWorld())
	{
		if (AArenaGameMode* GM = Cast<AArenaGameMode>(World->GetAuthGameMode()))
		{
			GM->NotifyPlayerDeath(this);
		}
	}
}