// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item_Base.h"

#include "Components/BoxComponent.h"

// Sets default values
AItem_Base::AItem_Base()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Item Mesh"));
	SetRootComponent(ItemMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(ItemMesh);

}

void AItem_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItem_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

