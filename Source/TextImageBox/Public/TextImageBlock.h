// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextImageBlock.generated.h"

USTRUCT(BlueprintType)
struct FTextStyle
{
	GENERATED_BODY()

	/** The default font to render */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateFontInfo Font;
	/** The default font color */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor Color;
	/** Wrap text line*/
	UPROPERTY(BlueprintReadWrite)
	bool bWrap;

	FTextStyle()
	{
		Color = FSlateColor(FColor::White);
		bWrap = false;
	}
};

class UCanvasPanel;
class UHorizontalBox;
class USpacer;
class UTextBlock;
class UWrapBox;
class UImage;
class URetainerBox;

/**
 * A Text Block Enchanted 
 */
UCLASS()
class TEXTIMAGEBOX_API UTextImageBlock : public UUserWidget
{
	GENERATED_BODY()

protected:

	UTextImageBlock(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "TextImageBlock|Components")
	UCanvasPanel* CanvasPanel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "TextImageBlock|Components")
	UHorizontalBox* HorizontalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "TextImageBlock|Components")
	USpacer* Spacer;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "TextImageBlock|Components")
	UTextBlock* TextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "TextImageBlock|Components")
	UWrapBox* WrapBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "TextImageBlock|Components")
	URetainerBox* RetainerBox;
	UPROPERTY()
	FTimerHandle TimerHandle;

public:

	/** alignment text  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextImageBlock|Default Values")
	TEnumAsByte<ETextJustify::Type>Justification;
	/** The text to display */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextImageBlock|Default Values", DisplayName = "Text")
	FText CurrentText;
	/** The image text to display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TextImageBlock|Default Values")
	bool bImageDefault;
	/** Wrap text line*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TextImageBlock|Default Values")
	bool bWrap;
	/** enable Retainer box*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextImageBlock|Default Values")
	bool bRetainerImageText;
	/** The minimum desired size for the text */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextImageBlock|Default Values")
	float MinDesiredWidth;
	/** Array Textures letters */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextImageBlock|Text")
	TArray<UTexture2D*> LettersImages;
	/** The font size image to render */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextImageBlock|Text")
	float Size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextImageBlock|Text", DisplayName="Style")
	FTextStyle DefaultTextStyle;



	/**
	*  Get a Font from path
	*  @param Path path font file system
	*  @param FontInfo set a font
	*  @param FontSize new size font
	*/
	UFUNCTION(BlueprintCallable, Category = "TextImageBlock|Functions")
	bool SetFontFrom(FString Path, FSlateFontInfo& FontInfo, int32 FontSize = 24);

	/**
	*  determines the alignment appearance of text
	*  @param Justify Text alignment text 
	*/
	UFUNCTION(BlueprintCallable, Category = "TextImageBlock|Functions")
	void SetJustification(ETextJustify::Type Justify);

	/**
	*  Write new text
	*  @param NewText set new text with text localization
	*/
	UFUNCTION(BlueprintCallable, Category = "TextImageBlock|Functions", DisplayName="Set Text")
	void SetText(const FText NewText);

	/**
	*  Write DeprecatedFunction 
	*  @param NewText set new text with text localization
	*/
	UFUNCTION(BlueprintCallable, Category = "TextImageBlock|Functions", DisplayName = "Set Text With Text", meta = (DeprecatedFunction) )
	void SetFText(const FText NewText);

	/**
	*  Define if a text is default or image
	*  @param bEnableImage true image text false default text
	*  @param bUpdate force update text
	*/
	UFUNCTION(BlueprintCallable, Category = "TextImageBlock|Functions", DisplayName = "Change DefaultText To ImageText")
	void DefaultToImageText(bool bEnableImage, bool bUpdate = false);

	/**
	*  Define new style text
	*  @param TextStyle struct style
	*/
	UFUNCTION(BlueprintCallable, Category = "TextImageBlock|Functions")
	void SetTextStyle(FTextStyle TextStyle);

	/**
	*  Define new size text images
	*  @param NewSize new size
	*/
	UFUNCTION(BlueprintCallable, Category = "TextImageBlock|Functions")
	void SetTextImageSize(float NewSize);

	/**
	 *  Set the minimum desired width for this default text block
	 *  @param InMinDesiredWidth new minimum desired width
	 */
	UFUNCTION(BlueprintCallable, Category = "TextImageBlock|Functions")
	void SetMinDesiredWidth(float InMinDesiredWidth);


	UFUNCTION(BlueprintCallable, Category = "TextImageBlock|Functions")
	void UpdateRender();

	UFUNCTION(BlueprintCallable, Category = "TextImageBlock|Functions")
	void SetRetainRendering(bool bRetainer);

protected:

	/** Set Texture to UImage with size scale*/
	void SetTexture(UImage* Image, UTexture2D* NewTexture);


public:

	UFUNCTION(BlueprintCallable, Category = "TextImageBlock|Functions")
	void UpdateText();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateText();

	UFUNCTION(BlueprintImplementableEvent)
	void OnTextChange();
};
