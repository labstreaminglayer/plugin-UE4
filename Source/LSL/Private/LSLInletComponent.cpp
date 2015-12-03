#include "LSLPrivatePCH.h"
#include "LSLInletComponent.h"
#include <string>
#include <iostream>

// Sets default values for this component's properties
ULSLInletComponent::ULSLInletComponent()
: StreamName("NoName"), StreamType("NoType"),
  ResolveAttemptInterval(1.0f), tSinceLastResolveAttempt(0.0f)
{
    bWantsInitializeComponent = true;
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void ULSLInletComponent::BeginPlay()
{
    Super::BeginPlay();
    if (StreamName != FString(TEXT("")))
    {
        pred += "name='" + StreamName + "'";
    }
    else if (StreamType != FString(TEXT("")))
    {
        pred += "type='" + StreamType + "'";
    }
}

void ULSLInletComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (my_inlet != nullptr)
    {
        delete my_inlet;
        my_inlet = nullptr;
    }
    Super::EndPlay(EndPlayReason);
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
        std::cout << "Timestamp" << timestamp << std::endl;
        if (timestamp > 0.0)
        {
            //TODO: Choose delegate to broadcast based on my_inlet->info().channel_format()
            OnStreamUpdatedFloat.Broadcast( FloatDataArray, DeltaTime );
        }
    }
    else
    {
        tSinceLastResolveAttempt += DeltaTime;
        if (tSinceLastResolveAttempt >= ResolveAttemptInterval)
        {
            tSinceLastResolveAttempt = 0.0f;
            UE_LOG(LogLSL, Log, TEXT("Attempting to resolve stream with predicate %s."), *pred);
            std::vector<lsl::stream_info> results = lsl::resolve_stream(TCHAR_TO_ANSI(*pred), 1, 1.0/125.0);
            if (!results.empty())
            {
                UE_LOG(LogLSL, Log, TEXT("Stream found. Creating inlet."));
                my_inlet = new lsl::stream_inlet(results[0]);
                //TODO: Choose DataArray based on my_inlet->info().channel_format()
                FloatDataArray.SetNumZeroed(my_inlet->info().channel_count());
            }
        }
        
    }
}