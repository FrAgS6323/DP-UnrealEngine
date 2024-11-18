#include "TurtleLidarBottom.h"

// Sets default values
ATurtleLidarBottom::ATurtleLidarBottom(){
	this->sBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurtlebotLidarBottom"));
	RootComponent = this->sBodyMesh;
	PrimaryActorTick.bCanEverTick = true;

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Models/TurtleBot/TurtlebotLidarTop"));
	UStaticMesh* MeshAsset = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Models/TurtleBot/TurtlebotLidarBottom")));
	if (MeshAsset)
	{
		this->sBodyMesh->SetStaticMesh(MeshAsset);
		UE_LOG(LogTemp, Warning, TEXT("Mesh loaded successfully: %s"), *MeshAsset->GetName());
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Mesh not loaded!"));
	}
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