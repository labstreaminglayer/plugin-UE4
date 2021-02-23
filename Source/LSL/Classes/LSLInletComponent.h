// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#pragma warning (push)
#pragma warning (disable: 4800)
#include "lsl_cpp.h"
#pragma warning (pop)
#include "LSLTypes.h"
#include "LSLInletComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LSL_API ULSLInletComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    ULSLInletComponent();

    // Called when the game starts
    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // Called every frame
    virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

    // Name of stream. Used to resolve stream.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LSL)
    FString StreamName;

    // Type of stream. Used to resolve stream if name not present.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LSL)
    FString StreamType;

    // Maximum buffer length in seconds if there is a nominal sampling rate, otherwise x100 in samples
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LSL)
    int32 MaxBufLen = 10;

    // The maximum size, in samples, at which chunks are transmitted. (0 = the chunk sizes used by the sender)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LSL)
    int32 MaxChunkLen = 0;

    // Delegate triggered once per frame update for stream
    UPROPERTY(BlueprintAssignable, Category = LSL)
    FLSLStreamUpdatedFloatDelegate OnStreamUpdatedFloat;

    // TODO: More types of DataArrays
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LSL)
    TArray<float> FloatDataArray;

protected:
    
    lsl::stream_inlet *my_inlet;
    lsl::continuous_resolver* my_resolver;
};
