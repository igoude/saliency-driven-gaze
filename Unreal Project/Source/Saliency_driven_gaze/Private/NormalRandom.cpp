// Fill out your copyright notice in the Description page of Project Settings.

#include "NormalRandom.h"

#include "Math/UnrealMathUtility.h"
#include <random>

#define M_PI 3.14159265358979323846  /* pi */

float UNormalRandom::NormalRandom(float mu, float sigma)
{
	std::random_device rd{};
	std::mt19937 gen{ rd() };
	std::normal_distribution<float> d{ mu, sigma };

	return d(gen);
}


float UNormalRandom::GammaRandom(float alpha, float loc, float scale)
{
	std::random_device rd{};
	std::mt19937 gen{ rd() };
	std::gamma_distribution<float> d{ alpha, scale };

	return d(gen) + loc;
}


float UNormalRandom::NormalProbability(float mu, float sigma, float x)
{
	float result = (1.0f / sigma * FMath::Sqrt(2.0 * M_PI)) * FMath::Exp(-0.5f * FMath::Pow(((x - mu) / sigma), 2.0f));
	return result;
}