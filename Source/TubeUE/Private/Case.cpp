#include "Case.h"
#include "../EngineHelper.h"

// Sets default values
ACase::ACase() {
	PrimaryActorTick.bCanEverTick = true;
	this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponentCase"));
	this->sCaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CaseMesh"));
	this->pointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("pCaseLight"));

	if (this->RootComponent){
		this->sCaseMesh->SetupAttachment(this->RootComponent);
		this->pointLight->SetupAttachment(this->sCaseMesh);
	}

	UEngineHelper::loadMeshDynamic(TEXT("/Game/Models/Case/Case"), this->sCaseMesh);
	
	this->sCaseMesh->SetRelativeRotation(FRotator(0, 90, 0));
	this->sCaseMesh->SetSimulatePhysics(true);
	this->sCaseMesh->SetEnableGravity(true);
	this->sCaseMesh->SetMobility(EComponentMobility::Movable);
	this->sCaseMesh->SetMassOverrideInKg(NAME_None, 10.0f, true);
	this->pointLight->SetRelativeLocation(FVector(0, 12, 0));
	this->pointLight->SetVisibility(false);
	this->pointLight->SetMobility(EComponentMobility::Movable);
	this->pointLight->SetCastShadows(false);
	this->pointLight->bAffectsWorld = true;
}

// Called when the game starts or when spawned
void ACase::BeginPlay(){
	Super::BeginPlay();
}

// Called every frame
void ACase::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	this->pointLight->SetVisibility(true);
	this->pointLight->SetIntensity(10000.0f);
	this->pointLight->SetLightColor(FLinearColor::Red);// Nastavenie jasu
}