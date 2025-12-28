// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "ArenaPlayerState.h"

AArenaGameStateBase::AArenaGameStateBase()
	: Super()
	, WinnerPlayer(nullptr)
{
}

void AArenaGameStateBase::NotifyRoundEnd(AArenaPlayerState* winner)
{
	if (!HasAuthority()) return;
	WinnerPlayer = winner;
	//OnRoundEnd(WinnerPlayer);
}


void AArenaGameStateBase::OnRep_Winner()
{
	if (HasAuthority()) return;
	OnRoundEnd(WinnerPlayer);
}

void AArenaGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AArenaGameStateBase, WinnerPlayer);
}