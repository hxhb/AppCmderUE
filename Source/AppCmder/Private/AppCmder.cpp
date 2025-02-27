// Copyright Epic Games, Inc. All Rights Reserved.

#include "AppCmder.h"
#include "Engine/Engine.h"
#include "Profiler/UnrealProfilerHelper.h"
#include "Engine/Engine.h"
#define LOCTEXT_NAMESPACE "FAppCmderModule"

DEFINE_LOG_CATEGORY(LogAppCmder)

#define INSIGHT_TO_NAME TEXT("InsightTo")
static TAutoConsoleVariable<FString> CVarInsightTo(
	INSIGHT_TO_NAME,
	TEXT(""),
	TEXT("Launch Insight Profiling"),
	ECVF_Default);

namespace NSAppCmder
{
	TArray<FString> GetExecCmds()
	{
		TArray<FString> ExecCommands;
		// Optionally exec commands passed in the command line.
		FString ExecCmds;
		if( FParse::Value(FCommandLine::Get(), TEXT("AppExecCmds="), ExecCmds, false) )
		{
			TArray<FString> CommandArray;
			ExecCmds.ParseIntoArray( CommandArray, TEXT(","), true );

			for( int32 Cx = 0; Cx < CommandArray.Num(); ++Cx )
			{
				const FString& Command = CommandArray[Cx];
				// Skip leading whitespaces in the command.
				int32 Index = 0;
				while( FChar::IsWhitespace( Command[Index] ) )
				{
					Index++;
				}

				if( Index < Command.Len()-1 )
				{
					ExecCommands.AddUnique(*Command+Index);
				}
			}
		}
		return ExecCommands;
	}
	void OnPostEngineInit()
	{
		TArray<FString> ExecCmds = NSAppCmder::GetExecCmds();
		FString Display = FString::Printf( TEXT("[NSAppCmder::OnPostEngineInit] AppExecCmds: %d\n"),ExecCmds.Num());
		for(const FString& ExecCmd:ExecCmds)
		{
			Display += FString::Printf( TEXT("\t%s\n"), *ExecCmd);
			GEngine->Exec( nullptr, *ExecCmd, *GLog );
		}
		UE_LOG(LogAppCmder,Display,TEXT("%s"),*Display);
	}
}

void FAppCmderModule::StartupModule()
{
	bStatnamedevents = FParse::Param(FCommandLine::Get(), TEXT("statnamedevents"));
	FString EnabledChannels;
	FParse::Value(FCommandLine::Get(), TEXT("-trace="), EnabledChannels, false);
	bool bContainObjChannel = EnabledChannels.Contains(TEXT("OBJECT"),ESearchCase::IgnoreCase);
	if(bStatnamedevents && bContainObjChannel)
	{
#if STATS  
		FThreadStats::MasterEnableAdd();  
#endif
	}
	RegistInsightTo();
	FCoreDelegates::OnPostEngineInit.AddStatic(&NSAppCmder::OnPostEngineInit);
}

void FAppCmderModule::ShutdownModule()
{
	if(bStatnamedevents)
	{
#if STATS  
		FThreadStats::MasterEnableSubtract();    
#endif
	}
}

void FAppCmderModule::RegistInsightTo()
{
	auto CVar = IConsoleManager::Get().FindConsoleVariable(INSIGHT_TO_NAME);
	if (CVar)
	{
		CVar->SetOnChangedCallback(FConsoleVariableDelegate::CreateLambda([](IConsoleVariable *CVar)
		{
			FString CVarValue = CVar->GetString();

			if(CVarValue.Equals(TEXT("off"),ESearchCase::IgnoreCase))
			{
				UUnrealProfilerHelper::InsightOff();
				return;
			}
			
			FString DefaultIP = TEXT("127.0.0.1");
			if(!CVarValue.Contains(TEXT(" ")))
			{
				CVarValue += FString::Printf(TEXT(" %s"),*DefaultIP);
			}
			FString Ip,Channels;
			int32 Port = 1980;
			CVarValue.Split(TEXT(" "),&Channels,&Ip);
			bool bIsIp = !Ip.IsEmpty() && Ip.MatchesWildcard(TEXT("*.*.*.*"));
			if(bIsIp)
			{
				if(Ip.Contains(TEXT(":")))
				{
					FString IpStr;
					FString PortStr;
					Ip.Split(TEXT(":"),&IpStr,&PortStr);
					if(!PortStr.IsEmpty()){ Port = FCString::Atoi(*PortStr); }
					if(!IpStr.IsEmpty()){ Ip = IpStr; }
				}
				DefaultIP = Ip;
			}
			
			if(!CVarValue.IsEmpty() && !DefaultIP.IsEmpty() && !Channels.IsEmpty() )
			{
				UUnrealProfilerHelper::InsightTo(DefaultIP,Port,Channels);
			}
			else
			{
				UE_LOG(LogAppCmder,Warning,TEXT("InsightTo Params Invalid!"));
			}
		}));
	}
}



#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAppCmderModule, AppCmder)