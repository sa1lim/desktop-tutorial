// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MainAIController.h"

#include "AI/Components/TEAIPerceptionComponent.h"

AMainAIController::AMainAIController()
{
	NewPerceptionComponent = CreateDefaultSubobject < UAIPerceptionComponent>("PerceptionComponent");
	SetPerceptionComponent(*NewPerceptionComponent);
}

void AMainAIController::OnPossess(APawn* InPawn)
{
}
