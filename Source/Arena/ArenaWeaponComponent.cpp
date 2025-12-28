// Copyright Epic Games, Inc. All Rights Reserved.


#include "ArenaWeaponComponent.h"
#include "ArenaCharacter.h"
#include "ArenaProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Engine/Engine.h"


// Sets default values for this component's properties
UArenaWeaponComponent::UArenaWeaponComponent()
{
	SetIsReplicatedByDefault(true);
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UArenaWeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
	
	ServerFire();
}

bool UArenaWeaponComponent::ServerFire_Validate()
{
	return true;
}

void UArenaWeaponComponent::ServerFire_Implementation()
{
	if (!Character->HasAuthority())
	{
		return;
	}
	// create a projectile with replication
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
			if (World == nullptr)
			{
				return;
			}
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// Spawn the projectile at the muzzle
			AArenaProjectile* projectile=World->SpawnActor<AArenaProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if(projectile)
			{
				projectile->InstigatorPS = Cast<AArenaPlayerState>(Character->GetPlayerState());
			}
		}
	}
}


bool UArenaWeaponComponent::AttachWeapon(AArenaCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UArenaWeaponComponent>())
	{
		return false;
	}

	//If Character has picked up a weapon, fail to attach a new one.
	if (Character->GetInstanceComponents().FindItemByClass<UArenaWeaponComponent>())
		return false;

	////Judge if the Character is the local player.
	//USkeletalMeshComponent* TargetMesh = nullptr;
	//if (Character->IsLocallyControlled())			
	//{
	//	TargetMesh = Character->GetMesh1P();
	//}
	//else											
	//{
	//	TargetMesh = Character->GetMesh();	
	//}

	//if (TargetMesh == nullptr)
	//{
	//	GEngine->AddOnScreenDebugMessage(
	//		-1,
	//		10.f,
	//		FColor::Red,
	//		TEXT("Error: No SM to attach weapon")
	//	);
	//	return false;
	//}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	this->SetOnlyOwnerSee(true);

	GetOwner()->SetOwner(Character);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UArenaWeaponComponent::Fire);
		}
	}

	return true;
}

void UArenaWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// ensure we have a character owner
	if (Character && IsValid(Character))
	{
		// remove the input mapping context from the Player Controller
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(FireMappingContext);
			}
		}
	}

	// maintain the EndPlay call chain
	Super::EndPlay(EndPlayReason);
}


