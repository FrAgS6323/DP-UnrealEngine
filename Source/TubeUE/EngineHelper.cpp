#include "EngineHelper.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"

void UEngineHelper::loadMeshStatic(const TCHAR* path, UStaticMeshComponent* parentComp){
	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(path);

	if (meshAsset.Succeeded()){
		parentComp->SetStaticMesh(meshAsset.Object);
	}else{
		UE_LOG(LogTemp, Error, TEXT("Failed to load TurtlebotBody mesh!"));
	}
}

void UEngineHelper::loadMeshDynamic(const TCHAR* path, UStaticMeshComponent *parentComp){
	UStaticMesh* meshAsset = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, path));

	if (meshAsset) {
		UE_LOG(LogTemp, Warning, TEXT("Mesh loaded successfully (dynamic): %s"), *meshAsset->GetName());
		parentComp->SetStaticMesh(meshAsset);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Mesh not loaded!"));
	}
}

auto UEngineHelper::loadMaterialDynamic(const TCHAR* path) -> UMaterialInterface* {
	UMaterialInterface* mat = LoadObject<UMaterialInterface>(nullptr, path);

#if 0
	if (mat && mesh) {
		//UE_LOG(LogTemp, Warning, TEXT("Material loaded successfully (dynamic): %s"), *mat->GetName());
		mesh->SetMaterial(slot, mat);
	}
	else {
		//UE_LOG(LogTemp, Error, TEXT("Material not loaded!"));
	}
#endif
	return mat;
}

void UEngineHelper::setupConstraint(UPhysicsConstraintComponent* constraint,
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
									float twistLimitAngle){
	if (!constraint){
		UE_LOG(LogTemp, Error, TEXT("Constraint not initialized!"));
		return;
	}

	if (!stillComponent || !moveableComponent){
		UE_LOG(LogTemp, Error, TEXT("Constraint not set: One or both constrained components are NULL!"));
		return;
	}

	constraint->SetupAttachment(attachToComponent);
	constraint->SetConstrainedComponents(moveableComponent, 
										 NAME_None,
										 stillComponent,
										 NAME_None);
	constraint->SetLinearXLimit(xMotionType, xLimit);
	constraint->SetLinearYLimit(yMotionType, yLimit);
	constraint->SetLinearZLimit(zMotionType, zLimit);
	//constraint->SetLinearPositionDrive(true, true, true);
	//constraint->SetLinearDriveParams(10000.0f, 1000.0f, 0.0f);  // Stiffness, Damping, Force Limit
	//constraint->SetAngularDriveParams(5000.0f, 500.0f, 0.0f);

	constraint->SetAngularSwing1Limit(swing1MotionType, swing1LimitAngle);
	constraint->SetAngularSwing2Limit(swing2MotionType, swing2LimitAngle);
	constraint->SetAngularTwistLimit(twistMotionType, twistLimitAngle);
}

auto UEngineHelper::performRaycast(AActor *actor, 
								   FVector startOffset, 
								   FVector rayDirection,
								   TArray<UPrimitiveComponent*> meshesToExclude,
								   bool enableDebugRay,
								   double rayLength,
								   double &hitDistance) -> bool {
	FVector startVec = actor->GetActorLocation() + startOffset,
			endVec = ((rayDirection * rayLength) + startVec);

	FHitResult hitResult;
	FCollisionQueryParams collisionParams;

	meshesToExclude.IsEmpty() 
		? 
		collisionParams.AddIgnoredActor(actor) 
		: 
		collisionParams.AddIgnoredComponents(meshesToExclude);

	bool bIsHit = actor->GetWorld()->LineTraceSingleByChannel(hitResult,
															  startVec,
															  endVec,
															  ECC_Visibility,
															  collisionParams);

	if (bIsHit) {
		//UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *hitResult.GetActor()->GetName());

		hitDistance = hitResult.Distance;
		//UE_LOG(LogTemp, Warning, TEXT("Hit Distance: %f"), this->distance);
		//DrawDebugLine(GetWorld(), startVec, endVec, FColor::Red, false, 1, 0, 1);
		// 
		if(enableDebugRay) DrawDebugPoint(actor->GetWorld(), hitResult.Location, 10, FColor::Green, false, 1);
	}else{
		if(enableDebugRay) DrawDebugLine(actor->GetWorld(), startVec, endVec, FColor::Red, false, 1, 0, 1);
	}
	return bIsHit;
}

auto UEngineHelper::degToRad(double deg)->double {
		return (deg * PI) / 180;
}