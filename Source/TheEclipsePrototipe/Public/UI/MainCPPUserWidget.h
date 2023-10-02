// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainCPPUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEECLIPSEPROTOTIPE_API UMainCPPUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		bool DrawBullets() const;

	UFUNCTION(BlueprintCallable)
		bool  IsPlayerAlive() const;

	UFUNCTION(BlueprintCallable)
		bool OpenUsingInventory() const;
};
