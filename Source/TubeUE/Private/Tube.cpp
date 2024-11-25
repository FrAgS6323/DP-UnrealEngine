#include "Tube.h"

ATube::ATube(){
	PrimaryActorTick.bCanEverTick = true;
	this->sTubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MergedTubeMesh"));
	RootComponent = this->sTubeMesh;
}

void ATube::BeginPlay(){
	Super::BeginPlay();
    this->sTubeMesh->SetMobility(EComponentMobility::Movable);
    this->sTubeMesh->SetSimulatePhysics(true);
    this->sTubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
#if 0
	FVector meshBounds = this->sTubeMesh->Bounds.BoxExtent;
	FVector centerOfMassOffset = FVector(0.0f, 0.0f, -meshBounds.Z);

	this->sTubeMesh->SetCenterOfMass(centerOfMassOffset, NAME_None);
#endif
}

double ATube::getDistance() {
	return (this->distance > 0.001) ? this->distance : 0;
}

void ATube::performRaycast() {
    FVector startVec = FVector(GetActorLocation().X,
                               GetActorLocation().Y,
                               GetActorLocation().Z - ATube::halfTubeHeight),
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

    if (bIsHit) {
        //UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *hitResult.GetActor()->GetName());

        this->distance = hitResult.Distance;
        //UE_LOG(LogTemp, Warning, TEXT("Hit Distance: %f"), this->distance);
        //DrawDebugLine(GetWorld(), startVec, endVec, FColor::Red, false, 1, 0, 1);
        // 
        DrawDebugPoint(GetWorld(), hitResult.Location, 10, FColor::Green, false, 1);
    }
    else {
        DrawDebugLine(GetWorld(), startVec, endVec, FColor::Red, false, 1, 0, 1);
    }
}

void ATube::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
    //this->performRaycast();
}