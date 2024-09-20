// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item_Base.generated.h"

class UWidgetComponent;
class UBoxComponent;

UCLASS()
class GPE340_SHOOTER_NICK_API AItem_Base : public AActor
{
	GENERATED_BODY()
	
public:
	
	AItem_Base();
	virtual void Tick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;

private:

	/* Used for setting the Weapon Mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ItemMesh;
	
	/* Collision Box used to line trace against */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> CollisionBox;

	/* World Widget that displays when the player looks at the item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> PickupWidget;
};
