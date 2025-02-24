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
		static void loadMaterialDynamic(const TCHAR* path, UStaticMeshComponent* mesh, int slot);
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
		static auto performRaycast(AActor *actor, 
								   FVector startOffset, 
								   FVector rayDirection,
								   TArray<UPrimitiveComponent*> meshesToExclude,
								   bool enableDebugRay,
								   double rayLength,
								   double& hitDistance) -> bool;

		static auto degToRad(double deg) -> double;

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