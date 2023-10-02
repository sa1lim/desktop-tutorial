// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MainCharacterMovementComponent.h"
#include "Player/MainCharacter.h"

float UMainCharacterMovementComponent::GetMaxSpeed() const
{
	auto MaxSpeed = Super::GetMaxSpeed();
	auto Player = Cast<AMainCharacter>(GetPawnOwner());
	return Player && Player->IsRunning() ? MaxSpeed * 2 : MaxSpeed;
}
