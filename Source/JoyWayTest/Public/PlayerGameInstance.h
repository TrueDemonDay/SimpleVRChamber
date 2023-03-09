// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlayerGameInstance.generated.h"

/**
 * 
 */
class APlayerCharacter;
class APlayerInventory;
class UMenuWidget;
class UMapSelecWidget;

USTRUCT(BlueprintType)
struct FSavedItem
{
	GENERATED_BODY()

	//Default empty construct
	FSavedItem()
	: isValid(false), CanSave(false), SaveItemClass(nullptr), isActive(false), isWeapon(false), AmmoLeft(0)
	{}

	//All params
	FSavedItem(bool bisValid, bool bCanSave, UClass* uSaveItemClass, bool bisActive, bool bisWeapon, int AmmoCountLeft)
		: isValid(bisValid), CanSave(bCanSave), SaveItemClass(uSaveItemClass), isActive(bisActive), isWeapon(bisWeapon), AmmoLeft(AmmoCountLeft)
	{}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isValid = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanSave = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* SaveItemClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isActive = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isWeapon = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AmmoLeft = 0;
};

UCLASS()
class JOYWAYTEST_API UPlayerGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	//------------------Refereses---------------------------
	UPROPERTY()
	APlayerCharacter* PlayerRef;
	UPROPERTY()
	APlayerInventory* PlayerInventoryRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UMenuWidget* MenuWidgetRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UMapSelecWidget* MapSelectRef;

	FSavedItem LeftIteminInventory;
	FSavedItem RightItemInventory;
	FSavedItem ItemLeftHand;
	FSavedItem ItemRightHand;


	//------------------------------------------------------

	//---------------------Widgets logic--------------------

	void OpenLevelSelector();

	void CloseLevelSelector();

	void MapSelected(FName NameOfMap);

	void ResetPlayerPosition();

	void OpenMenu();

	void CloseMenu();

	void Exit();

	//-------------------------------------------------------

	UFUNCTION(BlueprintCallable)
	void StartOpenNewLevel(FName NameOfMap);

	void SaveItem(AActor* ItemActor, FSavedItem &SaveItemInfo);

	UFUNCTION()
	AActor* LoadItem(FSavedItem ItemInfo);
};
