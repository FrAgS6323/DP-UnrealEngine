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
		inline static constexpr double height = 58.0f;
		inline static constexpr double minAngle = 0.0;
		inline static constexpr double maxAngle = 60.0;
		inline static constexpr double numRotationVel = 1.0;
		bool bHit = true;
		double distance, angle, desiredHeight;
		//size_t count = 0;
		TObjectPtr<UStaticMeshComponent> sTubeMesh;
		TObjectPtr<UStaticMeshComponent> sHolderMesh;
		TObjectPtr<UStaticMeshComponent> sBallMesh;
		TObjectPtr<UPhysicsConstraintComponent> tubeJoint;
		UPID *pidControllerBall, 
			 *pidControllerServo;
		void initialize();
		void performRaycast();
		auto getDistance() -> double;
		auto getRegulationHeight() -> double;
		auto splitBallForce(const double& pidOutput, const double& angle) -> FVector;
		void convertXYToHeightAngle(double x,
									double y,
									double& outHeight,
									double& outAngle);
		void convertHeightAngleToXY(double inHeight,
									double inAngle,
									double& xOut,
									double& yOut);
		void PIDBall(float deltaTime);
		void PIDServo(float deltaTime);
		void rotate(double inAngleDeg);
	protected:
		virtual void BeginPlay() override;
	public:	
		ATube();
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PID Ball") bool bPidBallSwitch;
		UPROPERTY(EditAnywhere, Category = "PID Ball") bool bSetIdealBallPID;
		UPROPERTY(EditAnywhere, Category = "PID Ball") double ballP = 0.75;
		UPROPERTY(EditAnywhere, Category = "PID Ball") double ballI = 0.25;
		UPROPERTY(EditAnywhere, Category = "PID Ball") double ballD = 0.1;
		UPROPERTY(EditAnywhere, Category = "PID Servo") bool bSetIdealServoPID;
		UPROPERTY(EditAnywhere, Category = "PID Servo") double servoP = 18.0;
		UPROPERTY(EditAnywhere, Category = "PID Servo") double servoI = 8.0;
		UPROPERTY(EditAnywhere, Category = "PID Servo") double servoD = 16.0;
		UPROPERTY(EditAnywhere, Category = "XY pos") bool bCoords = false;
		UPROPERTY(EditAnywhere, Category = "XY pos") int xBallPos = 0;
		UPROPERTY(EditAnywhere, Category = "XY pos") int yBallPos = 0;
		UPROPERTY(EditAnywhere, Category = "Settings") double setAngle = 45.0;
		UPROPERTY(EditAnywhere, Category = "Settings") double setDesiredHeight = 20.0;
		UPROPERTY(EditAnywhere, Category = "Settings") double saturationLimitBallMin = 0.0;
		UPROPERTY(EditAnywhere, Category = "Settings") double saturationLimitBallMax = 5.0;
		UPROPERTY(EditAnywhere, Category = "Settings") double saturationLimitServoMin = -1000.0;
		UPROPERTY(EditAnywhere, Category = "Settings") double saturationLimitServoMax = 1000.0;
		//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joint") class UPhysicsConstraintComponent* tubeJoint;
		virtual void Tick(float DeltaTime) override;
};