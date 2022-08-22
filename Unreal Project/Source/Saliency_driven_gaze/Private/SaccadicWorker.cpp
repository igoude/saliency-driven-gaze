// Fill out your copyright notice in the Description page of Project Settings.

#include "SaccadicWorker.h"

// This code will be run on the thread that invoked this thread (i.e. game thread)
#pragma region Main Thread Code

FSaccadicWorker::FSaccadicWorker(bool debugMode)
{
	m_debugMode = debugMode;

	// Import the DLL and Load Functions
	if (!this->ImportDLL())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Import DLL"));
	}
	if (!this->ImportMethods())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Import DLL Methods"));
	}

	// Init torch module
	this->CallInitNetCV();
	UE_LOG(LogTemp, Warning, TEXT("Our fixation thread has been initialized"));

	Thread = FRunnableThread::Create(this, TEXT("SaccadicThread"), 0U, EThreadPriority::TPri_Highest);
}

FSaccadicWorker::~FSaccadicWorker()
{
	if (Thread)
	{
		// Kill() is a blocking call, it waits for the thread to finish.
		// Hopefully that doesn't take too long
		Thread->Kill();
		delete Thread;
	}

	FPlatformProcess::FreeDllHandle(v_dllHandle);
}

bool FSaccadicWorker::ImportDLL()
{
	// Init DLL from a Path
	FString FilePath = FPaths::ProjectDir() + FString(m_dllPath);
	UE_LOG(LogTemp, Warning, TEXT("DLL path: %s"), *FilePath);

	if (FPaths::FileExists(FilePath))
	{
		v_dllHandle = FPlatformProcess::GetDllHandle(*FilePath);
		if (v_dllHandle != NULL)
		{
			return true;
		}
	}
	return false;
}

bool FSaccadicWorker::ImportMethods()
{
	// Loop Through and Import All Functions from DLL   --   Make Sure proc_name matches name of DLL method
	if (v_dllHandle != NULL)
	{
		FString ProcName = FString("InitNet");
		m_funcInitNetCV = (__InitNet)FPlatformProcess::GetDllExport(v_dllHandle, *ProcName);
		if (m_funcInitNetCV == NULL)
		{
			return false;
		}

		ProcName = FString("GetFixation");
		m_funcGetFixationCV = (__GetFixation)FPlatformProcess::GetDllExport(v_dllHandle, *ProcName);
		if (m_funcGetFixationCV == NULL)
		{
			return false;
		}

		ProcName = FString("GetFixationSalmap");
		m_funcGetFixationSalmapCV = (__GetFixationSalmap)FPlatformProcess::GetDllExport(v_dllHandle, *ProcName);
		if (m_funcGetFixationSalmapCV == NULL)
		{
			return false;
		}
	}

	return true;
}

void FSaccadicWorker::CallInitNetCV()
{
	// Init the Msinet model
	if (m_funcInitNetCV == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("Function Not Loaded From DLL: InitNet"));
	}

	FString ModelPath = FPaths::ProjectDir() + FString(m_modulePath);
	std::string ModelStr = std::string(TCHAR_TO_UTF8(*ModelPath));
	UE_LOG(LogTemp, Warning, TEXT("Model path: %s"), *ModelPath);

	bool FunctionDone = m_funcInitNetCV(ModelStr.c_str());

	if (!FunctionDone)
	{
		UE_LOG(LogTemp, Error, TEXT("Libtorch not connected"));
	}
}

#pragma endregion
// The code below will run on the new thread.


bool FSaccadicWorker::Init()
{
	bRunThread = true;
	bInputReady = false;
	bOutputReady = false;

	m_ViewData.AddUninitialized(kImageSize * kImageSize * 4);
	m_BiasData.AddUninitialized(kImageSize * kImageSize * 4);
	m_SalmapData.AddUninitialized(kImageSize * kImageSize * 4);
	m_SalientWeight = 1.0f;
	m_FixationCoords = new int[2];

	// Return false if you want to abort the thread
	return true;
}

uint32 FSaccadicWorker::Run()
{
	while (bRunThread)
	{
		if (bInputReady && !bOutputReady)
		{
			// Do the thing
			if (m_debugMode) {
				// With saliency map 
				bool FunctionDone = m_funcGetFixationSalmapCV(
					(unsigned char*)m_ViewData.GetData(),
					(unsigned char*)m_BiasData.GetData(),
					m_SalientWeight,
					(unsigned char*)m_SalmapData.GetData(),
					m_FixationCoords);
			}
			else
			{
				// Without saliency map
				bool FunctionDone = m_funcGetFixationCV(
					(unsigned char*)m_ViewData.GetData(),
					(unsigned char*)m_BiasData.GetData(),
					m_SalientWeight,
					m_FixationCoords);
			}

			// Do this once you've finished using the input/output variables
			// From this point onwards, do not touch them!
			bInputReady = false;
			bOutputReady = true;

			// I hear that it's good to let the thread sleep a bit, so the OS can utilise it better or something.
			FPlatformProcess::Sleep(0.01f);
		}
	}

	return 0;
}

void FSaccadicWorker::Stop()
{
	bRunThread = false;
}
