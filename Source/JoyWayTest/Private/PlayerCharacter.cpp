// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MotionControllerComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"		//for sphere trace
#include "Kismet/GameplayStatics.h"			//for projectile path
#include "Interface/ItemsInterface.h"
#include "GrabItemBase.h"
#include "PlayerInventory.h"
#include "..\Public\PlayerCharacter.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	OnTakeAnyDamage.AddDynamic(this, &APlayerCharacter::TakeAnyDamage);

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(5.f, 5.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);

	// Create a CameraComponent	
	PlayerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TtsFirstPersonCamera"));
	PlayerCameraComponent->SetupAttachment(GetCapsuleComponent());
	PlayerCameraComponent->SetRelativeLocation(FVector(0, 0, -1*GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	//PlayerCameraComponent->bUsePawnControlRotation = true;

	CameraCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CameraScene"));
	CameraCollision->SetupAttachment(PlayerCameraComponent);
	CameraCollision->InitSphereRadius(30.f);
	CameraCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	CameraCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CameraCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block); //Set block projectiles

	// Create VR controllers
	//Left
	PlayerLeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftVRController"));
	PlayerLeftController->SetupAttachment(GetCapsuleComponent());
	PlayerLeftController->SetRelativeLocation(PlayerCameraComponent->GetRelativeLocation());
	PlayerLeftController->SetTrackingSource(EControllerHand::Left);
	PlayerLeftController->bDisplayDeviceModel = true;
	//Right
	PlayerRightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightVRController"));
	PlayerRightController->SetupAttachment(GetCapsuleComponent());
	PlayerRightController->SetRelativeLocation(PlayerCameraComponent->GetRelativeLocation());
	PlayerRightController->SetTrackingSource(EControllerHand::Right);
	PlayerRightController->bDisplayDeviceModel = true;
}

//------------- Called to bind functionality to input----------------------------//
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	//Grab and drop left
	PlayerInputComponent->BindAction("GrabLeft", IE_Pressed, this, &APlayerCharacter::ActionGrabLeft);
	PlayerInputComponent->BindAction("GrabLeft", IE_Released, this, &APlayerCharacter::ActionDropLeft);
	//Grab and drop right
	PlayerInputComponent->BindAction("GrabRight", IE_Pressed, this, &APlayerCharacter::ActionGrabRight);
	PlayerInputComponent->BindAction("GrabRight", IE_Released, this, &APlayerCharacter::ActionDropRight);
	//Use item left hand
	PlayerInputComponent->BindAction("TriggerLeft", IE_Pressed, this, &APlayerCharacter::ActionUseLeftItem);
	PlayerInputComponent->BindAction("TriggerLeft", IE_Released, this, &APlayerCharacter::ActionStopUseLeftItem);
	//Use item right hand
	PlayerInputComponent->BindAction("TriggerRight", IE_Pressed, this, &APlayerCharacter::ActionUseRightItem);
	PlayerInputComponent->BindAction("TriggerRight", IE_Released, this, &APlayerCharacter::ActionStopUseRightItem);
	//Bind movement events
	PlayerInputComponent->BindAxis("MovementAxisLeft_Y", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MovementAxisLeft_X", this, &APlayerCharacter::MoveRight);
	//Telepeport event
	PlayerInputComponent->BindAxis("MovementAxisRight_Y", this, &APlayerCharacter::ActionTeleport);
}
FVector APlayerCharacter::GetCameraLocation()
{
	if (PlayerCameraComponent)
	{
		FVector Location = PlayerCameraComponent->GetComponentLocation();
		return Location;
	}
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString("Error"));
	return FVector(0, 0, 0);
}
FRotator APlayerCharacter::GetCameraRotation()
{
	if (PlayerCameraComponent)
		return PlayerCameraComponent->GetComponentRotation();
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString("Error"));
	return FRotator(0,0,0);
}
//------------------------------------------------------------------------------------//

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (TeleportPlaceClass)
	{
		TeleportPlace = GetWorld()->SpawnActor(TeleportPlaceClass);
		TeleportPlace->SetHidden(true);
	}
	if (PlayerInventoryClass)
	{
		AActor* SpawnedActor = GetWorld()->SpawnActor(PlayerInventoryClass);
		PlayerInventoryRef = Cast<APlayerInventory>(SpawnedActor);
		PlayerInventoryRef->SetOwnerPlayer(this);
	}
	SetRespawnTransform(GetTransform());
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime) {Super::Tick(DeltaTime);} //Off it and don't use!

//Taka damage event.
void APlayerCharacter::TakeAnyDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString("!!!!!!!!!!OMFG U DAMAGED!!!!!!!!"));
	if (!bIsDead)
	{
		bIsDead = true;
		ActionDropLeft();
		ActionDropRight();
		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &APlayerCharacter::Respawn, 5);
	}
}

//---------------------------Grab logic--------------------------------//
void APlayerCharacter::Grab(UMotionControllerComponent * ControllerComponent, AActor* &ActorToAttach)
{
	FVector ControllerLocation = ControllerComponent->GetComponentLocation();
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Reserve(1);
	ObjectTypesArray.Emplace(ECollisionChannel::ECC_Visibility);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	FHitResult OutHit;
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), ControllerLocation, ControllerLocation, 10.f, ObjectTypesArray, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true))
	{
		if (OutHit.Actor !=nullptr)
		{
			if (OutHit.Actor->GetClass()->ImplementsInterface(UItemsInterface::StaticClass()))
			{
				GrabDone.Broadcast(OutHit.GetActor());
				IItemsInterface::Execute_GrabItem(OutHit.GetActor(), ControllerComponent);
				FAttachmentTransformRules Rule = Cast<AGrabItemBase>(OutHit.Actor)->GetRule();
				OutHit.Actor->AttachToComponent(ControllerComponent, Rule);
				ActorToAttach = OutHit.GetActor();
			}
		}
	}
}

//--------------------------Drop logic-----------------------------------------//
void APlayerCharacter::Drop(AActor* &ActorToDrop)
{
	if (ActorToDrop)
	{
		if (ActorToDrop->GetClass()->ImplementsInterface(UItemsInterface::StaticClass()))
			IItemsInterface::Execute_DropItem(ActorToDrop);
		ActorToDrop->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		DropDone.Broadcast(ActorToDrop);
		ActorToDrop = nullptr;
	}
}

//---------------------------Use items logic----------------------------------//
void APlayerCharacter::StartUseItem(AActor * Item)
{
	if (Item)
	{
		if (Item->GetClass()->ImplementsInterface(UItemsInterface::StaticClass()))
			IItemsInterface::Execute_StartUseItem(Item);
	}
}

void APlayerCharacter::StopUseItem(AActor * Item)
{
	if (Item)
	{
		if (Item->GetClass()->ImplementsInterface(UItemsInterface::StaticClass()))
			IItemsInterface::Execute_StopUseItem(Item);
	}
}

//---------------------------Input implementation-------------------------------//
void APlayerCharacter::ActionGrabLeft()
{
	if (!bIsDead)
		Grab(PlayerLeftController, LeftGrabingItem);
}

void APlayerCharacter::ActionDropLeft()
{
	ActionStopUseLeftItem();
	Drop(LeftGrabingItem);
}

void APlayerCharacter::ActionGrabRight()
{
	if (!bIsDead)
		Grab(PlayerRightController, RightGrabingItem);
}

void APlayerCharacter::ActionDropRight()
{
	ActionStopUseRightItem();
	Drop(RightGrabingItem);
}

void APlayerCharacter::ActionUseLeftItem()
{
	StartUseItem(LeftGrabingItem);
}

void APlayerCharacter::ActionStopUseLeftItem()
{
	StopUseItem(LeftGrabingItem);
}

void APlayerCharacter::ActionUseRightItem()
{
	StartUseItem(RightGrabingItem);
}

void APlayerCharacter::ActionStopUseRightItem()
{
	StopUseItem(RightGrabingItem);
}
//-----------------------------------------------------------------------//

//------------------Locomotion movement, use left hand!!!-----------------//
void APlayerCharacter::MoveForward(float Value)
{
	if (abs(Value) > 0.1f && !bIsDead)
	{
		const FRotator rot(0, PlayerLeftController->GetComponentRotation().Yaw, 0);
		FVector ControllerForwradVector = rot.RotateVector(FVector(1, 0, 0));
		AddMovementInput(ControllerForwradVector, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (abs(Value) > 0.1f && !bIsDead)
	{
		const FRotator rot(0, PlayerLeftController->GetComponentRotation().Yaw, 0);
		FVector ControllerRightVector = rot.RotateVector(FVector(0, 1, 0));
		AddMovementInput(ControllerRightVector, Value);
	}
}
//-----------------------------------------------------------------------//

//-----------------------------Teleport----------------------------------//

void APlayerCharacter::ActionTeleport(float Value)
{
	if (Value >= 0.5f && !bIsDead)
	{
		if (!bTryTelepor)
		{
			bTryTelepor = true;
		}

		//Set params for trace
		FPredictProjectilePathParams PredictParams;
		PredictParams.StartLocation = PlayerRightController->GetComponentLocation();
		PredictParams.LaunchVelocity = PlayerRightController->GetComponentRotation().RotateVector(FVector(600, 0, 0));
		PredictParams.ProjectileRadius = 10.f;
		PredictParams.bTraceWithCollision = true;
		PredictParams.TraceChannel = ECollisionChannel::ECC_GameTraceChannel1;
		//TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
		//ObjectTypesArray.Reserve(1);
		//ObjectTypesArray.Emplace(ECollisionChannel::ECC_GameTraceChannel1);
		//PredictParams.ObjectTypes = ObjectTypesArray;
		PredictParams.ActorsToIgnore.Add(this);
		FPredictProjectilePathResult PredictResult;
		bCanTeleport = UGameplayStatics::PredictProjectilePath(GetWorld(), PredictParams, PredictResult);
		TeleportLockation = PredictResult.HitResult.Location;
		if (TeleportPlace)
		{
			TeleportPlace->SetActorHiddenInGame(!bCanTeleport);
			TeleportPlace->SetActorLocation(TeleportLockation);
		}
	}
	else
	if (bTryTelepor && !bIsDead)
	{
		bTryTelepor = false;
		if (TeleportPlace)
			TeleportPlace->SetActorHiddenInGame(true);
		if (bCanTeleport)
		{
			TryTeleport();
			bCanTeleport = false;
		}
	}
	else
	{
		bTryTelepor = false;
		bCanTeleport = false;
	}
}

void APlayerCharacter::FindAndDrawTeleportLocation()
{

}

void APlayerCharacter::TryTeleport()
{
	TeleportTo(TeleportLockation, FRotator::ZeroRotator);
}

//-------------------Respawn-----------------------//

void APlayerCharacter::SetRespawnTransform(FTransform NewTransform)
{
	RespawnTransform = NewTransform;
}

void APlayerCharacter::Respawn()
{
	TeleportLockation = RespawnTransform.GetLocation();
	bIsDead = false;
	TryTeleport();
}

//-----------------------------------------------------------------------//

