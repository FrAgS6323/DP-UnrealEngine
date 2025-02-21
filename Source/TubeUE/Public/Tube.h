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
		double distance;
		TObjectPtr<UStaticMeshComponent> sTubeMesh;
		TObjectPtr<UStaticMeshComponent> sHolderMesh;
		TObjectPtr<UStaticMeshComponent> sBallMesh;
		TObjectPtr<UPhysicsConstraintComponent> tubeJoint;
		UPID* pidController;
		void initialize();
		void performRaycast();
		auto getDistance() -> double;
		auto getRegulationHeight() -> double;
		void PIDreg(float deltaTime);
	protected:
		virtual void BeginPlay() override;
	public:	
		ATube();
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PID") bool bPidSwitch;
		UPROPERTY(EditAnywhere, Category = "PID") bool bSetIdealPID;
		UPROPERTY(EditAnywhere, Category = "PID") double P;
		UPROPERTY(EditAnywhere, Category = "PID") double I;
		UPROPERTY(EditAnywhere, Category = "PID") double D;
		UPROPERTY(EditAnywhere, Category = "Settings") double angle = 45.0;
		UPROPERTY(EditAnywhere, Category = "Settings") double desiredHeight;
		//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joint") class UPhysicsConstraintComponent* tubeJoint;
		virtual void Tick(float DeltaTime) override;
};