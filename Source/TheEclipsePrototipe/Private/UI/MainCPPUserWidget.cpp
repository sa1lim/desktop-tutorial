// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainCPPUserWidget.h"
#include "Player/MainCharacter.h"

bool UMainCPPUserWidget::DrawBullets() const
{
	const auto Player = GetOwningPlayerPawn();
	if (Player)
	{
		AMainCharacter* Main = Cast<AMainCharacter>(Player);
		if (Main)
		{
			if (Main->GetUsingWeapon())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

bool UMainCPPUserWidget::IsPlayerAlive() const
{
	const auto Player = GetOwningPlayerPawn();
	if (Player)
	{
		AMainCharacter* Main = Cast<AMainCharacter>(Player);
		if (Main->IsDead())
		{
			return false;
		}
		return true;
	}
	return true;
}

bool UMainCPPUserWidget::OpenUsingInventory() const
{
	const auto Player = GetOwningPlayerPawn();
	if (Player)
	{
		AMainCharacter* Main = Cast<AMainCharacter>(Player);
		if (Main->IsInventoryOpen())
		{
			return true;
		}
		return false;
	}
	return false;
}
