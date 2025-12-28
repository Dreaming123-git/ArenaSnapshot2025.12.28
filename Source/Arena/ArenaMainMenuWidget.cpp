// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaMainMenuWidget.h"
#include "Components/Button.h"
 
void UArenaMainMenuWidget::NativeOnInitialized()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Initialized."));

	Super::NativeOnInitialized();
	/*if (!StartButton_Cpp)
	{
		return;
	}*/
	StartButton_Cpp->OnClicked.AddDynamic(this, &UArenaMainMenuWidget::OnStartButtonClicked);
}

void UArenaMainMenuWidget::OnStartButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartButton was clicked."));
}
