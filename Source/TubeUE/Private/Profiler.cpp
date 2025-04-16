#include "Profiler.h"
#include "../EngineHelper.h"

AProfiler::AProfiler(){
	PrimaryActorTick.bCanEverTick = true;
}

void AProfiler::BeginPlay(){
	Super::BeginPlay();
	UEngineHelper::Profiler::initCPUprofiler();
}

void AProfiler::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	UEngineHelper::Profiler::profilePerformance(true, DeltaTime);
}

void AProfiler::EndPlay(const EEndPlayReason::Type EndPlayReason){
	Super::EndPlay(EndPlayReason);
	UEngineHelper::Profiler::writeAvgsToLOG();
}
