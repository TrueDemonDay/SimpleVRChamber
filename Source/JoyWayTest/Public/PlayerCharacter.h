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
class UPlayerGameInstance;
class UWidgetInteractionComponent;
class UWidgetComponent;

struct FSavedItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGrabDone, AActor*, GrabActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDropDone, AActor*, DropActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateNiagara);


UCLASS()
class JOYWAYTEST_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// First person camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* PlayerCameraComponent;

	//Player controllers
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UMotionControllerComponent* PlayerLeftController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UMotionControllerComponent* PlayerRightController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USphereComponent* CameraCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Path)
	UNiagaraComponent* TraceNiagara;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widgets)
	UWidgetInteractionComponent* WidgetInterator;

	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widgets)
	UWidgetComponent* MenuWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widgets)
	UWidgetComponent* MapSelector;

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

	//-----------------------------------

	//---------------input functions end-------------------//
	UPROPERTY()
	AActor* LeftGrabingItem = nullptr;
	UPROPERTY()
	AActor* RightGrabingItem = nullptr;

	//Teleport varrible
	bool bTryTelepor = false;
	bool bCanTeleport = false;
	FVector TeleportLockation;
	AActor* TeleportPlace = nullptr;
	APlayerInventory* PlayerInventoryRef = nullptr;

	//BP references
	UPROPERTY(EditDefaultsOnly, Category = SpawnedActors)
	TSubclassOf<AActor> TeleportPlaceClass;
	UPROPERTY(EditDefaultsOnly, Category = SpawnedActors)
	TSubclassOf<APlayerInventory> PlayerInventoryClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
	UPlayerDeathWidget* DeathWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> Points;

	UPROPERTY()
	UPlayerGameInstance* GameInstRef = nullptr;

	void AttachLoadItem(AActor* NewItem, UMotionControllerComponent* ControllerToAttach);

	bool MenuOpened = false;
	void OpenCloseMenu();

	UPROPERTY(EditDefaultsOnly, Category = Input)
	FKey InteractKey;

	FTimerHandle RespawnTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetCameraLocation();
	FRotator GetCameraRotation();

	inline AActor* GetLeftItem() { return LeftGrabingItem; }
	inline AActor* GetRightItem() { return RightGrabingItem; }

	//Function for diligate
	UPROPERTY(BlueprintAssignable)
	FGrabDone GrabDone;
	UPROPERTY(BlueprintAssignable)
	FDropDone DropDone;
	UPROPERTY(BlueprintAssignable)
	FUpdateNiagara UpdateNiagara;

	void Respawn();

};
