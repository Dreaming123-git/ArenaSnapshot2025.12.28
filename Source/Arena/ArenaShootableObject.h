// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArenaShootableObject.generated.h"

UCLASS()
class ARENA_API AArenaShootableObject : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ObjectMesh;
	
	
public:	
	// Sets default values for this actor's properties
	AArenaShootableObject();
	UFUNCTION()
	int32 GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION()
	void OnHitByProjectile();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Health")
	int32 TotalHealth=100;

	UPROPERTY(EditAnywhere, Category = "Score")
	int32 ScorePerHit=10;

	UPROPERTY()
	int32 CurrentHealth = 100;

public:	
};
