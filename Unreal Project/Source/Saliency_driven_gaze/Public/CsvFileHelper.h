// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "CsvFileHelper.generated.h"


/** Structure that defines a fixation */
USTRUCT(BlueprintType)
struct FFixationData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:

    FFixationData()
        : Position(0)
        , Duration(0)
        , TimeStamp(0)
        , ObjectName("")
        , ObjectPosition(0)
        , ObjectRotation(0)
        , ObjectScale(0)
        , HitPosition(0)
        , HitNormal(0)
        , EyeDirection(0)
        , HeadPosition(0)
        , HeadRotation(0)
        , EyeLeftBlink(0)
        , EyeRightBlink(0)
        , GazeLeftCoord(0)
        , GazeRightCoord(0)
        , GazeCombineCoord(0)
        , Tracked(false)
    {}

    /** The 'Name' column is the index of the fixation */

    /** Position of the fixation in world space */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fixation)
        FVector Position;

    /** Duration of the fixation in seconds */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fixation)
        float Duration;

    /** Time stamp of the fixation in seconds */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fixation)
        float TimeStamp;

    /** Unique name of the hit object (if exists) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fixation)
        FString ObjectName;

    /** Position of the hit object in world space */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fixation)
        FVector ObjectPosition;

    /** Rotation of the hit object in world space */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fixation)
        FRotator ObjectRotation;

    /** Scale of the hit object in world space */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fixation)
        FVector ObjectScale;

    /** Position of the fixation in object space */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fixation)
        FVector HitPosition;

    /** Normal at the hit point in world space */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fixation)
        FVector HitNormal;

    /** Direction from the eyes to the fixation in world space */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fixation)
        FVector EyeDirection;

    /** Position of the head/HMD in world space */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fixation)
        FVector HeadPosition;

    /** Rotation of the head/HMD in world space */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fixation)
        FRotator HeadRotation;

    /** Blink value of the left eye (0 is open, 1 is close) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fixation)
        float EyeLeftBlink;

    /** Blink value of the right eye (0 is open, 1 is close) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fixation)
        float EyeRightBlink;

    /** Screenspace coordinate of the left eye gaze */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fixation)
        FVector2D GazeLeftCoord;

    /** Screenspace coordinate of the right eye gaze */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fixation)
        FVector2D GazeRightCoord;

    /** Screenspace coordinate of the combined eyes gaze */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fixation)
        FVector2D GazeCombineCoord;

    /** Whereas the eye tracker has something to track */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fixation)
        bool Tracked;
};


/** Structure that defines the experiment */
USTRUCT(BlueprintType)
struct FExperimentData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:

    FExperimentData()
        : Done(0)
        , Level_1_void(0)
        , Level_1_populated(0)
        , Level_1_void_event(0)
        , Level_1_populated_event(0)
        , Level_2_void(0)
        , Level_2_populated(0)
        , Level_2_void_event(0)
        , Level_2_populated_event(0)
        , Level_3_void(0)
        , Level_3_populated(0)
        , Level_3_void_event(0)
        , Level_3_populated_event(0)
    {}

    /** The 'Name' column is the index of the participant */

    /** Does this participant has already completed the experiment */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experiment)
        int Done;

    /** Orders of the experiment's levels */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experiment)
        int Level_1_void;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experiment)
        int Level_1_populated;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experiment)
        int Level_1_void_event;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experiment)
        int Level_1_populated_event;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experiment)
        int Level_2_void;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experiment)
        int Level_2_populated;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experiment)
        int Level_2_void_event;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experiment)
        int Level_2_populated_event;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experiment)
        int Level_3_void;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experiment)
        int Level_3_populated;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experiment)
        int Level_3_void_event;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experiment)
        int Level_3_populated_event;
};

/**
 * 
 */
UCLASS()
class SALIENCY_DRIVEN_GAZE_API UCsvFileHelper : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    UFUNCTION(BlueprintCallable, Category = "Csv")
    static bool SaveFixationTableToCsvFile(FString SaveDirectory, FString FileName, TArray<FFixationData> FixationTable, bool AllowOverWriting);
    
    UFUNCTION(BlueprintCallable, Category = "Csv")
    static bool SaveExperimentTableToCsvFile(FString SaveDirectory, FString FileName, TArray<FString> UserIDs, TArray<FExperimentData> ExperimentTable, bool AllowOverWriting);

    UFUNCTION(BlueprintCallable, Category = "Csv")
    static bool SaveScenePointsToPlyFile(FString SaveDirectory, FString FileName, TArray<FVector> Points, bool AllowOverWriting);

    UFUNCTION(BlueprintCallable, Category = "Csv")
    static int LoadPointCloudPlyFile(FString LoadDirectory, FString FileName, UPARAM(ref) TArray<FString>& PlyJson);
    
    //UFUNCTION(BlueprintCallable, Category = "Csv")
    //static bool LoadPointsJson(UPARAM(ref) TArray<FString>& PlyJson, UPARAM(ref) TArray<FVector>& pointPositions, UPARAM(ref) TArray<FColor>& pointColors, int nbPointsMax = 100, int modulo = 64);

    UFUNCTION(BlueprintCallable, Category = "Csv")
    static TArray<FLinearColor> LoadPointsJson(UPARAM(ref) TArray<FString>& PlyJson, int nbPoints = 100);


    static FString FIntToFString(int value);
    static FString FBoolToFString(bool value);
    static FString FFloatToFString(float value);
    static FString FVectorToFString(FVector vector);
    static FString FVector2DToFString(FVector2D vector2D);
    static FString FRotatorToFString(FRotator rotator);
};
