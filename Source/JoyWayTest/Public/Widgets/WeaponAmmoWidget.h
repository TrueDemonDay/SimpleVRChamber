// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponAmmoWidget.generated.h"

/**
 * 
 */
UCLASS()
class JOYWAYTEST_API UWeaponAmmoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* AmmoLeftBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* AmmoCountBlock;

	void SetAmmoInWidget(int AmmoLeft, int AmmoCount);
};
