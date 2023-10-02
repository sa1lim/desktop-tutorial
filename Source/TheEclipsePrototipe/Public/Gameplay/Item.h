// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_PickUp		UMETA("PickUp"),
	EIS_Taked		UMETA("Taked"),
	EIS_Falling		UMETA("Folling"),
	EIS_FlyToPlayer	UMETA("FlyToPlayer"),
	EIS_Equiped		UMETA("Equiped"),
	ETS_InInventory	UMETA("InInventory")
};

	
UCLASS()
class THEECLIPSEPROTOTIPE_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camponents")
		USkeletalMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camponents")
		class UBoxComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camponents")
		class UWidgetComponent* LookWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camponents")
		class USphereComponent* SphereCollision;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UWidgetComponent* GetWidget() const { return LookWidget; }
	USphereComponent* GetSpereComponent() const { return SphereCollision; }
	UBoxComponent* GetCollision() const { return Collision; }
	USkeletalMeshComponent* GetItemMesh() const { return ItemMesh; }

	EItemState GetState() const { return ItemState; }
	void SetState(EItemState State);

	UFUNCTION()
		void OnBeginOverlapSpere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlapSpere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void UseItem();

	
	

private:
	EItemState ItemState = EItemState::EIS_PickUp;

	void SetItemPropertiesOnState(EItemState State);


};
