// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WeaponAmmoWidget.h"
#include "Components/TextBlock.h"

void UWeaponAmmoWidget::SetAmmoInWidget(int AmmoLeft, int AmmoCount)
{
	if (AmmoLeftBlock && AmmoCountBlock)
	{
		AmmoLeftBlock->SetText(FText::FromString(FString::FromInt(AmmoLeft)));
		AmmoCountBlock->SetText(FText::FromString(FString::FromInt(AmmoCount)));
	}
}
