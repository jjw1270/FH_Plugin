// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#define CHECK_VALID(Obj) \
if (!(Obj)) \
{ \
    UE_LOG(LogTemp, Error, TEXT("%s : %s"), *GetClass()->GetName(), TEXT(#Obj)); \
    return; \
}

class FItemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
