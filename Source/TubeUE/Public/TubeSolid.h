#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotateTube.h"
#include "TubeSolid.generated.h"

UCLASS()
class TUBEUE_API ATubeSolid : public AActor{
	GENERATED_BODY()
	private:
		const double motorHeight = 7.42f;
		double distance;
		UPROPERTY(EditAnywhere) class UStaticMeshComponent* tubeSolidMesh;
		void performRaycast();
	protected:
		virtual void BeginPlay() override;
	public:	
		ATubeSolid();
		UPROPERTY(EditAnywhere) float angle;
		UPROPERTY(EditAnywhere, Category = "Desired height") float desiredHeight;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") URotateTube* rotateTubeComponent;
		double getDistance();
		virtual void Tick(float DeltaTime) override;
};
