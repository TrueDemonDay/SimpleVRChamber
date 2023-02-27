// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ItemsInterface.h"
#include "PlayerGameInstance.h"
#include "GrabItemBase.generated.h"

UCLASS()
class JOYWAYTEST_API AGrabItemBase : public AActor, public IItemsInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrabItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EAttachmentRule AttachRule = EAttachmentRule::KeepRelative;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FSavedItem ItemInfo;

	FAttachmentTransformRules GetRule();

	//---------Interface implementation--------------//
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GrabItem(UMotionControllerComponent* ControllerToAttach);
	virtual void GrabItem_Implementation(UMotionControllerComponent* ControllerToAttach) override;
	void Grab(UMotionControllerComponent* ControllerToAttach);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DropItem();
	virtual void DropItem_Implementation() override;
	void Drop();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartUseItem();
	virtual void StartUseItem_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopUseItem();
	virtual void StopUseItem_Implementation();
};
