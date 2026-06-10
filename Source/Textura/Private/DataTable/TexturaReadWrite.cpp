// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTable/TexturaReadWrite.h"

#include "JsonObjectConverter.h"

FString UTexturaReadWrite::ReadStringFromFile(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage) {
	// Check if file exists
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists((*FilePath))) {
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read String From File Failed - File doesn't exist - '%s'"), *FilePath);
		return "";
	}
	
	FString RetString = "";
	
	// Try to read file. Output goes in RetString
	if (!FFileHelper::LoadFileToString(RetString, *FilePath)) {
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read String From File Failed - Was not able to read file. Is this a text file? - '%s'"), *FilePath);
		return "";
	}
	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Read String From File Succeeded - '%s'"), *FilePath);
	return RetString;
}

void UTexturaReadWrite::WriteStringToFile(FString FilePath, FString String, bool& bOutSuccess, FString& OutInfoMessage) {
	// Try to write string into the file
	if (!FFileHelper::SaveStringToFile(String, *FilePath)) {
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Write String To File Failed - Was not able to write to file. Is your file read only? Is the path valid? - '%s'"), *FilePath);
		return;
	}
	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Write String To File Succeeded - '%s'"), *FilePath);
}

TSharedPtr<FJsonObject> UTexturaReadWrite::ReadJson(const FString& JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage) {
	const FString JsonString = ReadStringFromFile(JsonFilePath, bOutSuccess, OutInfoMessage);
	if (!bOutSuccess) {
		return nullptr;
	}
	
	TSharedPtr<FJsonObject> RetJsonObject;
	
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), RetJsonObject)) {
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read Json Failed - Was not able to deserialize the json string. Is it the right format? - '%s'"), *JsonString);
		return nullptr;
	}
	
	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Read Json Succeeded - '%s'"), *JsonFilePath);
	return RetJsonObject;
}

void UTexturaReadWrite::WriteJson(const FString& JsonFilePath, const TSharedPtr<FJsonObject>& JsonObject, bool& bOutSuccess, FString& OutInfoMessage) {
	FString JsonString;
	
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&JsonString, 0))) {
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Write Json Failed - Was not able to serialize the json to string. Is the JsonObject valid?"));
		return;
	}
	
	WriteStringToFile(JsonFilePath, JsonString, bOutSuccess, OutInfoMessage);
	if (!bOutSuccess) {
		return;
	}
	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Write Json Succeeded - '%s'"), *JsonFilePath);
	
}

FTexturaTextData UTexturaReadWrite::ReadTextDataFromJsonFile(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage) {
	
	// Try to read generic json object from file
	TSharedPtr<FJsonObject> JsonObject = ReadJson(JsonFilePath, bOutSuccess, OutInfoMessage);
	if (!bOutSuccess) {
		return FTexturaTextData();
	}
	
	FTexturaTextData RetTextData;
	
	if (!FJsonObjectConverter::JsonObjectToUStruct<FTexturaTextData>(JsonObject.ToSharedRef(), &RetTextData)) {
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT(
			"Read Struct Json Failed - Was not able to convert the json object to your desired structure. "
			"Is is the right format / struct? - '%s'"), *JsonFilePath);
		return FTexturaTextData();
	}
	
	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Read Struct Json Succeeded - '%s'"), *JsonFilePath);
	return RetTextData;
}

void UTexturaReadWrite::WriteTextDataToJsonFile(FString JsonFilePath, const FTexturaTextData TextData, bool& bOutSuccess, FString& OutInfoMessage) {
	
	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(TextData);
	if (!JsonObject) {
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Write Struct Json Failed - Was not able to convert the struct to a json object. ")
			"This shouldn't really happen");
		return;
	}
	
	// Try to write JSON to file
	WriteJson(JsonFilePath, JsonObject, bOutSuccess, OutInfoMessage);
}
