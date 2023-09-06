// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "TextImageBlock.h"

#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/Spacer.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "HAL/PlatformFileManager.h"

UTextImageBlock::UTextImageBlock(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	LettersImages.SetNum(255);
	Size = 24.0f;
	bImageDefault = false;
	bWrap = false;
	MinDesiredWidth = 0.0f;
}

void UTextImageBlock::NativePreConstruct()
{
	Super::NativePreConstruct();

	DefaultTextStyle.bWrap = bWrap;

	DefaultToImageText(bImageDefault);
	SetFText(Text);
	SetJustification(Justification);
	DefaultToImageText(bImageDefault);
	SetTextStyle(DefaultTextStyle);
	SetMinDesiredWidth(MinDesiredWidth);

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

void UTextImageBlock::SetText(const FString NewText)
{
	WrapBox->ClearChildren();

	if (!bImageDefault)
	{
		TextBlock->SetText(FText::FromString(NewText));
		return;
	}

	UHorizontalBox* Box = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
	WrapBox->AddChild(Box);

	for (int32 i = 0; i < NewText.Len(); i++)
	{
		if (LettersImages.IsValidIndex(NewText[i]))
		{
			UImage* Image = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
			SetTexture(Image, LettersImages[NewText[i]]);
			Box->AddChild(Image);

			if (NewText[i] == 32 && bWrap)
			{
				Box = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
				WrapBox->AddChild(Box);
			}
		}
	}
}

void UTextImageBlock::SetFText(const FText NewText)
{
	Text = NewText;
	SetText(NewText.ToString());
}

void UTextImageBlock::DefaultToImageText(bool bEnableImage)
{
	bImageDefault = bEnableImage;

	if (bImageDefault)
	{
		WrapBox->SetVisibility(ESlateVisibility::Visible);
		TextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		WrapBox->SetVisibility(ESlateVisibility::Collapsed);
		TextBlock->SetVisibility(ESlateVisibility::Visible);
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
	if(!Text.IsEmpty())
	{
		SetFText(Text);
		UE_LOG(LogTemp, Warning, TEXT("Updated Text Language %s"), *this->GetName());
	}
	OnUpdateText();
}
