#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurtleWheel.generated.h"

UCLASS(Blueprintable)
class TUBEUE_API ATurtleWheel : public AActor
{
	GENERATED_BODY()
	private:
		UPROPERTY(VisibleAnywhere, Category = "Components") class UStaticMeshComponent* sWheelMesh;
	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

	public:	
		ATurtleWheel();
		
		virtual void Tick(float DeltaTime) override;

};
