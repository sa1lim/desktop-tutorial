// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Item.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Player/MainCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	SetRootComponent(ItemMesh);

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	Collision->SetupAttachment(GetRootComponent());

	Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	LookWidget = CreateDefaultSubobject<UWidgetComponent>("Widget");
	LookWidget->SetupAttachment(GetRootComponent());

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetupAttachment(GetRootComponent());

	ItemState = EItemState::EIS_PickUp;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	LookWidget->SetVisibility(false);

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnBeginOverlapSpere);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AItem::OnEndOverlapSpere);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AItem::OnBeginOverlapSpere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainCharacter* Player = Cast<AMainCharacter>(OtherActor);
		if (Player)
		{
			Player->ChangeOverlapedItemCount(1);
		}
	}
}

void AItem::OnEndOverlapSpere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainCharacter* Player = Cast<AMainCharacter>(OtherActor);
		if (Player)
		{
			Player->ChangeOverlapedItemCount(-1);
		}
	}
}

void AItem::UseItem()
{}




void AItem::SetItemPropertiesOnState(EItemState State)
{
	switch (State)
	{
	case EItemState::EIS_PickUp:
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


		SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case EItemState::EIS_Equiped:
		LookWidget->SetVisibility(false);
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


		SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		break;
	case EItemState::EIS_Falling:
		ItemMesh->SetSimulatePhysics(true);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetEnableGravity(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

		SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;

	case EItemState::ETS_InInventory:
		LookWidget->SetVisibility(false);
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetVisibility(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
        
		Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AItem::SetState(EItemState State)
{
	ItemState = State;

	SetItemPropertiesOnState(ItemState);
}

