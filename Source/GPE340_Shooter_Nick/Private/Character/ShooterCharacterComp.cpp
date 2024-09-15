// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ShooterCharacterComp.h"

// Sets default values for this component's properties
UShooterCharacterComp::UShooterCharacterComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UShooterCharacterComp::BeginPlay()
{
	Super::BeginPlay();
	
}

void UShooterCharacterComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

