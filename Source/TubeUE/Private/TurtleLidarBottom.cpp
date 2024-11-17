#include "TurtleLidarBottom.h"

// Sets default values
ATurtleLidarBottom::ATurtleLidarBottom(){
	this->sBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurtlebotLidarBottom"));
	RootComponent = this->sBodyMesh;
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATurtleLidarBottom::BeginPlay(){
	Super::BeginPlay();
	this->sBodyMesh->SetMobility(EComponentMobility::Movable);
	this->sBodyMesh->SetSimulatePhysics(true);
}

// Called every frame
void ATurtleLidarBottom::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

