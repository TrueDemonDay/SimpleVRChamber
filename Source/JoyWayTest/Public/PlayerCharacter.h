// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UMotionControllerComponent;
class APlayerInventory;
class USphereComponent;
class UPlayerDeathWidget;
class UNiagaraComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGrabDone, AActor*, GrabActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDropDone, AActor*, DropActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateNiagara);


UCLASS()
class JOYWAYTEST_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	// First person camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* PlayerCameraComponent;

	//Player controllers
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* PlayerLeftController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* PlayerRightController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CameraCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Path, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* TraceNiagara;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bIsDead = false;

	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	//-------------Input functions---------------------//
	//Grab and drop main functions
	void Grab(UMotionControllerComponent* ControllerComponent, AActor* &ActorToAttach);
	void Drop(AActor* &ActorToDrop);

	//GrabAndDrop input action
	void ActionGrabLeft();
	void ActionDropLeft();
	void ActionGrabRight();
	void ActionDropRight();

	//Use holdinh item functions
	void StartUseItem(AActor* Item);
	void StopUseItem(AActor* Item);
	//Use input action
	void ActionUseLeftItem();
	void ActionStopUseLeftItem();
	void ActionUseRightItem();
	void ActionStopUseRightItem();

	//Locomotion movement functions
	void MoveForward(float Value);
	void MoveRight(float Value);

	//TeleportFunction
	void ActionTeleport(float Value);

	void FindAndDrawTeleportLocation();
	void TryTeleport();

	//----------Respawn block------------
	FTransform RespawnTransform;

	UFUNCTION(BlueprintCallable)
	void SetRespawnTransform(FTransform NewTransform);
	void Respawn();

	//-----------------------------------

	//---------------input functions end-------------------//
	AActor* LeftGrabingItem = nullptr;
	AActor* RightGrabingItem = nullptr;

	//Teleport varrible
	bool bTryTelepor = false;
	bool bCanTeleport = false;
	FVector TeleportLockation;
	AActor* TeleportPlace = nullptr;
	APlayerInventory* PlayerInventoryRef = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = SpawnedActors)
	TSubclassOf<AActor> TeleportPlaceClass;
	UPROPERTY(EditDefaultsOnly, Category = SpawnedActors)
	TSubclassOf<APlayerInventory> PlayerInventoryClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
	UPlayerDeathWidget* DeathWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> Points;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetCameraLocation();
	FRotator GetCameraRotation();

	AActor* GetLeftItem() { return LeftGrabingItem; }
	AActor* GetRightItem() { return RightGrabingItem; }

	//Function for diligate
	UPROPERTY(BlueprintAssignable)
	FGrabDone GrabDone;
	UPROPERTY(BlueprintAssignable)
	FDropDone DropDone;
	UPROPERTY(BlueprintAssignable)
	FUpdateNiagara UpdateNiagara;

};
