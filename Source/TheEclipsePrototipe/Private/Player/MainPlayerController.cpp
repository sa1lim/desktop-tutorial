// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainPlayerController.h"
#include "Player/MainCharacter.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::DrawBullets(AMainCharacter* main)
{
	if (main->GetUsingWeapon())
	{
		if (WidgetClass)
		{
			MainWidget = CreateWidget<UUserWidget>(this, WidgetClass);
			MainWidget->AddToViewport();
		}
	}
}
