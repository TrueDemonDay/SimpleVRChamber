// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MapSelecWidget.h"
#include "PlayerGameInstance.h"

void UMapSelecWidget::CloseMapSelect()
{
	if (GameInsRef)
	{
		GameInsRef->CloseLevelSelector();
	}
}
