// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class JOYWAYTEST_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	class UPlayerGameInstance* GameInsRef = nullptr;

	UFUNCTION(BlueprintCallable)
	void LevelSelectorPressed();

	UFUNCTION(BlueprintCallable)
	void ResetPressed();

	UFUNCTION(BlueprintCallable)
	void QuitPressed();

};
