// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Tests/DamageActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ADamageActor::ADamageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("Collision");
	SetRootComponent(SphereCollision);
}

// Called when the game starts or when spawned
void ADamageActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), 240, 32, FColor::Red);
	
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 1.f, GetActorLocation(), 240, nullptr, {}, this, nullptr, false);
	
}

