// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerInventory.generated.h"

class APlayerCharacter;

UCLASS()
class JOYWAYTEST_API APlayerInventory : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory)
	USceneComponent* SceneRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory)
	UStaticMeshComponent* LeftItemBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory)
	UStaticMeshComponent* RightItemBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory)
	USceneComponent* SpawnAmmoPoint;

	// Sets default values for this actor's properties
	APlayerInventory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Material)
	UMaterialInstance* MatirealWaitItem = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Material)
	UMaterialInstance* MatirealCanStoreItem = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Follow timer
	FTimerHandle TimerForFollow;

	//Ref for holding items
	UPROPERTY()
	AActor* LeftItem = nullptr;
	UPROPERTY()
	AActor* RightItem = nullptr;
	UPROPERTY()
	APlayerCharacter* OwnerPlayerRef = nullptr;

	//Vars for ammo spawn

	UPROPERTY(EditDefaultsOnly, Category = SpawnedActors)
	TSubclassOf<AActor> ReloadItemClass;
	UPROPERTY()
	AActor* ReloadItem = nullptr;

	bool StoreLeftItem = false;
	bool StoreRightItem = false;

	//Init function, starts follow player
	void SetOwnerPlayer(APlayerCharacter* NewOwner);

	void ChangeMaterial(UStaticMeshComponent* Mesh);

	UFUNCTION()
	void AttachToItemBox(AActor* AttachItem);
	UFUNCTION()
	void DetachFromItemBox(AActor* DetachItem);

	void StartFollowPlayerCamera();

	void FollowPlayerLogic();

	void SpawnAmmoLogic();

	UFUNCTION()
	void PreperAttachItemLeft(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void StopPreperAttachItemLeft(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void PreperAttachItemRight(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void StopPreperAttachItemRight(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void PreperAttachItem(AActor* OtherActor, AActor* &SaveItemActor, UStaticMeshComponent* &ItemBox);

	void StopPreperAttachItem(AActor* OtherActor, AActor* &SaveItemActor, UStaticMeshComponent* &ItemBox, bool StoreItem);

	void AttachLeftItem();
	void AttachRightItem();
};
