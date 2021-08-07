// Copyright (c) 2021 Chadwick Boulay

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLSL, All, All);

/**
 * The public interface to this module.
 */

class FLSL : public IModuleInterface //, public IModularFeature?
{
public:
    virtual void StartupModule();
    virtual void ShutdownModule();

private:
	void* LibLslHandle;
};
