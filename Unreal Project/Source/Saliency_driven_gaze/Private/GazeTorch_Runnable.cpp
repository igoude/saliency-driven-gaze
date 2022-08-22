// Fill out your copyright notice in the Description page of Project Settings.

#include "GazeTorch_Runnable.h"

// Sets default values
AGazeTorch_Runnable::AGazeTorch_Runnable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGazeTorch_Runnable::BeginPlay()
{
	Super::BeginPlay();
	
	m_SaccadicWorker = new FSaccadicWorker(m_debugMode);
	m_salmap = UTexture2D::CreateTransient(kImageSize, kImageSize, PF_B8G8R8A8);
}

// Called when the game starts or when spawned
void AGazeTorch_Runnable::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	delete m_SaccadicWorker;
}

// Called every frame
void AGazeTorch_Runnable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(m_SaccadicWorker->bOutputReady)
	{
		m_FixationOut.Set((float)m_SaccadicWorker->m_FixationCoords[0] / 255.0f,
						  (float)m_SaccadicWorker->m_FixationCoords[1] / 255.0f);

		this->OnFixationComputed.Broadcast(m_FixationOut, m_actorRequest[0]);

		m_actorRequest.RemoveAt(0);
		m_SaccadicWorker->bOutputReady = false;
	}

	if (m_deadFrame) {
		m_BiasMap->GameThread_GetRenderTargetResource()->ReadPixels(m_SaccadicWorker->m_BiasData);
		
		m_SaccadicWorker->bInputReady = true;
		m_deadFrame = false;
	}
}


void AGazeTorch_Runnable::ComputeFixation(
	UTextureRenderTarget2D* AgentView,
	UTextureRenderTarget2D* BiasMap,
	float SalientWeight)
{
	if (m_SaccadicWorker)
	{
		if (!m_SaccadicWorker->bInputReady && !m_SaccadicWorker->bOutputReady)
		{
			// Read and write the variables here
			AgentView->GameThread_GetRenderTargetResource()->ReadPixels(m_SaccadicWorker->m_ViewData);
			m_SaccadicWorker->m_SalientWeight = SalientWeight;	
			m_BiasMap = BiasMap;

			// Flick the switch to let the worker thread run
			// Do not read/write variables from thread from here on!
			m_deadFrame = true;
		}
	}
}


bool AGazeTorch_Runnable::IsMyTurn(AActor* actor)
{
	if (m_actorRequest.IsEmpty()) {
		m_actorRequest.Push(actor);
		return true;
	}

	int index = 0;
	if (m_actorRequest.Find(actor, index))
	{
		if (index == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		m_actorRequest.Push(actor);
		return false;
	}
}

UTexture2D* AGazeTorch_Runnable::GetSaliencyMap()
{
	// Write saliency map into a texture
	FTexture2DMipMap& Mip = m_salmap->PlatformData->Mips[0];
	void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(Data, m_SaccadicWorker->m_SalmapData.GetData(), 4 * kImageSize * kImageSize);
	Mip.BulkData.Unlock();
	m_salmap->UpdateResource();

	return m_salmap;
}

