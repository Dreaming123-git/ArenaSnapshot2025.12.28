// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShootableObject.h"

// Sets default values
AArenaShootableObject::AArenaShootableObject()
{
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));

}

// Called when the game starts or when spawned
void AArenaShootableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArenaShootableObject::OnHitByProjectile()
{
	if (CurrentHealth <= 0)
	{
		Destroy();
		return;
	}
	CurrentHealth -= 50;
	if (CurrentHealth <= 0)
	{
		Destroy();
	}
}



