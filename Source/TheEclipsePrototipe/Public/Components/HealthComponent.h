// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class THEECLIPSEPROTOTIPE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(EditAnywhere, Blueprintreadwrite, CAtegory = "Health Prop")
		float MaxHealth;

	UFUNCTION(BlueprintCallable)
		float GetHealthProcent() const { return Health / MaxHealth; }

	FOnDeath OnDeath;

	UFUNCTION(BlueprintCallable)
		float GetHealth() const { return Health; }

	bool IsAlive() const { return Health <= 0.f; }

	void SetHealth(float HP) { Health = HP; }

	void HealthUper(float PlusHP) { Health += PlusHP; }

	bool IsFullHP() const { return Health == MaxHealth; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Health;

	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};