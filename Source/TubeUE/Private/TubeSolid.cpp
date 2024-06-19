#include "TubeSolid.h"

ATubeSolid::ATubeSolid(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->tubeSolidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TubeSolidMesh"));
	RootComponent = this->tubeSolidMesh;
	this->rotateTubeComponent = CreateDefaultSubobject<URotateTube>(TEXT("RotateTubeComponent"));

	this->tubeSolidMesh->SetSimulatePhysics(true);
}

void ATubeSolid::BeginPlay(){
	Super::BeginPlay();

	FVector meshBounds = this->tubeSolidMesh->Bounds.BoxExtent;
	FVector centerOfMassOffset = FVector(0.0f, 0.0f, -meshBounds.Z);

	this->tubeSolidMesh->SetCenterOfMass(centerOfMassOffset, NAME_None);
}

double ATubeSolid::getDistance(){
    return (this->distance > 0.001) ? this->distance : 0;
}

void ATubeSolid::performRaycast(){
    FVector startVec = FVector(GetActorLocation().X,
                               GetActorLocation().Y, 
                               GetActorLocation().Z + this->motorHeight),
            upVec = GetActorUpVector(),
            endVec = ((upVec * 50.0f) + startVec);

    FHitResult hitResult;
    FCollisionQueryParams collisionParams;
    collisionParams.AddIgnoredActor(this);

    bool bIsHit = GetWorld()->LineTraceSingleByChannel(hitResult,
                                                       startVec,
                                                       endVec,
                                                       ECC_Visibility,
                                                       collisionParams);

    if (bIsHit){
        //UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *hitResult.GetActor()->GetName());
        
        this->distance = hitResult.Distance;
        UE_LOG(LogTemp, Warning, TEXT("Hit Distance: %f"), this->distance);
        //DrawDebugLine(GetWorld(), startVec, endVec, FColor::Red, false, 1, 0, 1);
        //DrawDebugPoint(GetWorld(), hitResult.Location, 10, FColor::Red, false, 1);
    }else{
        DrawDebugLine(GetWorld(), startVec, endVec, FColor::Green, false, 1, 0, 1);
    }
}

void ATubeSolid::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
    this->performRaycast();
	//if (this->rotateTubeComponent){
		//this->rotateTubeComponent->rotate(this->angle);
	//}
}