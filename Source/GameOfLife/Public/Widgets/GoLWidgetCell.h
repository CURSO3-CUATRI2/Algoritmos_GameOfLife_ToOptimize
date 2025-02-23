// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GoLWidgetCell.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class GAMEOFLIFE_API UGoLWidgetCell : public UUserWidget
{
	GENERATED_BODY()

public:
	bool IsOn() const;
	void SetValue(bool bOn);
	void Toggle();

	void SetNewValue(bool bOn);
	void UpdateValueFromNewValue();
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	FEventReply OnPressImage(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> CellImage;

	bool bNewValue = false;
};
