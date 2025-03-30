#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "EngineHelper.generated.h"

UCLASS()
class TUBEUE_API UEngineHelper : public UObject {
	GENERATED_BODY()
	public:
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

		template<typename T>
		static auto spawnActor(UWorld* world,
							   FName name,
							   FVector location,
							   FRotator rotation,
							   AActor* owner) -> T* {
			if(!world) return nullptr;
			location = owner ? owner->GetActorLocation() + location : location;
			rotation = owner ? owner->GetActorRotation() + rotation : rotation;

			FActorSpawnParameters spawnParams;
			spawnParams.Owner = owner;
			spawnParams.Instigator = owner ? owner->GetInstigator() : nullptr;
			spawnParams.Name = name;

			return world->SpawnActor<T>(T::StaticClass(), location, rotation, spawnParams);
		}	
};