#pragma once
#include "CoreMinimal.h"
#include "TurtleLidarTop.h"
#include "TurtleLidarBottom.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Engine/World.h"
#include "TurtleBody.generated.h"


UCLASS(Blueprintable)
class TUBEUE_API ATurtleBody : public APawn{
	GENERATED_BODY()
	private:
		bool bIsActive = false, bLidar;
		inline static double wheelRotationVel = 1000.0f;
		inline static double lidarRotationVel = 1500.0f;
		APlayerController* playerController;
		TObjectPtr<UStaticMeshComponent> sBodyMesh;
		TObjectPtr<UStaticMeshComponent> sLeftWheelMesh;
		TObjectPtr<UStaticMeshComponent> sRightWheelMesh;
		TObjectPtr<UStaticMeshComponent> sLeftBallMesh;
		TObjectPtr<UStaticMeshComponent> sRightBallMesh;
		TObjectPtr<UStaticMeshComponent> sLidarBottomMesh;
		TObjectPtr<UStaticMeshComponent> sLidarTopMesh;
		TObjectPtr<UPhysicsConstraintComponent> leftWheelJoint;
		TObjectPtr<UPhysicsConstraintComponent> rightWheelJoint;
		TObjectPtr<UPhysicsConstraintComponent> leftBallJoint;
		TObjectPtr<UPhysicsConstraintComponent> rightBallJoint;
		TObjectPtr<UPhysicsConstraintComponent> lidarBottomJoint;
		TObjectPtr<UPhysicsConstraintComponent> lidarTopJoint;
		void initialize();
		void moveFB(float value);
		void moveLR(float value);
		void moveRobot(float value);
		void turnLidar(double speed, bool bRaycast);
		void rotateWheel(UStaticMeshComponent* wheel, double direction);
	protected:
		virtual void BeginPlay() override;
	public:	
		ATurtleBody();
		void setActive(APlayerController* playerController, bool bActive);
		virtual void Tick(float DeltaTime) override;
};