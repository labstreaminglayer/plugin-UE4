// Copyright (c) 2021 Chadwick Boulay

#include "LSLOutletComponent.h"
#include "LSLPrivatePCH.h"
#include <string>

// Sets default values for this component's properties
ULSLOutletComponent::ULSLOutletComponent() :
	StreamName(),
	StreamType(),
	SamplingRate(LSL_IRREGULAR_RATE),
	ChannelFormat(EChannelFormat::cfmt_float32),
	StreamID("NoStreamID")
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    bWantsInitializeComponent = true;
}

// Called when the game starts
void ULSLOutletComponent::BeginPlay()
{
    Super::BeginPlay();
    //lsl_streaminfo lsl_myinfo = lsl_create_streaminfo(TCHAR_TO_ANSI(*StreamName), TCHAR_TO_ANSI(*StreamType), ChannelCount, SamplingRate, (lsl_channel_format_t)ChannelFormat, TCHAR_TO_ANSI(*StreamID));
    UE_LOG(LogLSL, Log, TEXT("Attempting to create stream outlet with name %s, type %s, sampling rate %d."), *StreamName, *StreamType, SamplingRate);
    lsl::stream_info data_info(
		TCHAR_TO_ANSI(*StreamName),
		TCHAR_TO_ANSI(*StreamType),
		(int16)Channels.Num(),
		(double)SamplingRate,
		lsl::channel_format_t(ChannelFormat),
		TCHAR_TO_ANSI(*StreamID)
	);
    lsl::xml_element channels = data_info.desc().append_child("channels");
	for (auto& ch : Channels)
	{
		channels.append_child("channel")
			.append_child_value("label", TCHAR_TO_UTF8(*(ch.Label)))
			.append_child_value("unit", TCHAR_TO_UTF8(*(ch.Unit)));
	}
    
    //TODO: Check to see if the stream already exists.
    my_outlet = new lsl::stream_outlet(data_info);
}

void ULSLOutletComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (my_outlet != nullptr)
    {
        delete my_outlet;
        my_outlet = nullptr;
    }
    Super::EndPlay(EndPlayReason);
}

//push_sample functions
void ULSLOutletComponent::PushSampleFloat(TArray<float> data)
{
    my_outlet->push_sample(data.GetData());
}


/*
void ULSLOutletComponent::PushSampleDouble(TArray<double> data)
{
    my_outlet->push_sample(data.GetData());
}
*/

void ULSLOutletComponent::PushSampleLong(TArray<int32> data)
{
    my_outlet->push_sample(data.GetData());
}

/*
void ULSLOutletComponent::PushSampleInt(TArray<int16> data)
{
    my_outlet->push_sample(data.GetData());
}
*/

/*
void ULSLOutletComponent::PushSampleShort(TArray<int16> data)
{
    my_outlet->push_sample(data.GetData());
}
*/

void ULSLOutletComponent::PushSampleString(TArray<FString> data)
{
    std::vector<std::string> strVec;
    int32 b;
    for(b=0; b < data.Num(); b++)
    {
        strVec.push_back(TCHAR_TO_UTF8(*data[b]));
    }
    my_outlet->push_sample(strVec);
}

/*
void ULSLOutletComponent::PushSampleChar(TArray<char> data)
{
    lsl_push_sample_ctp(lsl_myoutlet,const_cast<char*>(data),0.0,true);
}
*/
