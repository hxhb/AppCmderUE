#if PLATFORM_ANDROID
#include "CoreMinimal.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatformMisc.h"
#include "Kismet/KismetStringLibrary.h"
#include "Resources/Version.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <cpu-features.h>
#include <android_native_app_glue.h>
#include <cstdio>
#include <sys/resource.h>
#include <sys/system_properties.h>
#include <jni.h>
#include <android/sensor.h>
#include "Misc/EngineVersionComparison.h"

#if !UE_VERSION_OLDER_THAN(5,0,0)
	#define UNREAL_JNI_METHOD_NAME Java_com_epicgames_unreal_GameActivity_nativeAppendCommand
#else
	#define UNREAL_JNI_METHOD_NAME Java_com_epicgames_ue4_GameActivity_nativeAppendCommand
#endif

// Name of the UE4 commandline append setprop
static constexpr char UE4CommandLineSetprop[] = "debug.ue4.commandline";
static const uint32 CMD_LINE_MAX = 16384u;

FString FStringFromParam(JNIEnv* Env, jstring JavaString)
{
	if (!Env || !JavaString || Env->IsSameObject(JavaString, NULL))
	{
		return {};
	}

	const auto chars = Env->GetStringUTFChars(JavaString, 0);
	FString ReturnString(UTF8_TO_TCHAR(chars));
	Env->ReleaseStringUTFChars(JavaString, chars);
	return ReturnString;
}

FString FStringFromLocalRef(JNIEnv* Env, jstring JavaString)
{
	FString ReturnString = FStringFromParam(Env, JavaString);

	if (Env && JavaString)
	{
		Env->DeleteLocalRef(JavaString);
	}

	return ReturnString;
}

jmethodID GetExtraCmdlineMethodID()
{
	jmethodID GetExtraCmdlineMethod = NULL;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		GetExtraCmdlineMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetExtraCmdline", "()Ljava/lang/String;", false);
	}
	return GetExtraCmdlineMethod;
}

FString GetExtraCmdline()
{
	FString ExtraCmdline = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		ExtraCmdline = FStringFromLocalRef(Env, (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, GetExtraCmdlineMethodID()));
	}
	if(!ExtraCmdline.IsEmpty())
	{
		TArray<FString> Sections = UKismetStringLibrary::ParseIntoArray(ExtraCmdline,TEXT("/.,;/"));
		ExtraCmdline.Empty();
		for(const auto& Section: Sections)
		{
			ExtraCmdline += FString::Printf(TEXT(" %s"),*Section);
		}
	}
	UE_LOG(LogTemp,Display,TEXT("Java Extra Cmdline: %s"),*ExtraCmdline);
	return ExtraCmdline;
}

JNI_METHOD void UNREAL_JNI_METHOD_NAME()
{
	FString ExtraCmdline = GetExtraCmdline();

	FString Key = TEXT("cmdline");
	FString Value = TEXT("");
	if(!ExtraCmdline.IsEmpty())
	{
		Value = ExtraCmdline;
	}
	else
	{
#if ENGINE_MINOR_VERSION < 26
		char CommandLineSetpropAppend[CMD_LINE_MAX];
		if (__system_property_get(UE4CommandLineSetprop, CommandLineSetpropAppend) > 0)
		{
			Value = (UTF8_TO_TCHAR(CommandLineSetpropAppend));
		}
#endif
	}
	if(!Value.IsEmpty())
	{
		FAndroidMisc::ConfigRulesVariables.Add(Key, Value);
		UE_LOG(LogTemp,Display,TEXT("Append Command: %s"),*Value);
	}
}
#endif