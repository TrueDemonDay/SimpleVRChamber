// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrabItemBase.h"
#include "ShootingItem.generated.h"


class UWeaponAmmoWidget;
/**
 * 
 */
UCLASS()
class JOYWAYTEST_API AShootingItem : public AGrabItemBase
{
	GENERATED_BODY()
	

public:

	UPROPERTY(VisibleAnywhere, Category = Shooting)
	USceneComponent* ShootPoint = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Shooting)
	UWeaponAmmoWidget* AmmoWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Shooting)
	bool bAutoMode = false;

	UPROPERTY(EditDefaultsOnly, Category = Shooting)
	float ShootingSpeed = 0.15f;

	UPROPERTY(EditDefaultsOnly, Category = Shooting)
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Shooting)
	int MaxAmmoCount = 10;

	int AmmoLeft = MaxAmmoCount;

	FTimerHandle TimerForShooting;

	bool bShoot = false;

	void ShootFromScene();

	virtual void StartUseItem_Implementation() override;
	virtual void StopUseItem_Implementation() override;
	virtual void GrabItem_Implementation(UMotionControllerComponent* ControllerToAttach) override;
	virtual void DropItem_Implementation() override;


	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	void SetAmmoLeft(int NewAmmoLeft);

	UFUNCTION(BlueprintCallable)
	bool IsNeeedReload();

	//------------------------Init varrible functions----------------------//
	UFUNCTION(BlueprintCallable)
	void SetWidgetRef(UWeaponAmmoWidget* NewWeaponWidget);

	//Set shoot point in BP, i know it's not best solution, but i haven't models to make sockets, so i think right now it's fine =)
	UFUNCTION(BlueprintCallable)
	void SetShootPoint(USceneComponent* NewShootPoint);
};
