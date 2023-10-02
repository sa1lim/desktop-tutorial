// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainCharacter.h"
#include "Components/MainCharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/Item.h"
#include "Components/WidgetComponent.h"
#include "Gameplay/Weapon/Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/HealthComponent.h"
#include "Gameplay/Helpers/HealthItem.h"

// Sets default values
AMainCharacter::AMainCharacter(const FObjectInitializer& ObjectInint) : Super(ObjectInint.SetDefaultSubobjectClass<UMainCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(GetMesh(), FName("Head"));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");

	IsAimimg = false;

	CameraDefaultFOV = 0.0f;

	ZoomedFOV = 40.f;

	ShouldTrace = false;

	CanTakeWeapon = false;

	CanFire = true;

	IsFiring = false;

	FireSpeed = 0.1f;

	UspBullets = 30;
	SgBullets = 40;

	Hospitals = 0;

	IsUsingHospital = false;

	IsUsingInventory = false;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent)
	
	if (CameraComp)
	{
		CameraDefaultFOV = CameraComp->FieldOfView;
	}

	HealthComponent->OnDeath.AddUObject(this, &AMainCharacter::Death);
}



// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForItems();

	if (IsFiring)
	{
		AtuoReload();
	}

	if (IsUsingHospital)
	{
		HealthComponent->HealthUper(0.1f);
	}

	UE_LOG(LogTemp, Warning, TEXT("%i"), UsingInventory.Num())
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::Turn);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMainCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AMainCharacter::OnEndRunning);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMainCharacter::PlyerAiming);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::OnFiring);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMainCharacter::OnStopFiring);
	PlayerInputComponent->BindAction("Take", IE_Pressed, this, &AMainCharacter::EquipWeaponInGame);
	PlayerInputComponent->BindAction("ThrowWepaon", IE_Pressed, this, &AMainCharacter::DeattachWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMainCharacter::Reload);
	PlayerInputComponent->BindAction("Hill", IE_Pressed, this, &AMainCharacter::UseHospital);
	PlayerInputComponent->BindAction("OpenUsingInventory", IE_Pressed, this, &AMainCharacter::OpenUsingInventory);
	PlayerInputComponent->BindAction("OpenUsingInventory", IE_Released, this, &AMainCharacter::CloseUsingInventory);
}

void AMainCharacter::MoveForward(float Value)
{
	
	IsMovingForward = Value > 0.f;
	AddMovementInput(GetActorForwardVector() * Value);
}

void AMainCharacter::MoveRight(float Value)
{
	
	AddMovementInput(GetActorRightVector() * Value);
}

void AMainCharacter::LookUp(float Rate)
{
	if (!IsUsingInventory)
	{
		AddControllerPitchInput(Rate);
	}
}

void AMainCharacter::Turn(float Rate)
{
	if (!IsUsingInventory)
	{
		AddControllerYawInput(Rate);
	}
}


void AMainCharacter::OnStartRunning()
{
	IsPlayerRunning = true;
}

void AMainCharacter::OnEndRunning()
{
	IsPlayerRunning = false;
}

void AMainCharacter::PlyerAiming()
{
	if (!IsAimimg)
	{
		IsAimimg = true;

		CameraComp->SetFieldOfView(ZoomedFOV);

		if (AttachWeapon)
		{
			AttachWeapon->GetItemMesh()->SetVisibility(false);
		}
	}
	else
	{
		IsAimimg = false;

		CameraComp->SetFieldOfView(CameraDefaultFOV);

		if (AttachWeapon)
		{
			AttachWeapon->GetItemMesh()->SetVisibility(true);
		}
	}
}

bool AMainCharacter::IsSeeingPickUp(FHitResult& OutHitResult)
{
	FVector2D ViewPortSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
	}

	FVector2D CentrOfVieport;
	CentrOfVieport.X = ViewPortSize.X / 2.f;
	CentrOfVieport.Y = ViewPortSize.Y / 2.f;
	CentrOfVieport.Y -= 50;
	
	FVector CenterPosition;
	FVector CenterDirection;

	bool IsScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CentrOfVieport, CenterPosition, CenterDirection);

	if (IsScreenToWorld)
	{
		FVector Start{ CenterPosition };
		FVector End{ Start + CenterDirection * 500.f };
		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Visibility);
		if (OutHitResult.bBlockingHit)
		{
			return true;
		}
	}
	return false;
}

void AMainCharacter::ChangeOverlapedItemCount(int32 Count)
{
	if (CountOverlapedItems + Count <= 0)
	{
		CountOverlapedItems = 0;
		ShouldTrace = false;
	}
	else
	{
		CountOverlapedItems += Count;
		ShouldTrace = true;
	}
}

void AMainCharacter::TraceForItems()
{
	if (ShouldTrace)
	{
		FHitResult TickHitResult;
		IsSeeingPickUp(TickHitResult);

		TraceItem = Cast<AItem>(TickHitResult.Actor);

		if (TickHitResult.bBlockingHit)
		{
			if (TraceItem && TraceItem->GetWidget())
			{
				TraceItem->GetWidget()->SetVisibility(true);
				CanTakeWeapon = true;
			}

			if (SeeingItem)
			{
				if (TraceItem != SeeingItem)
				{
					SeeingItem->GetWidget()->SetVisibility(false);

				}
			}

			SeeingItem = TraceItem;
		}
	}
	else if (SeeingItem)
	{
		SeeingItem->GetWidget()->SetVisibility(false);
		CanTakeWeapon = false;
	}
}

AWeapon* AMainCharacter::AttachingWeapon()
{
	if (CanTakeWeapon)
	{
		if (WeaponClass)
		{
			return GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		}

		return nullptr;
	}
	return nullptr;
}

void AMainCharacter::Fire()
{
	if (AttachWeapon && AttachWeapon->GetState() == EItemState::EIS_Equiped)
	{
		AttachWeapon->UseItem();
	}
	
}

void AMainCharacter::EquipWeapon(AWeapon* WeaponEquip)
{
	if (WeaponEquip)
	{
		const USkeletalMeshSocket* WeaponSocket = GetMesh()->GetSocketByName(FName("WeaponSocket"));
		if (WeaponSocket)
		{
			WeaponSocket->AttachActor(WeaponEquip, GetMesh());
			AttachWeapon = WeaponEquip;
			WeaponEquip->SetState(EItemState::EIS_Equiped);
			IsUsingWeapon = true;
		}
	}
}

void AMainCharacter::DeattachWeapon()
{
	if(AttachWeapon)
	{
		FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
		AttachWeapon->GetItemMesh()->DetachFromComponent(DetachmentRules);

		AttachWeapon->SetState(EItemState::EIS_Falling);
		AttachWeapon->ThrowWeapon();
		UsingInventory.Remove(AttachWeapon);
		IsUsingWeapon = false;
		AttachWeapon = nullptr;
	}
}

void AMainCharacter::SwapWepon(AWeapon* Weapon)
{
	DeattachWeapon();
	EquipWeapon(Weapon);
}


void AMainCharacter::EquipWeaponInGame()
{
	if (TraceItem)
	{
		auto TraceHit = Cast<AWeapon>(TraceItem);
		if(TraceHit)
		{
			if (AttachWeapon)
			{
				if (UsingInventory.Num() < CountofSlots)
				{
					UsingInventory.Add(TraceHit);
					TraceHit->SetState(EItemState::ETS_InInventory);
				}
			}
			else
			{
				if (UsingInventory.Num() < CountofSlots)
				{

					SwapWepon(TraceHit);
				}
			}
			SeeingItem = nullptr;
			TraceItem = nullptr;
		}
		auto SecondTraceHit = Cast<AHealthItem>(TraceItem);
		if (SecondTraceHit)
		{
			Hospitals++;
			SecondTraceHit->Destroy();
			TraceItem = nullptr;
			
		}
	}
}

void AMainCharacter::OnFiring()
{
	IsFiring = true;
	StartTimer();
}

void AMainCharacter::OnStopFiring()
{
	IsFiring = false;
}

void AMainCharacter::StartTimer()
{
	if (CanFire)
	{
		Fire();
		CanFire = false;
		GetWorldTimerManager().SetTimer(AutoFireTeimer, this, &AMainCharacter::SetCanFireTrue, FireSpeed);
	}
}

void AMainCharacter::SetCanFireTrue()
{
	CanFire = true;
	if (IsFiring)
	{
		StartTimer();
	}
}

void AMainCharacter::Death()
{
	GetMesh()->SetSimulatePhysics(true);

	GetCharacterMovement()->DisableMovement();

	Destroy();

	if (AttachWeapon)
	{
		AttachWeapon->Destroy();
	}
}

void AMainCharacter::AtuoReload()
{
	if (AttachWeapon)
	{
		if (AttachWeapon->GetAmmo() == 0 && AttachWeapon->GetMagazins() - 1 > -1)
		{
			AttachWeapon->SetBullets(AttachWeapon->GetMaxBullets());
			AttachWeapon->SetMagazins(AttachWeapon->GetMagazins() - 1);
		}
	}
}

void AMainCharacter::Reload()
{
	if (AttachWeapon)
	{
		if (AttachWeapon->GetMagazins() - 1 > -1 && AttachWeapon->GetAmmo() != AttachWeapon->GetMaxBullets())
		{
			AttachWeapon->SetBullets(AttachWeapon->GetMaxBullets());
			AttachWeapon->SetMagazins(AttachWeapon->GetMagazins() - 1);
		}
	}
}

void AMainCharacter::UseHospital()
{
	if (!HealthComponent->IsFullHP())
	{
		if (Hospitals - 1 >= 0)
		{
			IsUsingHospital = true;

			Hospitals--;

			GetWorldTimerManager().SetTimer(HealthTimer, this, &AMainCharacter::StopUsingHospital, 2.f);
		}
	}
	else
	{
		return;
	}
}

void AMainCharacter::StopUsingHospital()
{
	IsUsingHospital = false;
}

void AMainCharacter::OpenUsingInventory()
{
	IsUsingInventory = true;

}

void AMainCharacter::CloseUsingInventory()
{
	IsUsingInventory = false;


}


void AMainCharacter::NextWeapon()
{

}
