// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MenuWidget.h"
#include "PlayerGameInstance.h"

void UMenuWidget::LevelSelectorPressed()
{
	if (GameInsRef)
	{
		GameInsRef->OpenLevelSelector();
	}
}

void UMenuWidget::ResetPressed()
{
	if (GameInsRef)
	{
		GameInsRef->ResetPlayerPosition();
	}
}

void UMenuWidget::QuitPressed()
{
	if (GameInsRef)
	{
		GameInsRef->Exit();
	}
}
