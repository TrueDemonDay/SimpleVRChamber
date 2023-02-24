// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventory.h"
#include "PlayerCharacter.h"
#include "GrabItemBase.h"

// Sets default values
APlayerInventory::APlayerInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root scene"));
	RootComponent = SceneRootComponent;

	LeftItemBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftItemBoxHolder"));
	LeftItemBox->SetupAttachment(GetRootComponent());
	LeftItemBox->SetRelativeLocation(FVector(20,-30,-90));
	LeftItemBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerInventory::PreperAttachItemLeft);
	LeftItemBox->OnComponentEndOverlap.AddDynamic(this, &APlayerInventory::StopPreperAttachItemLeft);
	LeftItemBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftItemBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);

	RightItemBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightItemBoxHolder"));
	RightItemBox->SetupAttachment(GetRootComponent());
	RightItemBox->SetRelativeLocation(FVector(20, 30, -90));
	RightItemBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerInventory::PreperAttachItemRight);
	RightItemBox->OnComponentEndOverlap.AddDynamic(this, &APlayerInventory::StopPreperAttachItemRight);
	RightItemBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightItemBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);

	SpawnAmmoPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnAmmoPoint"));
	SpawnAmmoPoint->SetupAttachment(GetRootComponent());
	SpawnAmmoPoint->SetRelativeLocation(FVector(30,0,-60));

}

// Called when the game starts or when spawned
void APlayerInventory::BeginPlay()
{
	Super::BeginPlay();

	//Spawn ammo actor (In game not attached if don't FinishSpawning, but if will finish firs time not attached and then game frizze one time.
	ReloadItem = GetWorld()->SpawnActor<AActor>(ReloadItemClass);
	ReloadItem->FinishSpawning(SpawnAmmoPoint->GetComponentTransform());
	
}

// Called every frame
void APlayerInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerInventory::SetOwnerPlayer(APlayerCharacter * NewOwner)
{
	if (NewOwner)
	{
		OwnerPlayerRef = NewOwner;
		OwnerPlayerRef->GrabDone.AddDynamic(this, &APlayerInventory::DetachFromItemBox);
		OwnerPlayerRef->DropDone.AddDynamic(this, &APlayerInventory::AttachToItemBox);
		if (ReloadItem)
			ReloadItem->AttachToComponent(SpawnAmmoPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		GetWorld()->GetTimerManager().SetTimer(TimerForFollow, this, &APlayerInventory::FollowPlayerLogic, 0.0167f , true);
	}
}

void APlayerInventory::AttachToItemBox(AActor * AttachItem)
{
	if (AttachItem)
	{
		FTimerHandle Timer;
		if (AttachItem == LeftItem)
		{
			StoreLeftItem = true;
			if (LeftItem->GetClass()->ImplementsInterface(UItemsInterface::StaticClass()))
				IItemsInterface::Execute_GrabItem(LeftItem, nullptr);
			LeftItem->AttachToComponent(LeftItemBox, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			LeftItemBox->SetHiddenInGame(true);
		}
		if (AttachItem == RightItem)
		{
			StoreRightItem = true;
			if (RightItem->GetClass()->ImplementsInterface(UItemsInterface::StaticClass()))
				IItemsInterface::Execute_GrabItem(RightItem, nullptr);
			RightItem->AttachToComponent(RightItemBox, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			RightItemBox->SetHiddenInGame(true);
		}
	}
}

void APlayerInventory::DetachFromItemBox(AActor * DetachItem)
{
	if (LeftItem && LeftItem == DetachItem)
	{
		StoreLeftItem = false;
		LeftItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//LeftItem = nullptr;
		LeftItemBox->SetHiddenInGame(false);
	}
	if (RightItem && RightItem == DetachItem)
	{
		StoreRightItem = false;
		RightItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//RightItem = nullptr;
		RightItemBox->SetHiddenInGame(false);
	}
}

void APlayerInventory::FollowPlayerLogic()
{
	FVector NewLocation = OwnerPlayerRef->GetCameraLocation();
	FRotator NewRotation = FRotator(0, OwnerPlayerRef->GetCameraRotation().Yaw, 0);
	SetActorLocationAndRotation(NewLocation, NewRotation);
	
	SpawnAmmoLogic();
}

void APlayerInventory::SpawnAmmoLogic()
{
	if (ReloadItemClass && SpawnAmmoPoint)
	{
		if (ReloadItem != nullptr)
		{
			UPrimitiveComponent* RootMesh = Cast<UPrimitiveComponent>(ReloadItem->GetRootComponent());
			if (RootMesh)
			{
				if (RootMesh->IsSimulatingPhysics())
				{
					ReloadItem = GetWorld()->SpawnActor<AActor>(ReloadItemClass);
					ReloadItem->FinishSpawning(SpawnAmmoPoint->GetComponentTransform());
					ReloadItem->AttachToComponent(SpawnAmmoPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				}
			}
		}
		else
			ReloadItem = GetWorld()->SpawnActor<AActor>(ReloadItemClass, SpawnAmmoPoint->GetComponentTransform());
	}
}

void APlayerInventory::PreperAttachItemLeft(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	PreperAttachItem(OtherActor, LeftItem, LeftItemBox);
}

void APlayerInventory::StopPreperAttachItemLeft(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	StopPreperAttachItem(OtherActor, LeftItem, LeftItemBox, StoreLeftItem);
}

void APlayerInventory::PreperAttachItemRight(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	PreperAttachItem(OtherActor, RightItem, RightItemBox);
}

void APlayerInventory::StopPreperAttachItemRight(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	StopPreperAttachItem(OtherActor, RightItem, RightItemBox, StoreRightItem);
}

void APlayerInventory::PreperAttachItem(AActor * OtherActor, AActor* &SaveItemActor, UStaticMeshComponent* &ItemBox)
{
	AGrabItemBase* GrabItem = Cast<AGrabItemBase>(OtherActor);
	if (OtherActor && !SaveItemActor && GrabItem)
	{
		if ((OtherActor == OwnerPlayerRef->GetLeftItem() || OtherActor == OwnerPlayerRef->GetRightItem()) && GrabItem->ItemInfo.CanSave)
		{
			SaveItemActor = OtherActor;
			if (MatirealCanStoreItem)
				ItemBox->SetMaterial(0, MatirealCanStoreItem);
		}
	}
}

void APlayerInventory::StopPreperAttachItem(AActor * OtherActor, AActor *& SaveItemActor, UStaticMeshComponent *& ItemBox, bool StoreItem)
{
	if (SaveItemActor == OtherActor && !StoreItem)
	{
		SaveItemActor = nullptr;
		if (MatirealWaitItem)
			ItemBox->SetMaterial(0, MatirealWaitItem);
	}
}

void APlayerInventory::AttachLeftItem()
{
	if (LeftItem)
		LeftItem->AttachToComponent(LeftItemBox, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void APlayerInventory::AttachRightItem()
{
	if (RightItem)
		RightItem->AttachToComponent(LeftItemBox, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

