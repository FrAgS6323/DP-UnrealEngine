#pragma once
#include <Windows.h>
#include <pdh.h>
#pragma comment(lib, "pdh.lib")
#include "RHI.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include <d3d12.h>
#include <dxgi1_4.h>
#include <dxgi1_6.h>
#include "Windows/HideWindowsPlatformTypes.h"
#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/BodySetup.h"
#include "EngineHelper.generated.h"

UCLASS()
class TUBEUE_API UEngineHelper : public UObject {
	GENERATED_BODY()
	public:
		class Profiler {
			private:
				inline static int32 sampleCount = 0;
				inline static float totalFPS = 0.f;
				inline static float totalCPU = 0.f;
				inline static float totalRAM = 0.f;
				inline static float totalUsedVRAM = 0.f;
				inline static float totalBudgetVRAM = 0.f;
				inline static float totalPercentVRAM = 0.f;
				inline static PDH_HQUERY cpuQuery;
				inline static PDH_HCOUNTER cpuTotal;
				static void profileFPS(bool bScreen, float delatTime, FColor color);
				static void profileRAM(bool bScreen, float delatTime, FColor color);
				static void profileCPU(bool bScreen, float delatTime, FColor color);
				static void profileGPU(bool bScreen, float delatTime, FColor color);
			public:
				static void initCPUprofiler();
				static void profilePerformance(bool bScreen, float deltaTime);
				static void writeAvgsToLOG();
		};
		static void loadMeshStatic(const TCHAR* path, UStaticMeshComponent* parentComp);
		static void loadMeshDynamic(const TCHAR* path, UStaticMeshComponent* parentComp);
		static auto loadMaterialDynamic(const TCHAR* path) -> UMaterialInterface*;
		static void setupConstraint(UPhysicsConstraintComponent* constraint, 
									USceneComponent* attachToComponent,
									UPrimitiveComponent* stillComponent,
									UPrimitiveComponent* moveableComponent,
									ELinearConstraintMotion xMotionType,
									float xLimit,
									ELinearConstraintMotion yMotionType,
									float yLimit,
									ELinearConstraintMotion zMotionType,
									float zLimit,
									EAngularConstraintMotion swing1MotionType, 
									float swing1LimitAngle,
									EAngularConstraintMotion swing2MotionType,
									float swing2LimitAngle,
									EAngularConstraintMotion twistMotionType,
									float twistLimitAngle);
		static auto performRaycast(UWorld *world, 
								   UStaticMeshComponent* mesh,
								   FVector startOffset, 
								   FVector rayDirection,
								   TArray<UPrimitiveComponent*> meshesToExclude,
								   bool enableDebugRay,
								   double rayLength,
								   double& hitDistance) -> bool;

		static auto degToRad(double deg) -> double;
		static auto radToDeg(double rad) -> double;
		static auto findActorsOfClass(const UObject *worldContextObject, 
									  TSubclassOf<AActor> actorClass, 
									  size_t index) -> TObjectPtr<AActor>;
		static void setSpectatorCameraSpeed(APlayerController* playerController, float speed);
		static void setKinematicTarget(UStaticMeshComponent* component, bool bState);
		static void drawAllSimpleCollidersForActor(AActor *actor);
};