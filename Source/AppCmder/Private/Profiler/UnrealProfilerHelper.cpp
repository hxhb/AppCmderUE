// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealProfilerHelper.h"
#include "ParseTokens.h"
#include "AppCmder.h"
#if !UE_BUILD_SHIPPING
	#include "Trace/Trace.h"
#endif

void UUnrealProfilerHelper::InsightChannel(FString Channels)
{
#if !UE_BUILD_SHIPPING
	FString EnabledChannels = Channels;
	UE::String::ParseTokens(EnabledChannels, TEXT(","), [](FStringView Token) {
		TCHAR ChannelName[64];
		const size_t ChannelNameSize = Token.CopyString(ChannelName, 64);
		ChannelName[ChannelNameSize] = '\0';
		Trace::ToggleChannel(ChannelName, true);
	});
#endif
}

void UUnrealProfilerHelper::InsightTo(FString IP,int32 Port,FString Channels)
{
#if !UE_BUILD_SHIPPING
	InsightChannel(Channels);
	Trace::SendTo(*IP,Port);
	UE_LOG(LogAppCmder,Display,TEXT("InsightTo %s:%d,Channels %s."),*IP,Port,*Channels);
#endif
}

void UUnrealProfilerHelper::InsightOff()
{
#if !UE_BUILD_SHIPPING
	UE_LOG(LogAppCmder,Display,TEXT("InsightTo off,ToggleAll as false."));
	Trace::FChannel::ToggleAll(false);
#endif
}
