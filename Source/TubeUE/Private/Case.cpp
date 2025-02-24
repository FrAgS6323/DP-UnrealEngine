#include "Case.h"
#include "../EngineHelper.h"

// Sets default values
ACase::ACase() {
	PrimaryActorTick.bCanEverTick = true;
	this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponentCase"));
	this->sCaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CaseMesh"));
	//this->pointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("pCaseLight"));

	if (this->RootComponent){
		this->sCaseMesh->SetupAttachment(this->RootComponent);
		//this->pointLight->SetupAttachment(this->sCaseMesh);
	}

	UEngineHelper::loadMeshDynamic(TEXT("/Game/Models/Case/Case"), this->sCaseMesh);
	
	this->sCaseMesh->SetRelativeRotation(FRotator(0, 90, 0));
	this->sCaseMesh->SetSimulatePhysics(true);
	this->sCaseMesh->SetEnableGravity(true);
	this->sCaseMesh->SetMobility(EComponentMobility::Movable);
	this->sCaseMesh->SetMassOverrideInKg(NAME_None, 10.0f, true);
#if 0
	this->pointLight->SetRelativeLocation(FVector(11, 18, 1.25)); //12, 15, 2 (y, x, z)
	this->pointLight->SetVisibility(false);
	this->pointLight->SetMobility(EComponentMobility::Movable);
	this->pointLight->SetCastShadows(false);
	this->pointLight->bAffectsWorld = true;
#endif
}

// Called when the game starts or when spawned
void ACase::BeginPlay(){
	Super::BeginPlay();
}

// Called every frame
void ACase::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	this->bBlueButton ? UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/Materials/Glowing/GlowBlue"), this->sCaseMesh, 4) : UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/Materials/Blue"), this->sCaseMesh, 4);
	this->bRedButton ? UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/Materials/Glowing/GlowRed"), this->sCaseMesh, 6) : UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/Materials/Red"), this->sCaseMesh, 6);
	this->bGreenButton ? UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/Materials/Glowing/GlowGreen"), this->sCaseMesh, 7) : UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/Materials/Green"), this->sCaseMesh, 7);
	this->bYellowButton ? UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/Materials/Glowing/GlowYellow"), this->sCaseMesh, 5) : UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/Materials/Yellow"), this->sCaseMesh, 5);

#if 0
	this->pointLight->SetVisibility(true);
	this->pointLight->SetIntensity(50000.0f);
	this->pointLight->SetLightColor(FLinearColor::Red);
#endif
}