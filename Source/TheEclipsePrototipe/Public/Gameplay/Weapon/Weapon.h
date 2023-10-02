// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Item.h"
#include "Weapon.generated.h"

UCLASS()
class THEECLIPSEPROTOTIPE_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	AWeapon();

	virtual void UseItem() override;

	virtual void Tick(float DeltaTime) override;

	void ThrowWeapon();

	void WeaponDontFall();

	bool IsWeaponFalling;

	UPROPERTY(EditdefaultsOnly, BlueprintReadWrite, Category = "particles")
		class UParticleSystem* BeemParticle;

	UPROPERTY(EditdefaultsOnly, BlueprintReadWrite, Category = "particles")
		class UParticleSystem* Hit_PS;

	int32 GetAmmo() const { return CurrentAmmo; }

	int32 GetMagazins() const { return CurrentMagazins; }

	int32 GetMaxBullets() const { return 30; }

	void SetBullets(int32 Bullets) { CurrentAmmo = Bullets; }

	void SetMagazins(int32 Magazin) { CurrentMagazins = Magazin; }

	FTimerHandle StopFallingTimer;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int32 CurrentMagazins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tecstur")
		UTexture2D* WeaponTexture;
};
