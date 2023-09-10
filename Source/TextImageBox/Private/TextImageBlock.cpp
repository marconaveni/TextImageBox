// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "TextImageBlock.h"

#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/Spacer.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/RetainerBox.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "HAL/PlatformFileManager.h"

UTextImageBlock::UTextImageBlock(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	LettersImages.SetNum(255);
	Size = 24.0f;
	MinDesiredWidth = 0.0f;
	bImageDefault = false;
	bWrap = false;
	bRetainerImageText = false;
}

void UTextImageBlock::NativePreConstruct()
{
	Super::NativePreConstruct();

	DefaultTextStyle.bWrap = bWrap;
	RetainerBox->SetRetainRendering(bRetainerImageText);

	DefaultToImageText(bImageDefault);
	SetJustification(Justification);
	SetTextStyle(DefaultTextStyle);
	SetMinDesiredWidth(MinDesiredWidth);
	SetText(CurrentText);

}

bool UTextImageBlock::SetFontFrom(FString Path, FSlateFontInfo& FontInfo, int32 FontSize)
{
	// File Exists?
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*Path))
	{
		const FSlateFontInfo NewFontInfo(Path, FMath::Clamp(FontSize, 2, 100));
		FontInfo = NewFontInfo;
		return true;
	}
	return false;
}

void UTextImageBlock::SetJustification(ETextJustify::Type Justify)
{
	switch (Justify)
	{
	case ETextJustify::Center:
		WrapBox->SetHorizontalAlignment(HAlign_Center);
		break;
	case ETextJustify::Left:
		WrapBox->SetHorizontalAlignment(HAlign_Left);
		break;
	case ETextJustify::Right:
		WrapBox->SetHorizontalAlignment(HAlign_Right);
		break;
	default:
		break;
	}
	TextBlock->SetJustification(Justify);
}

void UTextImageBlock::SetText(const FText NewText)
{
	CurrentText = NewText;

	if (!bImageDefault)
	{
		TextBlock->SetText(CurrentText);
		return OnTextChange();
	}

	WrapBox->ClearChildren();
	const FString StringText = CurrentText.ToString();

	UHorizontalBox* Box = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
	WrapBox->AddChild(Box);

	for (int32 i = 0; i < StringText.Len(); i++)
	{
		if (LettersImages.IsValidIndex(StringText[i]))
		{
			UImage* Image = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
			SetTexture(Image, LettersImages[StringText[i]]);
			Box->AddChild(Image);

			if (StringText[i] == 32 && bWrap)
			{
				Box = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
				WrapBox->AddChild(Box);
			}
		}
	}

	if (RetainerBox)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTextImageBlock::UpdateRender, GetWorld()->GetDeltaSeconds(), false, GetWorld()->GetDeltaSeconds());
	}

	return OnTextChange();
}

void UTextImageBlock::SetFText(const FText NewText)
{
	SetText(NewText);
}

void UTextImageBlock::DefaultToImageText(bool bEnableImage, bool bUpdate)
{

	bImageDefault = bEnableImage;

	if (bImageDefault)
	{
		RetainerBox->SetVisibility(ESlateVisibility::Visible);
		TextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		RetainerBox->SetVisibility(ESlateVisibility::Collapsed);
		TextBlock->SetVisibility(ESlateVisibility::Visible);
	}

	if(bUpdate)
	{
		UpdateText();
	}
}

void UTextImageBlock::SetTextStyle(FTextStyle TextStyle)
{
	TextBlock->SetFont(TextStyle.Font);
	TextBlock->SetColorAndOpacity(TextStyle.Color);
	TextBlock->SetAutoWrapText(TextStyle.bWrap);
}

void UTextImageBlock::SetTextImageSize(float NewSize)
{
	Size = NewSize;
	UpdateText();
}

void UTextImageBlock::SetMinDesiredWidth(float InMinDesiredWidth)
{
	UCanvasPanelSlot* SpacerSlot = Cast<UCanvasPanelSlot>(Spacer->Slot);
	if (SpacerSlot != nullptr)
	{
		SpacerSlot->SetSize(FVector2D(InMinDesiredWidth, 1.0f));
	}
}

void UTextImageBlock::UpdateRender()
{
	RetainerBox->RequestRender();
}

void UTextImageBlock::SetTexture(UImage* Image, UTexture2D* NewTexture)
{
	if (NewTexture == nullptr) return;

	const float NewSize = (Size / 10) + 1;

	FSlateBrush Brush;
	Brush.SetImageSize(FVector2D(NewTexture->GetSizeX() * NewSize, NewTexture->GetSizeY() * NewSize));
	Brush.SetResourceObject(NewTexture);

	Image->SetBrush(Brush);
}

void UTextImageBlock::UpdateText()
{
	SetText(CurrentText);
	OnUpdateText();
}
