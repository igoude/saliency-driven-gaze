// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Misc/Paths.h"

typedef bool(*__InitNet)(const char* model_path);
typedef bool(*__GetFixation)(unsigned char* image, unsigned char* bias_map, float salient_weight, int* uv_coordinates);
typedef bool(*__GetFixationSalmap)(unsigned char* image, unsigned char* bias_map, float salient_weight, unsigned char* salmap, int* uv_coordinates);

#define kImageSize 256

/**
 * 
 */
class SALIENCY_DRIVEN_GAZE_API FSaccadicWorker: public FRunnable
{
public:
	
	// Constructor, create the thread by calling this
	FSaccadicWorker(bool debugMode);

	// Destructor
	virtual ~FSaccadicWorker() override;

	// Overriden from FRunnable
	// Do not call these functions youself, that will happen automatically
	bool Init() override; // Do your setup here, allocate memory, ect.
	uint32 Run() override; // Main data processing happens here
	void Stop() override; // Clean up any memory you allocated here

	// The boolean to safely read inputs and outputs from the game thread
	bool bInputReady;
	bool bOutputReady;

	/** Useful parameters */
	TArray<FColor> m_ViewData;
	TArray<FColor> m_BiasData;
	TArray<FColor> m_SalmapData;
	float m_SalientWeight;
	int* m_FixationCoords;

	bool m_debugMode;

private:
	// Used to know when the thread should exit, changed in Stop(), read in Run()
	bool bRunThread;

	// Thread handle. Control the thread using this, with operators like Kill and Suspend
	FRunnableThread* Thread;

	/** DLL Handle */
	void* v_dllHandle;

	/** DLL Functions */
	__InitNet m_funcInitNetCV;
	__GetFixation m_funcGetFixationCV;
	__GetFixationSalmap m_funcGetFixationSalmapCV;

	/** Paths relative to project dir */
	const char* m_dllPath = "Binaries/Win64/saliency_wrapper.dll";
	const char* m_modulePath = "SaccadicModel/msinet_scripted_256.pt";

	/**
	* Attempt to import DLL.
	* @return Whether the operation is succesfull.
	*/
	bool ImportDLL();

	/**
	* Attempt to import all functions of the DLL.
	* @return Whether the operation is succesfull.
	*/
	bool ImportMethods();

	/**
	* Init the Msinet model
	*/
	void CallInitNetCV();
};
