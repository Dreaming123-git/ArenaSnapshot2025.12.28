// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArenaProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "ArenaShootableObject.h"
#include "AttributeComponent.h"
#include "ArenaCharacter.h"

AArenaProjectile::AArenaProjectile()
{
	SetReplicates(true);
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AArenaProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AArenaProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//if (!HasAuthority()) return;
	if (!OtherActor || OtherActor == this) return;

	if (ACharacter* HitCharacter = Cast<AArenaCharacter>(OtherActor))
	{
		if (UAttributeComponent* Attr = HitCharacter->FindComponentByClass<UAttributeComponent>())
		{
			Attr->ApplyDamage(50.f, this);   
		}
	}
	if (AArenaShootableObject* shootableObject = Cast<AArenaShootableObject>(OtherActor))
	{
		shootableObject->OnHitByProjectile();
	}
}