#pragma once
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "PID.h"
#include "RotateTube.h"
#include "WebHandler.h"
#include "Tube.generated.h"

enum class eRequestType { GET, POST };

UENUM(BlueprintType)
enum class ERunningModesTube : uint8
{
	SIMULATION      UMETA(DisplayName = "Simulation"),
	VISUALIZATION   UMETA(DisplayName = "Visualization"),
	SIM_IN_REAL_TIME UMETA(DisplayName = "Sim in Real Time")
};

UCLASS(Blueprintable)
class TUBEUE_API ATube : public AActor{
	GENERATED_BODY()
	private:
		bool bPidBallSwitch = false,
			 bPidBallSwitchViz = false,
			 bSetIdealBallPID = true,
			 bCoords = false,
			 bCoordsViz = false,
			 bIsActive = false,
			 bChangeMode = false,
			 bShowColliders = false;
		int xBallPos = 0, 
			yBallPos = 0,
			xBallPosViz = 0,
			yBallPosViz = 0;
		inline static constexpr double motorHeight = 7.42;
		inline static constexpr double halfTubeHeight = 23.0;
		inline static constexpr double height = 58.0f;
		inline static constexpr double minAngle = 0.0;
		inline static constexpr double maxAngle = 60.0;
		inline static constexpr double numRotationVel = 1.0;
		double setDesiredHeight = 0.0,
			   setDesiredHeightViz = 0.0,
			   ballP = 0.75, 
			   ballI = 0.25, 
			   ballD = 0.1, 
			   setAngle = 0.0,
			   setAngleViz = 0.0,
			   servoP = 18.0,
			   servoI = 8.0,
			   servoD = 16.0,
			saturationLimitBallMin = 0.0,
			saturationLimitBallMax = 5.0,
			saturationLimitServoMin = -1000.0,
			saturationLimitServoMax = 1000.0;
		bool bHit = true,
			 bPhysicsIsSet = false;
		double distance,
			   angle,
			   desiredHeight,
			   actualHeightViz;
		TArray<double> regHeights;
		ERunningModesTube mode = ERunningModesTube::SIMULATION;
		TObjectPtr<UStaticMeshComponent> sHolderMesh, 
										 sTubeMesh,
										 sBallMesh;
		TObjectPtr<UPhysicsConstraintComponent> tubeJoint;
		APlayerController* playerController;
		TObjectPtr<UWidgetSwitcher> widgetSwitcher;
		TSharedPtr<FJsonObject> responseObj;
		TUniquePtr<UPID> pidControllerBall, 
						 pidControllerServo;
		TUniquePtr<WebHandler> webHandlerGet, 
							   webHandlerPost;
		UCheckBox *pidCheckBox,
				  *pidCheckBoxSimIRL;
		UTextBlock *currentHeightViz,
			       *currentAngleViz,
			       *currentHeightVarSimIRL,
			       *currentAngleVarSimIRL,
			       *desiredHeightVarSimIRL,
			       *desiredAngleVarSimIRL;
		TFunction<void(FHttpRequestPtr request, FHttpResponsePtr response, bool connected)> onReqCompleteFunctorGet;
		TSharedPtr<TFunction<void(FHttpRequestPtr request, FHttpResponsePtr response, bool connected)>> onReqCompleteFunctorPost;
		void initialize();
		auto getDistance() -> double;
		auto getRegulationHeight() -> double;
		auto getBottomCoordsOfTube(double actualAngle) -> FVector;
		void performRaycast();
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
		void funcForWebHandlerGet(FHttpRequestPtr request, FHttpResponsePtr response, bool connected);
		void funcForWebHandlerPost(FHttpRequestPtr request, FHttpResponsePtr response, bool connected);
		void rotateWidgetToFaceCamera();
		UFUNCTION() void bindOnButtonSimClick();
		UFUNCTION() void bindOnButtonVizClick();
		UFUNCTION() void bindOnButtonSimIRLClick();
		UFUNCTION() void bindOnRegCheckboxChange(bool bIsChecked);
		UFUNCTION() void bindOnidealPIDCheckboxChange(bool bIsChecked);
		UFUNCTION() void bindOnCoordsCheckboxChange(bool bIsChecked);
		UFUNCTION() void bindOnTextCommitHeight(const FText& Text, ETextCommit::Type CommitType);
		UFUNCTION() void bindOnTextCommitAngle(const FText& Text, ETextCommit::Type CommitType);
		UFUNCTION() void bindOnTextCommitX(const FText& Text, ETextCommit::Type CommitType);
		UFUNCTION() void bindOnTextCommitY(const FText& Text, ETextCommit::Type CommitType);
		UFUNCTION() void bindOnTextCommitP(const FText& Text, ETextCommit::Type CommitType);
		UFUNCTION() void bindOnTextCommitI(const FText& Text, ETextCommit::Type CommitType);
		UFUNCTION() void bindOnTextCommitD(const FText& Text, ETextCommit::Type CommitType);
		UFUNCTION() void bindOnRegCheckboxChangeViz(bool bIsChecked);
		UFUNCTION() void bindOnCoordsCheckboxChangeViz(bool bIsChecked);
		UFUNCTION() void bindOnTextCommitHeightViz(const FText& Text, ETextCommit::Type CommitType);
		UFUNCTION() void bindOnTextCommitAngleViz(const FText& Text, ETextCommit::Type CommitType);
		UFUNCTION() void bindOnTextCommitXViz(const FText& Text, ETextCommit::Type CommitType);
		UFUNCTION() void bindOnTextCommitYViz(const FText& Text, ETextCommit::Type CommitType);
	protected:
		virtual void BeginPlay() override;
	public:	
		ATube();
		void setActive(APlayerController* inPlayerController, bool bActive);
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets") TObjectPtr<UWidgetComponent> widgetComponent;
		virtual void Tick(float DeltaTime) override;
};