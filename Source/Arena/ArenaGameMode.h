// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArenaGameMode.generated.h"

class AArenaPlayerState;

UCLASS(minimalapi)
class AArenaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AArenaGameMode();
	void StartPlay() override;
	FTimerHandle ExplodeHandle;     
	void NotifyPlayerDeath(class AArenaPlayerState* DeadPlayer);
protected:
	UFUNCTION() 
	void RoundEnd();

	UFUNCTION()
	TArray< AArenaPlayerState* > GetAlivePlayer();

	UPROPERTY()
	float RoundTime = 10.0f;

	UPROPERTY()
	AArenaPlayerState* Winner;

};



