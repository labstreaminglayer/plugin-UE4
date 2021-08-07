// Copyright (c) 2021 Chadwick Boulay

#pragma once

#include "Components/SceneComponent.h"
#include "LSLTypes.h"
#pragma warning (push)
#pragma warning (disable: 4800)
#include "lsl_cpp.h"
#pragma warning (pop)
#include "LSLOutletComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LSL)
    FString StreamName;

    // Type of stream. Used to build stream info
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LSL)
    FString StreamType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LSL)
    float SamplingRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LSL)
    EChannelFormat ChannelFormat;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LSL)
    FString StreamID;

	UPROPERTY(EditAnywhere, Category = LSL)
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

protected:

    lsl::stream_outlet *my_outlet;

};
