// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapWidget.generated.h"

/**
 * 
 */
UCLASS()
class JOYWAYTEST_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPlayerGameInstance* GameInsRef = nullptr;

	UFUNCTION(BlueprintCallable)
	void MapSelected(FName NameOfMap);
};
