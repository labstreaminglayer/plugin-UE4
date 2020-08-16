#include "FLSL.h"
#include "LSLPrivatePCH.h"
#include "Interfaces/IPluginManager.h"

DEFINE_LOG_CATEGORY(LogLSL)
IMPLEMENT_MODULE(FLSL, LSL)

void FLSL::StartupModule()
{
	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("LabStreamingLayer")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("ThirdParty/liblsl/bin/lsl.dll"));

	LibLslHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;
}

void FLSL::ShutdownModule()
{
	// Free the dll handle
	FPlatformProcess::FreeDllHandle(LibLslHandle);
	LibLslHandle = nullptr;
}
