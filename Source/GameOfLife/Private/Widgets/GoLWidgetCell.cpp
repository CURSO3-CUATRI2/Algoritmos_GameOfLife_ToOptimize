// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOfLife/Public/Widgets/GoLWidgetCell.h"

#include "Components/Image.h"
#include "Engine/RendererSettings.h"

void UGoLWidgetCell::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!IsValid(CellImage))
	{
		return;
	}

	CellImage->OnMouseButtonDownEvent.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UGoLWidgetCell, OnPressImage));
}

FEventReply UGoLWidgetCell::OnPressImage(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	Toggle();

	return FEventReply(true);
}

bool UGoLWidgetCell::IsOn() const
{
	if (!IsValid(CellImage))
	{
		return false;
	}

	return CellImage->GetColorAndOpacity().R > 0.5f;
}

void UGoLWidgetCell::SetValue(bool bOn)
{
	if (!IsValid(CellImage))
	{
		return;
	}
	
	CellImage->SetColorAndOpacity(bOn ? FLinearColor(FColor::White) : FLinearColor(FColor::Black));
}

void UGoLWidgetCell::SetNewValue(bool bOn)
{
	bNewValue = bOn;
}

void UGoLWidgetCell::UpdateValueFromNewValue()
{
	SetValue(bNewValue);
}

void UGoLWidgetCell::Toggle()
{
	SetValue(!IsOn());
}


