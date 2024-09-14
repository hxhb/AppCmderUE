// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
DECLARE_LOG_CATEGORY_EXTERN(LogAppCmder, Log, All);

class FAppCmderModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
protected:
	void RegistInsightTo();
	bool bStatnamedevents = false;
};
