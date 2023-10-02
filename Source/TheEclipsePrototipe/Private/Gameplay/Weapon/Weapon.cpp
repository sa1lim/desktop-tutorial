// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Weapon/Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Math/TransformNonVectorized.h"
#include "Player/MainCharacter.h"

AWeapon::AWeapon()
{
	CurrentAmmo = 30;

	CurrentMagazins = 2;
}

void AWeapon::UseItem()
{
	if (CurrentAmmo > 0)
	{
		CurrentAmmo -= 1;
		const USkeletalMeshSocket* FireSocket = GetItemMesh()->GetSocketByName(FName("FireSocket"));
		if (FireSocket)
		{
			FTransform FireSocketTransform = FireSocket->GetSocketTransform(GetItemMesh());

			FHitResult FireHitResult;

			FVector CameraRotation;

			FVector2D ViewPortSize;
			if (GEngine && GEngine->GameViewport)
			{
				GEngine->GameViewport->GetViewportSize(ViewPortSize);
			}

			FVector2D CentrOfVieport;
			CentrOfVieport.X = ViewPortSize.X / 2.f;
			CentrOfVieport.Y = ViewPortSize.Y / 2.f;

			FVector CenterPosition;
			FVector CenterDirection;

			bool ScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CentrOfVieport, CenterPosition, CenterDirection);

			if (ScreenToWorld)
			{
				FVector Start{ CenterPosition };
				FVector End{ CenterPosition + CenterDirection * 50000.f };

				FVector BeemEndPoint{ End };

				GetWorld()->LineTraceSingleByChannel(FireHitResult, Start, End, ECollisionChannel::ECC_Visibility);

				if (FireHitResult.bBlockingHit)
				{
					BeemEndPoint = FireHitResult.Location;
				}

				FVector WeaponStart{ FireSocketTransform.GetLocation() };
				FVector WeaponEnd{ BeemEndPoint };
				FHitResult WeaponHitResult;
				GetWorld()->LineTraceSingleByChannel(WeaponHitResult, WeaponStart, WeaponEnd, ECollisionChannel::ECC_Visibility);

				if (WeaponHitResult.bBlockingHit)
				{
					BeemEndPoint = WeaponHitResult.Location;
				}

				if (Hit_PS)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Hit_PS, BeemEndPoint);
				}

				if (BeemParticle)
				{
					UParticleSystemComponent* Beem = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeemParticle, FireSocketTransform);
					if (Beem)
					{
						Beem->SetVectorParameter("Target", BeemEndPoint);
					}
				}

			}
		}

	}
}



void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetState() == EItemState::EIS_Falling && IsWeaponFalling)
	{
		FRotator MeshRotation{ 0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f };
		GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::ResetPhysics);

	}

}

void AWeapon::ThrowWeapon()
{
	FRotator MeshRotation{ 0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f };
	GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::ResetPhysics);

	const FVector ForwardVector{ GetItemMesh()->GetForwardVector() };
	const FVector RightVector{ GetItemMesh()->GetRightVector() };

	FVector NewImpulse = RightVector.RotateAngleAxis(-20.f, ForwardVector);

	float NewRotation = 90.f;

	NewImpulse = NewImpulse.RotateAngleAxis(NewRotation, FVector(1.f));
	NewImpulse *= 20000.f;

	GetItemMesh()->AddImpulse(NewImpulse);

	IsWeaponFalling = true;

	GetWorldTimerManager().SetTimer(StopFallingTimer, this, &AWeapon::WeaponDontFall, 1.f);
}

void AWeapon::WeaponDontFall()
{
	IsWeaponFalling = false;

	SetState(EItemState::EIS_PickUp);
}

