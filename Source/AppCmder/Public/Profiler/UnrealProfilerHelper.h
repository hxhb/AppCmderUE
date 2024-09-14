// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UnrealProfilerHelper.generated.h"

/**
 * For Unreal Ingisht Profilier
 * by lipengzha
 */
UCLASS()
class APPCMDER_API UUnrealProfilerHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static void InsightChannel(FString Channels);
	UFUNCTION(BlueprintCallable)
	static void InsightTo(FString IP,int32 Port,FString Channels);
	UFUNCTION(BlueprintCallable)
	static void InsightOff();
};
