// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGameInstance.h"
#include "PlayerInventory.h"
#include "PlayerCharacter.h"
#include "GrabItemBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ShootingItem.h"
#include "Widgets/MapSelecWidget.h"
#include "Widgets/MenuWidget.h"
#include "Components/WidgetComponent.h"

void UPlayerGameInstance::OpenLevelSelector()
{
	if (MenuWidgetRef->IsValidLowLevel())
		MenuWidgetRef->SetVisibility(ESlateVisibility::Hidden);
	if (MapSelectRef->IsValidLowLevel())
		MapSelectRef->SetVisibility(ESlateVisibility::Visible);
	if (PlayerRef->IsValidLowLevel())
	{
		PlayerRef->MenuWidget->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
		PlayerRef->MapSelector->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	}
}

void UPlayerGameInstance::CloseLevelSelector()
{
	if (MapSelectRef->IsValidLowLevel())
		MapSelectRef->SetVisibility(ESlateVisibility::Hidden);
	if (MenuWidgetRef->IsValidLowLevel())
		MenuWidgetRef->SetVisibility(ESlateVisibility::Visible);
	if (PlayerRef->IsValidLowLevel())
	{
		PlayerRef->MenuWidget->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		PlayerRef->MapSelector->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	}
}

void UPlayerGameInstance::MapSelected(FName NameOfMap)
{
	CloseMenu();
	StartOpenNewLevel(NameOfMap);
}

void UPlayerGameInstance::ResetPlayerPosition()
{
	PlayerRef->Respawn();
}

void UPlayerGameInstance::OpenMenu()
{
	if (MenuWidgetRef)
	{
		MenuWidgetRef->SetVisibility(ESlateVisibility::Visible);
	}
	if (PlayerRef)
	{
		PlayerRef->MenuWidget->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	}
}

void UPlayerGameInstance::CloseMenu()
{
	if (MapSelectRef->IsValidLowLevel())
	{
		MapSelectRef->SetVisibility(ESlateVisibility::Hidden);
	}
	if (MenuWidgetRef->IsValidLowLevel())
	{
		MenuWidgetRef->SetVisibility(ESlateVisibility::Hidden);
	}
	if (PlayerRef->IsValidLowLevel())
	{
		PlayerRef->MenuWidget->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
		PlayerRef->MapSelector->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	}
}

void UPlayerGameInstance::Exit()
{
	//Close game or crash it, nvrmd :P
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}

void UPlayerGameInstance::StartOpenNewLevel(FName NameOfMap)
{

	//SavePlayerItems
	if (PlayerInventoryRef->IsValidLowLevel())
	{
		SaveItem(PlayerInventoryRef->LeftItem, LeftIteminInventory);
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString("Save Inventory Left"));
		SaveItem(PlayerInventoryRef->RightItem, RightItemInventory);
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString("Save Inventory Right"));
		GetWorld()->GetTimerManager().PauseTimer(PlayerInventoryRef->TimerForFollow);
	}
	if (PlayerRef->IsValidLowLevel())
	{
		SaveItem(PlayerRef->GetLeftItem(), ItemLeftHand);
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString("Save Hand Left"));
		SaveItem(PlayerRef->GetRightItem(), ItemRightHand);
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString("Save Hand Right"));
	}

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString("Start open level"));
	UGameplayStatics::OpenLevel(GetWorld(), NameOfMap);
}

void UPlayerGameInstance::SaveItem(AActor *ItemActor, FSavedItem & SaveItemInfo)
{
	if (ItemActor->IsValidLowLevel())
	{
		AGrabItemBase* Item = Cast<AGrabItemBase>(ItemActor);
		if (Item)
		{
			SaveItemInfo = Item->ItemInfo;
			Item->Drop();
			return;
		}
	}
	SaveItemInfo.isValid = false;
}

AActor* UPlayerGameInstance::LoadItem(FSavedItem ItemInfo)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString("Load item"));
	if (ItemInfo.isValid && ItemInfo.CanSave)
	{
		AActor* NewItem = GetWorld()->SpawnActor<AActor>(ItemInfo.SaveItemClass);
		NewItem->FinishSpawning(FTransform(FRotator::ZeroRotator, FVector::ZeroVector));
		if (NewItem)
		{
			if (ItemInfo.isWeapon)
			{
				AShootingItem* Weapon = Cast<AShootingItem>(NewItem);
				if (Weapon)
				{
					Weapon->SetAmmoLeft(ItemInfo.AmmoLeft);
				}
			}

			if (ItemInfo.isActive)
			{
				if (NewItem->GetClass()->ImplementsInterface(UItemsInterface::StaticClass()))
				{
					IItemsInterface::Execute_StartUseItem(NewItem);
				}
			}
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString("Load sucsess"));
			return NewItem;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Purple, FString("Load fail: Item not spawn"));
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Purple, FString("Load fail: Item not valid"));
	return nullptr;
}
