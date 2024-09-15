// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/WeaponComp.h"


UWeaponComp::UWeaponComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UWeaponComp::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void UWeaponComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}
