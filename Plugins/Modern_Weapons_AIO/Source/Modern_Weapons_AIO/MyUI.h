// Copyright SOUNDFX STUDIO © 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Animation/WidgetAnimation.h"
#include "MyUI.generated.h"

UCLASS()
class MODERN_WEAPONS_AIO_API UMyUI : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "MyUI Functions")
		void UI_Init();
	UFUNCTION(BlueprintCallable, Category = "MyUI Functions")
		void UI_SwitchWeapon(int wpnNumber);
	UFUNCTION(BlueprintCallable, Category = "MyUI Functions")
		void UI_Fire(bool bOn);
	UFUNCTION(BlueprintCallable, Category = "MyUI Functions")
		void UI_FireMode(bool bAuto);
	UFUNCTION(BlueprintCallable, Category = "MyUI Functions")
		void UI_Suppressor(bool bOn);
	UFUNCTION(BlueprintCallable, Category = "MyUI Functions")
		void UI_FireMode_Anim();
	UFUNCTION(BlueprintCallable, Category = "MyUI Functions")
		void UI_Suppressor_Anim();

	// Error Messages on the screen
	UFUNCTION(BlueprintCallable, Category = "MyUI Functions")
		void ErrorMessages(int32 n);

protected:

	// ref to Animation for UI Error Text
	//UPROPERTY(BlueprintReadWrite, Category = "MyUI Properties")
		//UWidgetAnimation* animTxtErrors;

private:

	UPROPERTY(meta = (BindWidget))
		UTextBlock* txt_errors;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* anim_TxtErrors;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* anim_Fire_Pressed;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* anim_Firemode_Pressed;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* anim_Suppressor_Pressed;

	UPROPERTY(meta = (BindWidget))
		TArray<UBorder*> weap;

	UPROPERTY(meta = (BindWidget))
		UBorder* weap_1;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_2;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_3;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_4;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_5;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_6;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_7;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_8;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_9;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_10;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_11;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_12;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_13;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_14;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_15;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_16;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_17;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_18;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_19;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_20;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_21;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_22;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_23;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_24;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_25;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_26;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_27;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_28;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_29;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_30;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_31;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_32;
	UPROPERTY(meta = (BindWidget))
		UBorder* weap_33;


	UPROPERTY(meta = (BindWidget))
		UBorder* action_1;
	UPROPERTY(meta = (BindWidget))
		UBorder* action_2;
	UPROPERTY(meta = (BindWidget))
		UBorder* action_3;


	// Switch UI Buttons On/Off
	void SetWpnOnOff(UBorder* wpn, bool on = true);

	// Colors for button's borders
	FLinearColor txt_on_color = FLinearColor(1.f, 1.f, 1.f, 1.f);
	FLinearColor txt_off_color = FLinearColor(1.f, 1.f, 1.f, 0.4f);
	FLinearColor brush_off_color = FLinearColor(1.f, 1.f, 1.f, 0.f);

	TArray<UBorder*> weapons;

	// Is Cockpit Snd On
	bool bIsCockpitSnd = false;
};