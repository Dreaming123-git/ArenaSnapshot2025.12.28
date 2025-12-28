// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ArenaPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ARENA_API AArenaPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AArenaPlayerState();

	UFUNCTION()
	void OnCharacterDeath(AActor* projectile);

	UPROPERTY()
	int32 CurrentScore = 0;

	UPROPERTY()
	bool bIsDead = false;
};
