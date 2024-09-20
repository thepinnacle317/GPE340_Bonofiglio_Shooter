// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item_Base.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"


AItem_Base::AItem_Base()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Item Mesh"));
	SetRootComponent(ItemMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(ItemMesh);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Pickup Widget"));
	PickupWidget->SetupAttachment(GetRootComponent());

}

void AItem_Base::BeginPlay()
{
	Super::BeginPlay();

	/* Hide Pickup Widget At Start */
	PickupWidget->SetVisibility(false);	
}

void AItem_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

