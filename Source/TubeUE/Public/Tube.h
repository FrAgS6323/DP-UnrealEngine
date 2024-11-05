#pragma once
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "RotateTube.h"
#include "Tube.generated.h"

UCLASS(Blueprintable)
class TUBEUE_API ATube : public AActor{

	GENERATED_BODY()
	private:
		inline static constexpr double motorHeight = 7.42;
		inline static constexpr double halfTubeHeight = 23.0;
		double distance;
		UPROPERTY(VisibleAnywhere) class UStaticMeshComponent* sTubeMesh;
		void performRaycast();
	protected:
		virtual void BeginPlay() override;
	public:	
		ATube();
		UPROPERTY(EditAnywhere) float angle;
		UPROPERTY(EditAnywhere, Category = "Desired height") float desiredHeight;
		double getDistance();
		virtual void Tick(float DeltaTime) override;
};
