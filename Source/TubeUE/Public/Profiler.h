#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Profiler.generated.h"

UCLASS()
class TUBEUE_API AProfiler : public AActor{
	GENERATED_BODY()
	public:	
		AProfiler();
	protected:
		virtual void BeginPlay() override;
	public:	
		virtual void Tick(float DeltaTime) override;
		void EndPlay(const EEndPlayReason::Type EndPlayReason);
};
