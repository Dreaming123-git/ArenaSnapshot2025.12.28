// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArenaMainMenuWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class ARENA_API UArenaMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	UButton* StartButton_Cpp;

	virtual void NativeOnInitialized() override;
	virtual bool Initialize() override 
	{ 
		Super::Initialize(); 
		return true; 
	};


	UFUNCTION()
	void OnStartButtonClicked();
};
