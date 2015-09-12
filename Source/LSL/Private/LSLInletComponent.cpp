#include "LSLPrivatePCH.h"
#include "LSLInletComponent.h"
#include <string>

// Sets default values for this component's properties
ULSLInletComponent::ULSLInletComponent()
{
    bWantsInitializeComponent = true;
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void ULSLInletComponent::InitializeComponent()
{
    Super::InitializeComponent();
    if (StreamName != FString(TEXT("")))
    {
        pred += "name='" + StreamName + "'";
    }
    else if (StreamType != FString(TEXT("")))
    {
        pred += "type='" + StreamType + "'";
    }
}

void ULSLInletComponent::UninitializeComponent()
{
    if (my_inlet)
    {
        delete my_inlet;
        my_inlet = nullptr;
    }
    Super::UninitializeComponent();
}

// Called every frame
void ULSLInletComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

    if (my_inlet)
    {
        //TODO: Choose DataArray based on my_inlet->info().channel_format()
        double timestamp = my_inlet->pull_sample(FloatDataArray.GetData(), FloatDataArray.Num(), 0.0);
        //UE_LOG(LogLSL, Log, TEXT("Pulled sample with timestamp %f"), timestamp);
        if (timestamp > 0.0)
        {
            //TODO: Choose delegate to broadcast based on my_inlet->info().channel_format()
            OnStreamUpdatedFloat.Broadcast( FloatDataArray, DeltaTime );
        }
    }
    else
    {
        UE_LOG(LogLSL, Log, TEXT("Attempting to resolve stream with predicate %s."), *pred);
        std::vector<lsl::stream_info> results = lsl::resolve_stream(TCHAR_TO_ANSI(*pred), 1, 0.008);  // Will slow down while resolving non-existent threads.
        
        if (!results.empty())
        {
            UE_LOG(LogLSL, Log, TEXT("Stream found. Creating inlet."));
            my_inlet = new lsl::stream_inlet(results[0]);
            //TODO: Choose DataArray based on my_inlet->info().channel_format()
            FloatDataArray.SetNumZeroed(my_inlet->info().channel_count());
        }
    }
}