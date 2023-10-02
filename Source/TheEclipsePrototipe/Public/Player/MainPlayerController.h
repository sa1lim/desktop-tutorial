// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class THEECLIPSEPROTOTIPE_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void DrawBullets(class AMainCharacter* main);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "widgets")
		TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY()
		UUserWidget* MainWidget;
};
