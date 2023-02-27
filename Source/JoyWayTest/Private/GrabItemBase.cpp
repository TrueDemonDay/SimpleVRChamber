// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabItemBase.h"

// Sets default values
AGrabItemBase::AGrabItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}

// Called when the game starts or when spawned
void AGrabItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrabItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrabItemBase::GrabItem_Implementation(UMotionControllerComponent * ControllerToAttach)
{
	Grab(ControllerToAttach);
}

void AGrabItemBase::Grab(UMotionControllerComponent * ControllerToAttach)
{
	UPrimitiveComponent* RootMesh = Cast<UPrimitiveComponent>(GetRootComponent());
	if (RootMesh)
	{
		RootMesh->SetSimulatePhysics(false);
	}
}

void AGrabItemBase::DropItem_Implementation()
{
	Drop();
}

void AGrabItemBase::Drop()
{
	UPrimitiveComponent* RootMesh = Cast<UPrimitiveComponent>(GetRootComponent());
	if (RootMesh)
	{
		RootMesh->SetSimulatePhysics(true);
	}
}

void AGrabItemBase::StartUseItem_Implementation()
{
}

void AGrabItemBase::StopUseItem_Implementation()
{
}

FAttachmentTransformRules AGrabItemBase::GetRule()
{
	if (AttachRule == EAttachmentRule::KeepRelative)
		return FAttachmentTransformRules::KeepRelativeTransform;
	if (AttachRule == EAttachmentRule::KeepWorld)
		return FAttachmentTransformRules::KeepWorldTransform;
	return FAttachmentTransformRules::SnapToTargetNotIncludingScale;
}

