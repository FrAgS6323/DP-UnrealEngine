#include "TurtleWheel.h"

// Sets default values
ATurtleWheel::ATurtleWheel(){
	PrimaryActorTick.bCanEverTick = true;
	this->sWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurtleWheel"));
	RootComponent = this->sWheelMesh;

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Models/TurtleBot/TurtlebotLidarTop"));
	UStaticMesh* MeshAsset = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Models/TurtleBot/TurtlebotWheelLeft")));
	if (MeshAsset)
	{
		this->sWheelMesh->SetStaticMesh(MeshAsset);
		UE_LOG(LogTemp, Warning, TEXT("Mesh loaded successfully: %s"), *MeshAsset->GetName());
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Mesh not loaded!"));
	}
}

// Called when the game starts or when spawned
void ATurtleWheel::BeginPlay(){
	Super::BeginPlay();
	this->sWheelMesh->SetMobility(EComponentMobility::Movable);
	this->sWheelMesh->SetSimulatePhysics(true);
}

// Called every frame
void ATurtleWheel::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}