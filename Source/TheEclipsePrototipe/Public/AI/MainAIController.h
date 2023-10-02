// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MainAIController.generated.h"

/**
 * 
 */
UCLASS()
class THEECLIPSEPROTOTIPE_API AMainAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	AMainAIController();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		class UAIPerceptionComponent* NewPerceptionComponent;

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
