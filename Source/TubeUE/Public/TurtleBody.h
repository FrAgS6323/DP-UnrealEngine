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
		inline static double wheelRotationVel = 1000.0f;
		inline static double lidarRotationVel = 1500.0f;
		//UPROPERTY(VisibleAnywhere, Category = "Meshes")
		TObjectPtr<UStaticMeshComponent> sBodyMesh;
		TObjectPtr<UStaticMeshComponent> sLeftWheelMesh;
		TObjectPtr<UStaticMeshComponent> sRightWheelMesh;
		TObjectPtr<UStaticMeshComponent> sLeftBallMesh;
		TObjectPtr<UStaticMeshComponent> sRightBallMesh;
		TObjectPtr<UStaticMeshComponent> sLidarBottomMesh;
		TObjectPtr<UStaticMeshComponent> sLidarTopMesh;
		//UPROPERTY(VisibleAnywhere, Category = "Joints")
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
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
	public:	
		ATurtleBody();
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") bool bForward;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") bool bBackward;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") bool bTurnLeft;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") bool bTurnRight;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lidar") bool bTurnLidar;
		void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
		//virtual void OnConstruction(const FTransform& Transform) override;
		virtual void Tick(float DeltaTime) override;
};