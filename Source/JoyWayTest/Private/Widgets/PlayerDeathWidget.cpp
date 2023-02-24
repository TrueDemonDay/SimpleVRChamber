// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerDeathWidget.h"
#include "Components/TextBlock.h"

void UPlayerDeathWidget::StartTimerBack(int NewTimeLeft)
{
	TimeLeft = NewTimeLeft;
	Countdown();
	SetVisibility(ESlateVisibility::Visible);
}

void UPlayerDeathWidget::Countdown()
{
	if (TimeLeft == 0)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		if (TimerBlock)
			TimerBlock->SetText(FText::FromString(FString::FromInt(TimeLeft)));

		TimeLeft--;
		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &UPlayerDeathWidget::Countdown, 1);
	}
}
