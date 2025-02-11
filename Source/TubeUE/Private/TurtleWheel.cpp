#include "TurtleWheel.h"
#include "../EngineHelper.h"

// Sets default values
ATurtleWheel::ATurtleWheel(){
    UWorld* world = GetWorld();

    PrimaryActorTick.bCanEverTick = true;
    this->sWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurtleWheelLeft"));
    UEngineHelper::loadMeshStatic(TEXT("/Game/Models/TurtleBot/TurtlebotWheelLeft"), this->sWheelMesh);

    if (this->sWheelMesh) {
        RootComponent = this->sWheelMesh;
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("RootComponent not initialized!"));
    }
}

// Called when the game starts or when spawned
void ATurtleWheel::BeginPlay(){
	Super::BeginPlay();
	this->sWheelMesh->SetMobility(EComponentMobility::Movable);
	this->sWheelMesh->SetSimulatePhysics(false);
}

// Called every frame
void ATurtleWheel::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}