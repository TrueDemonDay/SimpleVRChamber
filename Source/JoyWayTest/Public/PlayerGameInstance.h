// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlayerGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FSavedItem
{
	GENERATED_BODY()

		//~ The following member variable will be accessible by Blueprint Graphs:
		// This is the tooltip for our test variable.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isValid = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanSave = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* SaveItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isActive = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isWeapon = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AmmoLeft;
};

UCLASS()
class JOYWAYTEST_API UPlayerGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	//------------------Refereses---------------------------
	class APlayerCharacter* PlayerRef;
	class APlayerInventory* PlayerInventoryRef;
	class UMenuWidget* MenuWidgetRef;
	class UMapSelecWidget* MapSelectRef;

	FSavedItem LeftIteminInventory;
	FSavedItem RightItemInventory;
	FSavedItem ItemInLeftHand;
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
};
