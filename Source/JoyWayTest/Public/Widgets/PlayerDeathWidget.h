// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerDeathWidget.generated.h"

/**
 * 
 */
UCLASS()
class JOYWAYTEST_API UPlayerDeathWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TimerBlock;

	void StartTimerBack(int NewTimeLeft);

	int TimeLeft = 0;

	void Countdown();
};
