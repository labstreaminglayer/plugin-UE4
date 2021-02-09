#include "LSLInletComponent.h"
#include "LSLPrivatePCH.h"
#include <string>
#include <iostream>

// Sets default values for this component's properties
ULSLInletComponent::ULSLInletComponent()
: StreamName(""), StreamType("")
{
    bWantsInitializeComponent = true;
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void ULSLInletComponent::BeginPlay()
{
    Super::BeginPlay();
    
    FString pred;
    if (StreamName != FString(TEXT("")))
    {
        pred += "name='" + StreamName + "'";
    }
    else if (StreamType != FString(TEXT("")))
    {
        pred += "type='" + StreamType + "'";
    }
    std::string pred_string(TCHAR_TO_UTF8(*pred));
    my_resolver = new lsl::continuous_resolver(pred_string);
}

void ULSLInletComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (my_inlet != nullptr)
    {
        delete my_inlet;
        my_inlet = nullptr;
    }
    if (my_resolver != nullptr)
    {
        delete my_resolver;
        my_resolver = nullptr;
    }
    Super::EndPlay(EndPlayReason);
}

// Called every frame
void ULSLInletComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

    if (my_inlet)
    {
        double timestamp = 1.0;
        while (timestamp > 0.0)
        {
            //TODO: Choose DataArray based on my_inlet->info().channel_format()
            timestamp = my_inlet->pull_sample(FloatDataArray.GetData(), FloatDataArray.Num(), 0.0);
            //UE_LOG(LogLSL, Log, TEXT("Pulled sample with timestamp %f"), timestamp);
            //std::cout << "Timestamp" << timestamp << std::endl;
            if (timestamp > 0.0)
            {
                //TODO: Choose delegate to broadcast based on my_inlet->info().channel_format()
                OnStreamUpdatedFloat.Broadcast(FloatDataArray, DeltaTime);
            }
        }
        
    }
    else
    {
        std::vector<lsl::stream_info> results = my_resolver->results();
        if (!results.empty())
        {
            UE_LOG(LogLSL, Log, TEXT("Stream found. Creating inlet."));
            my_inlet = new lsl::stream_inlet(results[0]);
            //TODO: Choose DataArray based on my_inlet->info().channel_format()
            FloatDataArray.SetNumZeroed(my_inlet->info().channel_count());
        }
    }
}
