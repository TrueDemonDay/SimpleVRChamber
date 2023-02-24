// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MapWidget.h"
#include "PlayerGameInstance.h"

void UMapWidget::MapSelected(FName NameOfMap)
{
	if (GameInsRef)
	{
		GameInsRef->MapSelected(NameOfMap);
	}
}
