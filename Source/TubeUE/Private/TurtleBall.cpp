#include "TurtleBall.h"

// Sets default values
ATurtleBall::ATurtleBall(){
	this->sBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurtlebotBall"));
	RootComponent = this->sBodyMesh;
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATurtleBall::BeginPlay(){
	Super::BeginPlay();
}

// Called every frame
void ATurtleBall::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}