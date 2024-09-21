// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item_Base.generated.h"

/* Forward Declarations */
class UWidgetComponent;
class UBoxComponent;

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Uncommon UMETA(DisplayName = "Uncommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Epic UMETA(DisplayName = "Epic"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),

	EIR_Max UMETA(DisplayName = "Default Max")
};

UCLASS()
class GPE340_SHOOTER_NICK_API AItem_Base : public AActor
{
	GENERATED_BODY()
	
public:
	
	AItem_Base();
	virtual void Tick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;

	/* Used to visually set the value of the item rarity in the pickup widget */
	void SetItemRarity();

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

	/* Variable Used to assign the name of the item and show in the pickup widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	FString ItemName;

	/* Represents the amount of ammo on a weapon or amount dropped */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 ItemAmount;

	/* Item Rarity is used to show a visual representation throughout the game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemRarity ItemRarity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	TArray<bool> ActiveRarity;

public:
	FORCEINLINE TObjectPtr<UWidgetComponent> GetPickupWidget() const { return PickupWidget; }
	FORCEINLINE TObjectPtr<UBoxComponent> GetCollisionBox() const { return CollisionBox; }
};
