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

	bIsPlaying = false;
	if (IsValid(PlayStopText))
	{
		PlayStopText->SetText(FText::FromString(TEXT("Play")))	;
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

	AccumulatedTime += InDeltaTime;
	if (AccumulatedTime < 1.f / Speed)
	{
		return;
	}
	AccumulatedTime = 0.f;

	ExecutePass();
	UpdateCellsState();
}

void UGoLWidgetMainGrid::ExecutePass()
{
	// Una celula viva:
	//	- Pasará a la siguiente generación si tiene entre 2 y 3 vecinos vivos
	// Una celula muerta:
	//	- Revivirá en la siguiente generación si tiene 3 vecinos vivos

	for (int X = 0; X < Size; X++)
	{
		for (int Y = 0; Y < Size; Y++)
		{
			const FIntVector2 CellPosition(X, Y);
			UGoLWidgetCell* Cell = GetCellFromLineColumn(CellPosition);
			const bool bIsAlive = Cell->IsOn();
			const int NumLiveNeighbours = GetNumLiveNeighbours(CellPosition);

			const bool bNewValue = (bIsAlive && (NumLiveNeighbours == 2 || NumLiveNeighbours == 3)) ||
								   (!bIsAlive && NumLiveNeighbours == 3);
			Cell->SetNewValue(bNewValue);
		}
	}	
}

int UGoLWidgetMainGrid::GetNumLiveNeighbours(const FIntVector2& CellPosition) const
{
	int NumLiveNeighbours = 0;
	for (int X = -1; X <= 1; ++X)
	{
		for (int Y = -1; Y <= 1; ++Y)
		{
			if (X == 0 && Y == 0)
			{
				// Somos nosotros, no hace nada
				continue;
			}

			FIntVector2 NeighbourCellPosition = CellPosition + FIntVector2(X, Y);
			// Límites
#if 1
			if (NeighbourCellPosition.X < 0)
			{
				NeighbourCellPosition.X = Size - 1;
			}
			else if (NeighbourCellPosition.X >= Size)
			{
				NeighbourCellPosition.X = 0;
			}

			if (NeighbourCellPosition.Y < 0)
			{
				NeighbourCellPosition.Y = Size - 1;
			}
			else if (NeighbourCellPosition.Y >= Size)
			{
				NeighbourCellPosition.Y = 0;
			}
#else
			if (NeighbourCellPosition.X < 0 || NeighbourCellPosition.X >= Size ||
				NeighbourCellPosition.Y < 0 || NeighbourCellPosition.Y >= Size)
			{
				// Fuera de límites
				continue;
			}
#endif

			NumLiveNeighbours += GetCellFromLineColumn(NeighbourCellPosition)->IsOn() ? 1 : 0;
		}		
	}

	return NumLiveNeighbours;
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

UGoLWidgetCell* UGoLWidgetMainGrid::GetCellFromLineColumn(const FIntVector2& LineColumn) const
{
	int32 Index = LineColumn.X + LineColumn.Y * Size;

	return Cells.IsValidIndex(Index) ? Cells[Index] : nullptr;
}