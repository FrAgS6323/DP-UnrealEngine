#include "Case.h"
#include "../EngineHelper.h"

ACase::ACase() {
	PrimaryActorTick.bCanEverTick = true;
	this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponentCase"));
	this->sCaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CaseMesh"));
	
	this->blueButton = {false, 42, UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/GlowMaterials/ButtonBlue")), UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/GlowMaterials/Glowing/GlowBlue")) };
	this->greenButton = { false, 43, UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/GlowMaterials/ButtonGreen")), UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/GlowMaterials/Glowing/GlowGreen")) };
	this->redButton = { false, 44, UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/GlowMaterials/ButtonRed")), UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/GlowMaterials/Glowing/GlowRed")) };
	this->yellowButton = { false, 45, UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/GlowMaterials/ButtonYellow")), UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/GlowMaterials/Glowing/GlowYellow")) };
	this->leftGreenLed = {false, 46,  UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/GlowMaterials/1DownGreenSvietidlo")), UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/GlowMaterials/Glowing/GlowGreen")) };
	this->middleGreenLed = {false, 47, UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/GlowMaterials/2DownGreenSvietidlo")), UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/GlowMaterials/Glowing/GlowGreen")) };
	this->rightGreenLed = {false, 48, UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/GlowMaterials/3DownGreenSvietidlo")), UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/GlowMaterials/Glowing/GlowGreen")) };
	this->leftRedKnob = {false, 50, UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/GlowMaterials/RedLeftKnob")), UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/GlowMaterials/Glowing/GlowRed")) };
	this->rightRedKnob = {false, 49, UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/GlowMaterials/RedRightKnob")), UEngineHelper::loadMaterialDynamic(TEXT("/Game/Models/Case/GlowMaterials/Glowing/GlowRed")) };

	this->initSegArray(this->sevenSegOne, TArray<int>{24, 28, 27, 29, 30, 31, 32, 33}, TEXT("/Game/Models/Case/Materials/Default7Seg"), TEXT("/Game/Models/Case/GlowMaterials/Glowing/GlowSegment"));
	this->initSegArray(this->sevenSegTwo, TArray<int>{34, 35, 37, 38, 39, 40, 41, 36}, TEXT("/Game/Models/Case/Materials/Default7Seg"), TEXT("/Game/Models/Case/GlowMaterials/Glowing/GlowSegment"));

	this->initDisplay(this->displayUp, 
					  TArray<TArray<int>>{{54, 55, 56, 57, 58, 59, 60}, 
										  {61, 62, 63, 64, 65, 66, 67}, 
										  {68, 69, 70, 71, 72, 73, 74}, 
										  {75, 76, 77, 53, 78, 79, 80}, 
										  {81, 82, 83, 84, 85, 86, 87}}, 
					   TEXT("/Game/Models/Case/Materials/DefaultDisplay"), 
					   TEXT("/Game/Models/Case/GlowMaterials/Glowing/GlowDisplayRed"));

	this->initDisplay(this->displayDown,
					  TArray<TArray<int>>{{88, 89, 90, 5, 92, 93, 94},
										  {95, 96, 97, 98, 99, 100, 101},
										  {102, 103, 104, 105, 106, 107, 108},
										  {109, 110, 111, 112, 113, 114, 115},
										  {116, 117, 118, 119, 120, 121, 122}},
					  TEXT("/Game/Models/Case/Materials/DefaultDisplay"),
					  TEXT("/Game/Models/Case/GlowMaterials/Glowing/GlowDisplayGreen"));

	if (this->RootComponent)
		this->sCaseMesh->SetupAttachment(this->RootComponent);

	UEngineHelper::loadMeshDynamic(TEXT("/Game/Models/Case/Case"), this->sCaseMesh);
	
	this->sCaseMesh->SetRelativeRotation(FRotator(0, 90, 0));
	this->sCaseMesh->SetSimulatePhysics(true);
	this->sCaseMesh->SetEnableGravity(true);
	this->sCaseMesh->SetMobility(EComponentMobility::Movable);
	this->sCaseMesh->SetMassOverrideInKg(NAME_None, 10.0f, true);

	this->webHandler = new WebHandler(TEXT("http://147.232.60.231:5001/kuforDataAPI"), WebHandler::eRequestType::GET);
}

void ACase::funcForWebHandler(FHttpRequestPtr request, FHttpResponsePtr response, bool connected) {
	this->reqData.request = request;
	this->reqData.response = response;
	this->reqData.connected = connected;

#if 0
	TSharedPtr<FJsonObject> responseObj;
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(response->GetContentAsString());
	FJsonSerializer::Deserialize(jsonReader, responseObj);

	//UE_LOG(LogTemp, Warning, TEXT("response: %s"), *this->reqData.response->GetContentAsString());
	UE_LOG(LogTemp, Warning, TEXT("parsed-> target: %s"), *responseObj->GetStringField("target"));
#endif

}

void ACase::initSegArray(TArray<MatElementData>& segArray, 
						 const TArray<int>& slots, 
						 const TCHAR* offMatPath,
						 const TCHAR* onMatPath){
	for(const int &slot : slots)
		segArray.Add({ false, slot, UEngineHelper::loadMaterialDynamic(offMatPath), UEngineHelper::loadMaterialDynamic(onMatPath) });
}

void ACase::initDisplay(TArray<TArray<MatElementData>> &display, 
					    const TArray<TArray<int>> &slots, 
						const TCHAR* offMatPath,
						const TCHAR* onMatPath){
	for (size_t i = 0; i < ACase::displaySize; ++i) {
		TArray<MatElementData> segArray;

		this->initSegArray(segArray, slots[i], offMatPath, onMatPath);
		display.Add(segArray);
	}
}

auto ACase::numToDigits(const int& num) -> TArray<int> {
	TArray<int> digits;
	std::string str = std::to_string(std::abs(num));

	for (char c : str)
		digits.Add(c - '0');
	return digits;
}

void ACase::numToSegments(const int &num, TArray<MatElementData> &segArray){
	switch (num){
		case 0: {
			segArray[0].bSwitch = true;
			segArray[1].bSwitch = true;
			segArray[2].bSwitch = true;
			segArray[3].bSwitch = true;
			segArray[4].bSwitch = true;
			segArray[5].bSwitch = true;
			segArray[6].bSwitch = false;
		}
			break;
		case 1: {
			segArray[0].bSwitch = false;
			segArray[1].bSwitch = true;
			segArray[2].bSwitch = true;
			segArray[3].bSwitch = false;
			segArray[4].bSwitch = false;
			segArray[5].bSwitch = false;
			segArray[6].bSwitch = false;
		}
			break;
		case 2: {
			segArray[0].bSwitch = true;
			segArray[1].bSwitch = true;
			segArray[2].bSwitch = false;
			segArray[3].bSwitch = true;
			segArray[4].bSwitch = true;
			segArray[5].bSwitch = false;
			segArray[6].bSwitch = true;
		}
			  break;
		case 3: {
			segArray[0].bSwitch = true;
			segArray[1].bSwitch = true;
			segArray[2].bSwitch = true;
			segArray[3].bSwitch = true;
			segArray[4].bSwitch = false;
			segArray[5].bSwitch = false;
			segArray[6].bSwitch = true;
		}
			  break;
		case 4: {
			segArray[0].bSwitch = false;
			segArray[1].bSwitch = true;
			segArray[2].bSwitch = true;
			segArray[3].bSwitch = false;
			segArray[4].bSwitch = false;
			segArray[5].bSwitch = true;
			segArray[6].bSwitch = true;
		}
			  break;
		case 5: {
			segArray[0].bSwitch = true;
			segArray[1].bSwitch = false;
			segArray[2].bSwitch = true;
			segArray[3].bSwitch = true;
			segArray[4].bSwitch = false;
			segArray[5].bSwitch = true;
			segArray[6].bSwitch = true;
		}
			  break;
		case 6: {
			segArray[0].bSwitch = true;
			segArray[1].bSwitch = false;
			segArray[2].bSwitch = true;
			segArray[3].bSwitch = true;
			segArray[4].bSwitch = true;
			segArray[5].bSwitch = true;
			segArray[6].bSwitch = true;
		}
			  break;
		case 7: {
			segArray[0].bSwitch = true;
			segArray[1].bSwitch = true;
			segArray[2].bSwitch = true;
			segArray[3].bSwitch = false;
			segArray[4].bSwitch = false;
			segArray[5].bSwitch = false;
			segArray[6].bSwitch = false;
		}
			  break;
		case 8: {
			segArray[0].bSwitch = true;
			segArray[1].bSwitch = true;
			segArray[2].bSwitch = true;
			segArray[3].bSwitch = true;
			segArray[4].bSwitch = true;
			segArray[5].bSwitch = true;
			segArray[6].bSwitch = true;
		}
			  break;
		case 9: {
			segArray[0].bSwitch = true;
			segArray[1].bSwitch = true;
			segArray[2].bSwitch = true;
			segArray[3].bSwitch = true;
			segArray[4].bSwitch = false;
			segArray[5].bSwitch = true;
			segArray[6].bSwitch = true;
		}
			  break;
		case -1: {
			segArray[0].bSwitch = false;
			segArray[1].bSwitch = false;
			segArray[2].bSwitch = false;
			segArray[3].bSwitch = false;
			segArray[4].bSwitch = false;
			segArray[5].bSwitch = false;
			segArray[6].bSwitch = false;
		}
			  break;
	}
}

void ACase::numToDisplay(const int& num, TArray<TArray<MatElementData>>& display){
	TArray<int> digits = this->numToDigits(num);
	int32 numOfEmptyDigits = ACase::displaySize - digits.Num();

	for (size_t emptyIdx = 0; emptyIdx < numOfEmptyDigits; emptyIdx++)
		this->numToSegments(-1, display[emptyIdx]);

	for (int32 i = digits.Num() - 1; i >= 0; i--)
		this->numToSegments(digits[i], display[i + numOfEmptyDigits]);
}

void ACase::renderSegArray(const TArray<MatElementData>& segArray){
	for (const auto& segment : segArray)
		segment.bSwitch ? this->sCaseMesh->SetMaterial(segment.slot, segment.matOn) : this->sCaseMesh->SetMaterial(segment.slot, segment.matOff);
}

void ACase::renderDisplay(const TArray<TArray<MatElementData>>& display){
	for (const auto& segArray : display)
		this->renderSegArray(segArray);
}

void ACase::renderMaterials(){
	this->bBlueButton ? this->sCaseMesh->SetMaterial(this->blueButton.slot, this->blueButton.matOn) : this->sCaseMesh->SetMaterial(this->blueButton.slot, this->blueButton.matOff);
	this->bRedButton ? this->sCaseMesh->SetMaterial(this->redButton.slot, this->redButton.matOn) : this->sCaseMesh->SetMaterial(this->redButton.slot, this->redButton.matOff);
	this->bGreenButton ? this->sCaseMesh->SetMaterial(this->greenButton.slot, this->greenButton.matOn) : this->sCaseMesh->SetMaterial(this->greenButton.slot, this->greenButton.matOff);
	this->bYellowButton ? this->sCaseMesh->SetMaterial(this->yellowButton.slot, this->yellowButton.matOn) : this->sCaseMesh->SetMaterial(this->yellowButton.slot, this->yellowButton.matOff);
	this->bLeftGreenLed ? this->sCaseMesh->SetMaterial(this->leftGreenLed.slot, this->leftGreenLed.matOn) : this->sCaseMesh->SetMaterial(this->leftGreenLed.slot, this->leftGreenLed.matOff);
	this->bMiddleGreenLed ? this->sCaseMesh->SetMaterial(this->middleGreenLed.slot, this->middleGreenLed.matOn) : this->sCaseMesh->SetMaterial(this->middleGreenLed.slot, this->middleGreenLed.matOff);
	this->bRightGreenLed ? this->sCaseMesh->SetMaterial(this->rightGreenLed.slot, this->rightGreenLed.matOn) : this->sCaseMesh->SetMaterial(this->rightGreenLed.slot, this->rightGreenLed.matOff);
	this->bRightRedKnob ? this->sCaseMesh->SetMaterial(this->rightRedKnob.slot, this->rightRedKnob.matOn) : this->sCaseMesh->SetMaterial(this->rightRedKnob.slot, this->rightRedKnob.matOff);
	this->bLeftRedKnob ? this->sCaseMesh->SetMaterial(this->leftRedKnob.slot, this->leftRedKnob.matOn) : this->sCaseMesh->SetMaterial(this->leftRedKnob.slot, this->leftRedKnob.matOff);

	this->renderSegArray(this->sevenSegOne);
	this->renderSegArray(this->sevenSegTwo);

	this->renderDisplay(this->displayUp);
	this->renderDisplay(this->displayDown);
}

void ACase::BeginPlay(){
	Super::BeginPlay();
}

void ACase::setActive(bool bActive){
	this->bIsActive = bActive;
	UE_LOG(LogTemp, Warning, TEXT("Case now active!"));
}

void ACase::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	if (ERunningModesCase::SIMULATION == this->mode) {}
	else {
		this->onReqCompleteFunctor = [this](FHttpRequestPtr request, FHttpResponsePtr response, bool connected) {
			this->funcForWebHandler(request, response, connected);
			};
		this->webHandler->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctor);

		this->webHandler->sendRequest();

		if (this->reqData.response.IsValid()) {
			UE_LOG(LogTemp, Display, TEXT("Response Case: = %s"), *this->reqData.response->GetContentAsString());
		}
		else {
			UE_LOG(LogTemp, Display, TEXT("Response Case not valid"));
		}
	}

	this->numToSegments(this->sevenSegOneNum, this->sevenSegOne);
	this->numToSegments(this->sevenSegTwoNum, this->sevenSegTwo);

	this->numToDisplay(this->displayUpNum, this->displayUp);
	this->numToDisplay(this->displayDownNum, this->displayDown);

	this->renderMaterials();
}