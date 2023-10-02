// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AiAngryCharacter.h"
#include "AI/Components/TEAIPerceptionComponent.h"

// Sets default values
AAiAngryCharacter::AAiAngryCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void AAiAngryCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAiAngryCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAiAngryCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

