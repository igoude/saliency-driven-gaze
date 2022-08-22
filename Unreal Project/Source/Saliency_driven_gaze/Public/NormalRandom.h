// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "NormalRandom.generated.h"

/**
 * 
 */
UCLASS()
class SALIENCY_DRIVEN_GAZE_API UNormalRandom : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/** Pick up a random number in the gaussian distribution */
	UFUNCTION(BlueprintCallable, Category = "Random")
	static float NormalRandom(float mu, float sigma);

	/** Pick up a random number in the gamma distribution */
	UFUNCTION(BlueprintCallable, Category = "Random")
	static float GammaRandom(float alpha, float loc, float scale);

	/** Get the probability given a value and a gaussian distribution */
	UFUNCTION(BlueprintCallable, Category = "Random")
	static float NormalProbability(float mu, float sigma, float x);
};
