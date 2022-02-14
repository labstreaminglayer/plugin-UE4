// Copyright (c) 2021 Chadwick Boulay

#pragma once

#include "LSLTypes.generated.h"

//---------------------------------------------------
// Delegate types
//---------------------------------------------------

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLSLStreamUpdatedStringDelegate, const TArray<FString>&, StringArray, float, DeltaSeconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLSLStreamUpdatedFloatDelegate, const TArray<float>&, FloatArray, float, DeltaSeconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLSLStreamCaughtExceptionDelegate, const FString&, ErrorString);

UENUM(BlueprintType)
enum class EChannelFormat : uint8
{
    cfmt_undefined   =0  UMETA(DisplayName="undefined"),
    cfmt_float32     =1  UMETA(DisplayName="float32"),
    cfmt_double64    =2  UMETA(DisplayName="double64"),
    cfmt_string      =3  UMETA(DisplayName="string"),
    cfmt_int32       =4  UMETA(DisplayName="int32"),
    cfmt_int16       =5  UMETA(DisplayName="int16"),
    cfmt_int8        =6  UMETA(DisplayName="int8"),
    cfmt_int64       =7  UMETA(DisplayName="int64")
};

USTRUCT(BlueprintType)
struct FChannelData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = LSL) FString Label;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = LSL) FString Unit;
};

UCLASS()
class ULSLTypes : public UObject
{
	GENERATED_BODY()
};
