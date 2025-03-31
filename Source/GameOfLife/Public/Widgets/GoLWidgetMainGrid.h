// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GoLWidgetMainGrid.generated.h"

class USpinBox;
class UTextBlock;
class UButton;
class USlider;
class UUniformGridPanel;
class UGoLWidgetCell;
class UEditableTextBox;

/**
 * Main grid 
 */
UCLASS()
class GAMEOFLIFE_API UGoLWidgetMainGrid : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void ExecutePass();
	void UpdateCellsState();

	int GetNumLiveNeighbours(const FIntVector2& CellPosition) const;
	UGoLWidgetCell* GetCellFromLineColumn(const FIntVector2& LineColumn) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGoLWidgetCell> WidgetCellClass;

	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<TObjectPtr<UGoLWidgetCell>> Cells;

	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> Grid;

	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USpinBox> GridSizeValue;
	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> CreateGridButton;

	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> ThresholdValue;
	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> FillRandomButton;

	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> SpeedSlider;

	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> PlayStopButton;
	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayStopText;

	UPROPERTY(Transient, BlueprintReadOnly)
	bool bIsPlaying = false;

	int32 Size = 0;
	float AccumulatedTime = 0.f;

	UFUNCTION()
	void OnPressCreateGrid();

	UFUNCTION()
	void OnPressFillRandomly();
	
	UFUNCTION()
	void OnPressPlayStop();
};
