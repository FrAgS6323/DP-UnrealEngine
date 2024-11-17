#include "TurtleLidarTop.h"

// Sets default values
ATurtleLidarTop::ATurtleLidarTop(){
	this->sBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurtlebotLidarTop"));
	RootComponent = this->sBodyMesh;
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATurtleLidarTop::BeginPlay(){
	Super::BeginPlay();
	this->sBodyMesh->SetMobility(EComponentMobility::Movable);
	this->sBodyMesh->SetSimulatePhysics(true);
}

// Called every frame
void ATurtleLidarTop::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}