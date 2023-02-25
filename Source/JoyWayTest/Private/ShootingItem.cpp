// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingItem.h"
#include "Widgets/WeaponAmmoWidget.h"

void AShootingItem::ShootFromScene()
{
	if (ShootPoint && ProjectileClass && bShoot && AmmoLeft>0)
	{
		AmmoLeft--;
		ItemInfo.AmmoLeft = AmmoLeft;
		if (AmmoWidget)
			AmmoWidget->SetAmmoInWidget(AmmoLeft, MaxAmmoCount);
		FTransform SpawnTransform = ShootPoint->GetComponentTransform();
		SpawnTransform.SetScale3D(FVector((1, 1, 1)));
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform);
	}
	else
	GetWorld()->GetTimerManager().PauseTimer(TimerForShooting);
}

void AShootingItem::StartUseItem_Implementation()
{
	if (!bShoot)
	{
		bShoot = true;
		ShootFromScene();
		if (bAutoMode)
			GetWorld()->GetTimerManager().SetTimer(TimerForShooting, this, &AShootingItem::ShootFromScene, ShootingSpeed, bAutoMode);
	}
}

void AShootingItem::StopUseItem_Implementation()
{
	bShoot = false;
}

void AShootingItem::GrabItem_Implementation(UMotionControllerComponent * ControllerToAttach)
{
	Grab(ControllerToAttach);
	if (AmmoWidget)
		AmmoWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void AShootingItem::DropItem_Implementation()
{
	Drop();
	if (AmmoWidget)
		AmmoWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AShootingItem::SetWidgetRef(UWeaponAmmoWidget * NewWeaponWidget)
{
	AmmoWidget = NewWeaponWidget;
	if (AmmoWidget)
		AmmoWidget->SetAmmoInWidget(AmmoLeft, MaxAmmoCount);
}

void AShootingItem::SetShootPoint(USceneComponent * NewShootPoint)
{
	ShootPoint = NewShootPoint;
	Reload();
}

void AShootingItem::Reload()
{
	AmmoLeft = MaxAmmoCount;
	ItemInfo.AmmoLeft = AmmoLeft;
	if (AmmoWidget)
		AmmoWidget->SetAmmoInWidget(AmmoLeft, MaxAmmoCount);
}

void AShootingItem::SetAmmoLeft(int NewAmmoLeft)
{
	AmmoLeft = NewAmmoLeft;
	ItemInfo.AmmoLeft = AmmoLeft;
	if (AmmoWidget)
		AmmoWidget->SetAmmoInWidget(AmmoLeft, MaxAmmoCount);
}

bool AShootingItem::IsNeeedReload()
{
	return AmmoLeft != MaxAmmoCount;
}
