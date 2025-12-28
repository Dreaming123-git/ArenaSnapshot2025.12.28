// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ArenaGameStateBase.generated.h"

/**
 * 
 */
class AArenaPlayerState;
UCLASS()
class ARENA_API AArenaGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	AArenaGameStateBase();

	UFUNCTION(BlueprintImplementableEvent)
	void OnRoundEnd(AArenaPlayerState* winner);

	void NotifyRoundEnd(AArenaPlayerState* winner);

	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_Winner)
	AArenaPlayerState* WinnerPlayer = nullptr;

	UFUNCTION()
	void OnRep_Winner();

	// ¸´ÖÆÁÐ±í
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
};
