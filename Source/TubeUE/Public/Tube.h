#pragma once
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PID.h"
#include "RotateTube.h"

#include "Tube.generated.h"

UCLASS(Blueprintable)
class TUBEUE_API ATube : public AActor{

	GENERATED_BODY()
	private:
		inline static constexpr double motorHeight = 7.42;
		inline static constexpr double halfTubeHeight = 23.0;
		double distance, 
			   angle, 
			   desiredHeight;
		TObjectPtr<UStaticMeshComponent> sTubeMesh;
		TObjectPtr<UStaticMeshComponent> sHolderMesh;
		TObjectPtr<UStaticMeshComponent> sBallMesh;
		TObjectPtr<UPhysicsConstraintComponent> tubeJoint;
		TObjectPtr<UPID> pid;
		void initialize();
		void performRaycast();
	protected:
		virtual void BeginPlay() override;
	public:	
		ATube();
		auto getDistance() -> double;
		virtual void Tick(float DeltaTime) override;
};
