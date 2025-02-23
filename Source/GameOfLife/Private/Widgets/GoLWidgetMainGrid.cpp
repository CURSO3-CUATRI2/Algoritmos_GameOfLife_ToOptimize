// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GoLWidgetMainGrid.h"

#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/SpinBox.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Widgets/GoLWidgetCell.h"


void UGoLWidgetMainGrid::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(CreateGridButton))
	{
		CreateGridButton->OnClicked.AddDynamic(this, &UGoLWidgetMainGrid::OnPressCreateGrid);
	}

	if (IsValid(FillRandomButton))
	{
		FillRandomButton->OnClicked.AddDynamic(this, &UGoLWidgetMainGrid::OnPressFillRandomly);
	}

	if (IsValid(PlayStopButton))
	{
		PlayStopButton->OnClicked.AddDynamic(this, &UGoLWidgetMainGrid::OnPressPlayStop);
	}
}

void UGoLWidgetMainGrid::OnPressCreateGrid()
{
	if (!IsValid(WidgetCellClass))
	{
		return;
	}

	// Clear old cells
	for (UGoLWidgetCell* Cell : Cells)
	{
		if (IsValid(Cell))
		{
			Cell->RemoveFromParent();
		}
	}
	Cells.Empty();

	// New size
	Size = GridSizeValue->GetValue();
	int32 NumCells = Size * Size;
	Cells.Reserve(NumCells);
	for (int32 Line = 0; Line < Size; Line++)
	{
		for (int32 Column = 0; Column < Size; Column++)
		{
			UGoLWidgetCell* NewCell = CreateWidget<UGoLWidgetCell>(this, WidgetCellClass);
			UUniformGridSlot* GridSlot = Grid->AddChildToUniformGrid(NewCell, Column, Line);
			GridSlot->SetHorizontalAlignment(HAlign_Fill);
			GridSlot->SetVerticalAlignment(VAlign_Fill);
			NewCell->SetValue(false);
			Cells.Add(NewCell);
		}
	}
}

void UGoLWidgetMainGrid::OnPressFillRandomly()
{
	if (Cells.IsEmpty() || !IsValid(ThresholdValue))
	{
		return;
	}

	FRandomStream RandomStream(FPlatformTime::Cycles());
	float Threshold = ThresholdValue->GetValue();
	for (UGoLWidgetCell* Cell : Cells)
	{
		if (IsValid(Cell))
		{
			Cell->SetValue(RandomStream.GetFraction() < Threshold);
		}
	}	
}

void UGoLWidgetMainGrid::OnPressPlayStop()
{
	bIsPlaying = !bIsPlaying;
	if (IsValid(PlayStopText))
	{
		PlayStopText->SetText(bIsPlaying ? FText::FromString(TEXT("Pause")) : FText::FromString(TEXT("Play")));
	}
}

void UGoLWidgetMainGrid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bIsPlaying || Cells.IsEmpty())
	{
		return;
	}

	float Speed = SpeedSlider->GetValue();
	if (Speed <= KINDA_SMALL_NUMBER)
	{
		return;
	}

	AccumulatedTime = InDeltaTime;
	if (AccumulatedTime < 1.f / Speed)
	{
		return;
	}

	ExecutePass();
	UpdateCellsState();
}

void UGoLWidgetMainGrid::ExecutePass()
{
	// TODO
}

void UGoLWidgetMainGrid::UpdateCellsState()
{
	for (UGoLWidgetCell* Cell : Cells)
	{
		if (IsValid(Cell))
		{
			Cell->UpdateValueFromNewValue();
		}
	}
}

UGoLWidgetCell* UGoLWidgetMainGrid::GetCellFromLineColumn(const FIntVector2& LineColumn)
{
	// TODO
	int32 Index = 0;

	return Cells.IsValidIndex(Index) ? Cells[Index] : nullptr;
}