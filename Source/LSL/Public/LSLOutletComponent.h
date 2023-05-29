// Copyright (c) 2021 Chadwick Boulay

#pragma once

#include "Components/SceneComponent.h"
#include "LSLTypes.h"
#include "FLSL.h"
#pragma warning (push)
#pragma warning (disable: 4800)
#define LSL_CPP11 1
#include "lsl_cpp.h"
#pragma warning (pop)
#include "LSLOutletComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LSL_API ULSLOutletComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    ULSLOutletComponent();

    // Called when the game starts
    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // Name of stream. Used to build stream info
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LSL, meta = (ExposeOnSpawn = "true"))
    FString StreamName;

    // Type of stream. Used to build stream info
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LSL, meta = (ExposeOnSpawn = "true"))
    FString StreamType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LSL, meta = (ExposeOnSpawn = "true"))
    float SamplingRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LSL, meta = (ExposeOnSpawn = "true"))
    EChannelFormat ChannelFormat;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LSL, meta = (ExposeOnSpawn = "true"))
    FString StreamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LSL, meta = (ExposeOnSpawn = "true"))
	TArray<FChannelData> Channels;

    UFUNCTION(BlueprintCallable, Category = LSL)
    void PushSampleFloat(TArray<float> data);

    //UFUNCTION(BlueprintCallable, Category = LSL)
    //void PushSampleDouble(TArray<double> data);

    UFUNCTION(BlueprintCallable, Category = LSL)
    void PushSampleLong(TArray<int32> data);

    //UFUNCTION(BlueprintCallable, Category = LSL)
    //void PushSampleInt(TArray<int16> data);

    //UFUNCTION(BlueprintCallable, Category = LSL)
    //void PushSampleShort(TArray<int16> data);
 
    UFUNCTION(BlueprintCallable, Category = LSL)
    void PushSampleString(TArray<FString> data);

    /*
    UFUNCTION(BlueprintCallable, Category = LSL)
    void PushSampleChar(TArray<char> data);
    */
	template<typename T>
	void PushSample(TArray<T> data);

protected:

    lsl::stream_outlet *my_outlet = nullptr;

};

template<typename T>
inline void ULSLOutletComponent::PushSample(TArray<T> data) {
	if (my_outlet == nullptr) {
		UE_LOG(LogLSL, Warning, TEXT("%s outlet isn't valid, can't push sample"), *(GetFName().ToString()));
		return;
	}
	if (data.Num() == 0) {
		UE_LOG(LogLSL, Warning, TEXT("Trying to push empty sample to %s, skipping"), *(GetFName().ToString()));
		return;
	}
	my_outlet->push_sample(data.GetData());
}
