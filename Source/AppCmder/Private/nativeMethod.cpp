#include "CoreMinimal.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidPlatformMisc.h"
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

// Name of the UE4 commandline append setprop
static constexpr char UE4CommandLineSetprop[] = "debug.ue4.commandline";
static const uint32 CMD_LINE_MAX = 16384u;

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeAppendCommand()
{
	FString Key = TEXT("cmdline");
	FString Value = TEXT("");
	char CommandLineSetpropAppend[CMD_LINE_MAX];
	if (__system_property_get(UE4CommandLineSetprop, CommandLineSetpropAppend) > 0)
	{
		Value = (UTF8_TO_TCHAR(CommandLineSetpropAppend));
		FAndroidMisc::ConfigRulesVariables.Add(Key, Value);
	}
	UE_LOG(LogTemp,Display,TEXT("Append Command: %s"),*Value);
}
#endif