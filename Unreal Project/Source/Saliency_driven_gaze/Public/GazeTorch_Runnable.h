// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RHICommandList.h"

#include "SaccadicWorker.h"

#include "GazeTorch_Runnable.generated.h"

// Our delegate to return fixation value
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFixationDelegate, FVector2D, FixationOut, AActor*, ActorCaller);


UCLASS()
class SALIENCY_DRIVEN_GAZE_API AGazeTorch_Runnable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGazeTorch_Runnable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game ends or when destroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Saliency")
	void ComputeFixation(UTextureRenderTarget2D* AgentView,
		UTextureRenderTarget2D* BiasMap, float SalientWeight);

	UPROPERTY(BlueprintAssignable, Category = "Saliency")
	FFixationDelegate OnFixationComputed;

	UFUNCTION(BlueprintCallable, Category = "Saliency")
	bool IsMyTurn(AActor* actor);

	UFUNCTION(BlueprintCallable, Category = "Saliency")
	UTexture2D* GetSaliencyMap();
	
private:
	bool m_debugMode = true;
	FSaccadicWorker* m_SaccadicWorker;
	FVector2D m_FixationOut;

	TArray<AActor*> m_actorRequest;

	UTextureRenderTarget2D* m_BiasMap;
	UTexture2D* m_salmap;

	bool m_deadFrame = false;
};
