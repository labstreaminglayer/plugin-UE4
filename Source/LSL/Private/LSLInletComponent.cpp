// Copyright (c) 2021 Chadwick Boulay

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
        try {

        
            while (timestamp > 0.0)
            {
                if (StreamDataFormat == EChannelFormat::cfmt_float32)
                {
                    timestamp = my_inlet->pull_sample(FloatDataArray.GetData(), FloatDataArray.Num(), 0.0);
                    if (timestamp > 0.0)
                    {
                        //UE_LOG(LogLSL, Log, TEXT("Pulled sample with timestamp %f and value %f"), timestamp, FloatDataArray[0]);
                        //std::cout << "Timestamp" << timestamp << std::endl;
                        OnStreamUpdatedFloat.Broadcast(FloatDataArray, DeltaTime);
                    }
                }
                else if (StreamDataFormat == EChannelFormat::cfmt_double64)
                {
                    timestamp = my_inlet->pull_sample(double_vec.data(), double_vec.size(), 0.0);
                    if (timestamp > 0.0)
                    {
                        // UE_LOG(LogLSL, Log, TEXT("Pulled sample with timestamp %f and value %f"), timestamp, double_vec[0]);
                        //std::cout << "Timestamp" << timestamp << std::endl;
                        // Cast double to float
                        for (size_t i = 0; i < double_vec.size(); i++)
                        {
                            FloatDataArray[i] = (float)double_vec[i];
                        }
                        OnStreamUpdatedFloat.Broadcast(FloatDataArray, DeltaTime);
                    }
                }
                else if (StreamDataFormat == EChannelFormat::cfmt_string)
                {
                    timestamp = my_inlet->pull_sample(string_vec.data(), string_vec.size(), 0.0);
                    if (timestamp > 0.0)
                    {
                        FString unrealString(string_vec[0].c_str());
                        // UE_LOG(LogLSL, Log, TEXT("Pulled string sample with timestamp %f and value %s"), timestamp, *unrealString);
                        for (size_t i = 0; i < string_vec.size(); i++)
                        {
                            StringDataArray[i] = string_vec[i].c_str();
                        }
                        OnStreamUpdatedString.Broadcast(StringDataArray, DeltaTime);
                    }
                }
            }

        }
        catch (lsl::timeout_error& e)
        {
            delete my_inlet;
            my_inlet = nullptr;
            FString unrealErrorString(e.what());
            UE_LOG(LogLSL, Warning, TEXT("Caught lsl::timeout_error: %s"), *unrealErrorString);
            OnStreamCaughtException.Broadcast(unrealErrorString);
        }
    }
    else
    {
        if (!my_resolver)
        {
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

        std::vector<lsl::stream_info> results = my_resolver->results();
        if (!results.empty())
        {
            UE_LOG(LogLSL, Log, TEXT("Stream found. Creating inlet."));
            my_inlet = new lsl::stream_inlet(results[0], MaxBufLen, MaxChunkLen);
            StreamDataFormat = EChannelFormat(my_inlet->info().channel_format());
            FloatDataArray.SetNumZeroed(0);
            double_vec.resize(0);
            StringDataArray.SetNumZeroed(0);
            if (StreamDataFormat == EChannelFormat::cfmt_float32)
            {
                FloatDataArray.SetNumZeroed(my_inlet->info().channel_count());
            }
            else if (StreamDataFormat == EChannelFormat::cfmt_double64)
            {
                FloatDataArray.SetNumZeroed(my_inlet->info().channel_count());
                double_vec.resize(my_inlet->info().channel_count());
            }
            else if (StreamDataFormat == EChannelFormat::cfmt_string)
            {
                StringDataArray.SetNumZeroed(my_inlet->info().channel_count());
                string_vec.resize(my_inlet->info().channel_count());
            }
            
            // resolver can rest now
            delete my_resolver;
            my_resolver = nullptr;
        }
    }
}
