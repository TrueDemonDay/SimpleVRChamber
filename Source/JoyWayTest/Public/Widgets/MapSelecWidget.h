// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapSelecWidget.generated.h"

/**
 * 
 */
UCLASS()
class JOYWAYTEST_API UMapSelecWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	class UPlayerGameInstance* GameInsRef = nullptr;

	UFUNCTION(BlueprintCallable)
	void CloseMapSelect();
};
