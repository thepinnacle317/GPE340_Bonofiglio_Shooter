// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item_Base.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"


AItem_Base::AItem_Base():
ItemName(FString("Default Weapon")),
ItemAmount(0),
ItemRarity(EItemRarity::EIR_Common)
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
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}

	/* Visual Set the value of the item rarity */
	SetItemRarity();
}

void AItem_Base::SetItemRarity()
{
	/* Does not use zero so that the index aligns with the visual rarity value */
	for (int32 i = 0; i <= 5; i++)
	{
		ActiveRarity.Add(false);
	}

	switch (ItemRarity)
	{
	case EItemRarity::EIR_Common:
		ActiveRarity[1] = true;
		break;
	case EItemRarity::EIR_Uncommon:
		ActiveRarity[1] = true;
		ActiveRarity[2] = true;
		break;
	case EItemRarity::EIR_Rare:
		ActiveRarity[1] = true;
		ActiveRarity[2] = true;
		ActiveRarity[3] = true;
		break;
	case EItemRarity::EIR_Epic:
		ActiveRarity[1] = true;
		ActiveRarity[2] = true;
		ActiveRarity[3] = true;
		ActiveRarity[4] = true;
		break;
	case EItemRarity::EIR_Legendary:
		ActiveRarity[1] = true;
		ActiveRarity[2] = true;
		ActiveRarity[3] = true;
		ActiveRarity[4] = true;
		ActiveRarity[5] = true;
		break;
	}
}

void AItem_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

