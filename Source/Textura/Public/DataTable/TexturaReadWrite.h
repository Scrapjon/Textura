// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlock.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TexturaReadWrite.generated.h"

class FJsonObject;

USTRUCT(BlueprintType)
struct FTexturaTextData {
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString EntryName;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString TextContent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRichTextStyleRow StyleRow; 
	
};

/**
 * 
 */
UCLASS()
class TEXTURA_API UTexturaReadWrite : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// File Reading and writing code adapted from Alex Quevillon
	
	UFUNCTION(BlueprintCallable, Category="Data|Text|Read")
	static FString ReadStringFromFile(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage);
	
	UFUNCTION(BlueprintCallable, Category="Data|Text|Write")
	static void WriteStringToFile(FString FilePath, FString String, bool& bOutSuccess, FString& OutInfoMessage);
	
	// 
	static TSharedPtr<FJsonObject> ReadJson(const FString& JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage);
	
	// 
	static void WriteJson(const FString& JsonFilePath, const TSharedPtr<FJsonObject>& JsonObject, bool& bOutSuccess, FString& OutInfoMessage);
	
	UFUNCTION(BlueprintCallable, Category="Data|Json|Read")
	static FTexturaTextData ReadTextDataFromJsonFile(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage);
	
	UFUNCTION(BlueprintCallable, Category="Data|Json|Write")
	static void WriteTextDataToJsonFile(FString JsonFilePath, FTexturaTextData TextData, bool& bOutSuccess, FString& OutInfoMessage);
};
