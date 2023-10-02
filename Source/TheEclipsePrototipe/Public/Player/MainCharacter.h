// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/HealthComponent.h"
#include "MainCharacter.generated.h"

UENUM()
enum class EBulletType : uint8
{
	EBT_UspBullets	UMETA("UspBullets"),
	EBT_SgBullets	UMETA("SgBullets")
};
UCLASS()
class THEECLIPSEPROTOTIPE_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter(const FObjectInitializer& ObjectInint);

	UFUNCTION(BlueprintCallable)
		bool IsRunning() { return IsMovingForward && IsPlayerRunning; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UCameraComponent* CameraComp;

	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UHealthComponent* HealthComponent;

	void PlyerAiming();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Items")
		class AWeapon* AttachWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Items")
		class AItem* TraceItem;



	UPROPERTY(EditdefaultsOnly, BlueprintReadWrite, Category = "Items")
		TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditdefaultsOnly, BlueprintReadWrite, Category = "particles")
		class UParticleSystem* BeemParticle;

	UPROPERTY(EditdefaultsOnly, BlueprintReadWrite, Category = "particles")
		class UParticleSystem* Hit_PS;

	void OnFiring();
	void OnStopFiring();

	void StartTimer();

	void SetCanFireTrue();

	void DeattachWeapon();

	void SwapWepon(AWeapon* Weapon);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	int32 GetOverlapedItemCount() const { return CountOverlapedItems; }

	void ChangeOverlapedItemCount(int32 Count);

	bool GetShouldTakeWeapon() const { return CanTakeWeapon; }

	UFUNCTION(BlueprintCallable)
	bool GetUsingWeapon() const { return IsUsingWeapon; }

	bool IsDead() const { return !HealthComponent->IsAlive(); }

	UPROPERTY(EditdefaultsOnly, BlueprintReadWrite, Category = "Items")
	int32 Hospitals;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	TArray<AItem*> UsingInventory;

	bool IsInventoryOpen() const { return IsUsingInventory; }

	void LookUp(float Rate);

	void Turn(float Rate);

private:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void OnStartRunning();
	void OnEndRunning();

	bool IsMovingForward = false;
	bool IsPlayerRunning = false;

	UPROPERTY(VisibleAnywhere, Category = "Bools")
	bool IsAimimg;

	float CameraDefaultFOV;

	float ZoomedFOV;

	bool IsSeeingPickUp(FHitResult& OutHitResult);

	UPROPERTY(VisibleAnywhere, Category = "Var")
		class AItem* SeeingItem;

	bool ShouldTrace;

	int32 CountOverlapedItems;

	void TraceForItems();

	AWeapon* AttachingWeapon();

	void Fire();

	bool CanTakeWeapon;
    
	void EquipWeapon(AWeapon* WeaponEquip);

	void EquipWeaponInGame();

	bool IsFiring;

	float FireSpeed;

	bool CanFire;

	FTimerHandle AutoFireTeimer;

	void Death();

	bool IsUsingWeapon;

	TMap<EBulletType, int32> Ammo;

	int32 UspBullets;
	int32 SgBullets;

	void AtuoReload();

	void Reload();

	void UseHospital();

	void StopUsingHospital();

	bool IsUsingHospital;

	FTimerHandle HealthTimer;;

	const int32 CountofSlots = 20;
    
	void OpenUsingInventory();
	void CloseUsingInventory();

	bool IsUsingInventory;

	

	void NextWeapon();
};
