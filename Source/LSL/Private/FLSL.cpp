#include "LSLPrivatePCH.h"
#include "FLSL.h"

DEFINE_LOG_CATEGORY(LogLSL)
IMPLEMENT_MODULE(FLSL, LSL)

void FLSL::StartupModule()
{
    UE_LOG(LogLSL, Log, TEXT("Loaded LSL module in LabStreamingLayer plugin."));
}

void FLSL::ShutdownModule()
{
    UE_LOG(LogLSL, Log, TEXT("Unloaded LSL module in LabStreamingLayer plugin."));
}