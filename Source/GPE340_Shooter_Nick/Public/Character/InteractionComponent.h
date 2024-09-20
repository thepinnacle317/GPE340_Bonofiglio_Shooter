// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


class AItem_Base;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GPE340_SHOOTER_NICK_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FTimerHandle InteractionHandle;
	FHitResult ItemResults;
	TObjectPtr<AItem_Base> HitItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction | Trace Properties")
	float InteractionTraceDistance;

	/* Line Trace for items under the crosshairs */
	UFUNCTION()
	bool InteractionTrace(FHitResult& OutHitResult);
	void StartInteractionTimer();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction | Trace Properties")
	float TraceRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction | Trace Properties")
	float WidgetDisappearDistance;

protected:
	virtual void BeginPlay() override;
	
	

	
		
};
