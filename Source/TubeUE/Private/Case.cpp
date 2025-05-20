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
					  TArray<TArray<int>>{{88, 89, 90, 91, 92, 93, 94},
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

	this->webHandlerGet = MakeShared<WebHandler>(TEXT("http://147.232.60.231:5001/kuforDataAPI"), WebHandler::eRequestType::GET);
	this->webHandlerPost = MakeShared<WebHandler>(TEXT("http://147.232.60.231:5001/kuforSend"), WebHandler::eRequestType::POST);
}

void ACase::funcForWebHandlerGet(FHttpRequestPtr request, FHttpResponsePtr response, bool connected) {
	if (response.IsValid()) {
		TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(response->GetContentAsString());
		FJsonSerializer::Deserialize(jsonReader, this->responseObj);
		
		//UE_LOG(LogTemp, Warning, TEXT("response: %s"), *this->reqData.response->GetContentAsString());
		//UE_LOG(LogTemp, Warning, TEXT("parsed-> EthernetC:1:O.1: %s"), *this->responseObj->GetStringField("EthernetC:1:O.1"));
		FString value;

		if (this->responseObj->TryGetStringField("Local:3:O.Data.14", value)){
			this->blueButton.bSwitch = value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false;
			if (this->vizRestActualStates.bBlueButton != this->blueButton.bSwitch) {
				this->bVizBlueButton = this->blueButton.bSwitch;
				this->vizRestActualStates.bBlueButton = this->bVizBlueButton;
			}
		}
		if (this->responseObj->TryGetStringField("Local:3:O.Data.11", value)){
			this->redButton.bSwitch = value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false;
			if (this->vizRestActualStates.bRedButton != this->redButton.bSwitch) {
				this->bVizRedButton = this->redButton.bSwitch;
				this->vizRestActualStates.bRedButton = this->bVizRedButton;
			}
		}
		if (this->responseObj->TryGetStringField("Local:3:O.Data.12", value)){
			this->yellowButton.bSwitch = value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false;
			if (this->vizRestActualStates.bYellowButton != this->yellowButton.bSwitch) {
				this->bVizYellowButton = this->yellowButton.bSwitch;
				this->vizRestActualStates.bYellowButton = this->bVizYellowButton;
			}
		}
		if (this->responseObj->TryGetStringField("Local:3:O.Data.13", value)) {
			this->greenButton.bSwitch = value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false;
			if (this->vizRestActualStates.bGreenButton != this->greenButton.bSwitch) {
				this->bVizGreenButton = this->greenButton.bSwitch;
				this->vizRestActualStates.bGreenButton = this->bVizGreenButton;
			}
		}
		if (this->responseObj->TryGetStringField("Local:1:O.Ch0Data", value)) {
			LexTryParseString(this->displayUpNum, *value);
			if (this->vizRestActualStates.displayUpNum != this->displayUpNum){
				this->vizSetDisplayUpNum = this->displayUpNum;
				this->vizRestActualStates.displayUpNum = this->vizSetDisplayUpNum;
			}
			
		}
		if (this->responseObj->TryGetStringField("Local:1:O.Ch1Data", value)){ 
			LexTryParseString(this->displayDownNum, *value);
			if (this->vizRestActualStates.displayDownNum != this->displayDownNum) {
				this->displayDownNum *= 2;
				this->vizSetDisplayDownNum = this->displayDownNum;
				this->vizRestActualStates.displayDownNum = this->vizSetDisplayDownNum;
			}
			
		}	
		if (this->responseObj->TryGetStringField("Local:3:O.Data.8", value)) {
			this->leftGreenLed.bSwitch = value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false;
			if (this->vizRestActualStates.bLeftGreenLED != this->leftGreenLed.bSwitch) {
				this->bVizLeftGreenLed = this->leftGreenLed.bSwitch;
				this->vizRestActualStates.bLeftGreenLED = this->bVizLeftGreenLed;
			}
		}
		if (this->responseObj->TryGetStringField("Local:3:O.Data.9", value)) {
			this->middleGreenLed.bSwitch = value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false;
			if (this->vizRestActualStates.bMiddleGreenLED != this->middleGreenLed.bSwitch) {
				this->bVizMiddleGreenLed = this->middleGreenLed.bSwitch;
				this->vizRestActualStates.bMiddleGreenLED = this->bVizMiddleGreenLed;
			}
		}
		if (this->responseObj->TryGetStringField("Local:3:O.Data.10", value)) {
			this->rightGreenLed.bSwitch = value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false;
			if (this->vizRestActualStates.bRightGreenLED != this->rightGreenLed.bSwitch) {
				this->bVizRightGreenLed = this->rightGreenLed.bSwitch;
				this->vizRestActualStates.bRightGreenLED = this->bVizRightGreenLed;
			}
		}
		
		TArray<bool> bSevenSegOne, bSevenSegTwo;

		if (this->responseObj->TryGetStringField("Local:2:I.Data.3", value)) this->leftRedKnob.bSwitch = value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false;
		if (this->responseObj->TryGetStringField("Local:2:I.Data.5", value)) this->rightRedKnob.bSwitch = value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false;

		if (this->responseObj->TryGetStringField("EthernetC:2:O.1", value)) bSevenSegOne.Add(value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false); //a
		if (this->responseObj->TryGetStringField("EthernetC:2:O.0", value)) bSevenSegOne.Add(value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false); //b
		if (this->responseObj->TryGetStringField("EthernetC:1:O.2", value)) bSevenSegOne.Add(value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false); //c
		if (this->responseObj->TryGetStringField("EthernetC:1:O.1", value)) bSevenSegOne.Add(value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false); //d
		if (this->responseObj->TryGetStringField("EthernetC:1:O.0", value)) bSevenSegOne.Add(value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false); //e
		if (this->responseObj->TryGetStringField("EthernetC:2:O.2", value)) bSevenSegOne.Add(value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false); //f
		if (this->responseObj->TryGetStringField("EthernetC:2:O.3", value)) bSevenSegOne.Add(value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false); //g
		if (this->responseObj->TryGetStringField("EthernetC:1:O.3", value)) bSevenSegOne.Add(value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false); //.

		if (this->responseObj->TryGetStringField("Local:3:O.Data.5", value)) bSevenSegTwo.Add(value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false); //a
		if (this->responseObj->TryGetStringField("Local:3:O.Data.4", value)) bSevenSegTwo.Add(value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false); //b
		if (this->responseObj->TryGetStringField("Local:3:O.Data.2", value)) bSevenSegTwo.Add(value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false); //c
		if (this->responseObj->TryGetStringField("Local:3:O.Data.1", value)) bSevenSegTwo.Add(value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false); //d
		if (this->responseObj->TryGetStringField("Local:3:O.Data.0", value)) bSevenSegTwo.Add(value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false); //e
		if (this->responseObj->TryGetStringField("Local:3:O.Data.6", value)) bSevenSegTwo.Add(value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false); //f
		if (this->responseObj->TryGetStringField("Local:3:O.Data.7", value)) bSevenSegTwo.Add(value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false); //g
		if (this->responseObj->TryGetStringField("Local:3:O.Data.3", value)) bSevenSegTwo.Add(value.Equals(TEXT("true"), ESearchCase::IgnoreCase) ? true : false); //.
		
		this->segmentsToNum(bSevenSegOne, this->vizInputActualStates.sevenSegOneNumGet);
		this->segmentsToNum(bSevenSegTwo, this->vizInputActualStates.sevenSegTwoNumGet);
		
		if(-1 != this->vizInputActualStates.sevenSegOneNumGet){
			for (size_t i = 0; i < bSevenSegOne.Num(); ++i) this->sevenSegOne[i].bSwitch = bSevenSegOne[i];
			if (this->vizRestActualStates.sevenSegOneNum != this->vizInputActualStates.sevenSegOneNumGet) {
				this->vizSevenSegOneNum = this->vizInputActualStates.sevenSegOneNumGet;
				this->vizRestActualStates.sevenSegOneNum = this->vizInputActualStates.sevenSegOneNumGet;
			}
		}
		
		if (-1 != this->vizInputActualStates.sevenSegTwoNumGet) {
			for (size_t i = 0; i < bSevenSegTwo.Num(); ++i) this->sevenSegTwo[i].bSwitch = bSevenSegTwo[i];
			if (this->vizRestActualStates.sevenSegTwoNum != this->vizInputActualStates.sevenSegTwoNumGet) {
				this->vizSevenSegTwoNum = this->vizInputActualStates.sevenSegTwoNumGet;
				this->vizRestActualStates.sevenSegTwoNum = this->vizInputActualStates.sevenSegTwoNumGet;
			}
		}
	}
}

void ACase::funcForWebHandlerPost(FHttpRequestPtr request, FHttpResponsePtr response, bool connected){
	if (response.IsValid()){
		UE_LOG(LogTemp, Warning, TEXT("POST Response: %s"), *response->GetContentAsString());
	}else{
		UE_LOG(LogTemp, Error, TEXT("No response received"));
	}
}

void ACase::initSegArray(TArray<ACase::MatElementData>& segArray,
						 const TArray<int>& slots, 
						 const TCHAR* offMatPath,
						 const TCHAR* onMatPath){
	for(const int &slot : slots)
		segArray.Add({ false, slot, UEngineHelper::loadMaterialDynamic(offMatPath), UEngineHelper::loadMaterialDynamic(onMatPath) });
}

void ACase::initDisplay(TArray<TArray<ACase::MatElementData>> &display,
					    const TArray<TArray<int>> &slots, 
						const TCHAR* offMatPath,
						const TCHAR* onMatPath){
	for (size_t i = 0; i < ACase::displaySize; ++i) {
		TArray<ACase::MatElementData> segArray;

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

void ACase::segmentsToNum(const TArray<bool>& segArray, int& num){
	if (segArray[0] && segArray[1] && segArray[2] && segArray[3] && segArray[4] && segArray[5] && !segArray[6]) num = 0;
	else if (!segArray[0] && segArray[1] && segArray[2] && !segArray[3] && !segArray[4] && !segArray[5] && !segArray[6]) num = 1;
	else if (segArray[0] && segArray[1] && !segArray[2] && segArray[3] && segArray[4] && !segArray[5] && segArray[6]) num = 2;
	else if (segArray[0] && segArray[1] && segArray[2] && segArray[3] && !segArray[4] && !segArray[5] && segArray[6]) num = 3;
	else if (!segArray[0] && segArray[1] && segArray[2] && !segArray[3] && !segArray[4] && segArray[5] && segArray[6]) num = 4;
	else if (segArray[0] && !segArray[1] && segArray[2] && segArray[3] && !segArray[4] && segArray[5] && segArray[6]) num = 5;
	else if (segArray[0] && !segArray[1] && segArray[2] && segArray[3] && segArray[4] && segArray[5] && segArray[6]) num = 6;
	else if (segArray[0] && segArray[1] && segArray[2] && !segArray[3] && !segArray[4] && !segArray[5] && !segArray[6]) num = 7;
	else if (segArray[0] && segArray[1] && segArray[2] && segArray[3] && segArray[4] && segArray[5] && segArray[6]) num = 8;
	else if (segArray[0] && segArray[1] && segArray[2] && segArray[3] && !segArray[4] && segArray[5] && segArray[6]) num = 9;
	else num = -1;
}

void ACase::numToDisplay(const int& num, TArray<TArray<MatElementData>>& display){
	TArray<int> digits = this->numToDigits(num);

	if (digits.Num() > 5){
		UE_LOG(LogTemp, Warning, TEXT("You must enter number below 5 digits!"));
		return;
	}

	int32 numOfEmptyDigits = ACase::displaySize - digits.Num();

	for (size_t emptyIdx = 0; emptyIdx < numOfEmptyDigits; emptyIdx++)
		this->numToSegments(-1, display[emptyIdx]);

	for (int32 i = digits.Num() - 1; i >= 0; i--)
		this->numToSegments(digits[i], display[i + numOfEmptyDigits]);
}

void ACase::renderSegArray(const TArray<ACase::MatElementData>& segArray){
	for (const auto& segment : segArray)
		segment.bSwitch ? this->sCaseMesh->SetMaterial(segment.slot, segment.matOn) : this->sCaseMesh->SetMaterial(segment.slot, segment.matOff);
}

void ACase::renderDisplay(const TArray<TArray<ACase::MatElementData>>& display){
	for (const auto& segArray : display)
		this->renderSegArray(segArray);
}

void ACase::renderMaterials(){
	if (ERunningModesCase::SIMULATION == this->mode){
		this->bBlueButton ? this->sCaseMesh->SetMaterial(this->blueButton.slot, this->blueButton.matOn) : this->sCaseMesh->SetMaterial(this->blueButton.slot, this->blueButton.matOff);
		this->bRedButton ? this->sCaseMesh->SetMaterial(this->redButton.slot, this->redButton.matOn) : this->sCaseMesh->SetMaterial(this->redButton.slot, this->redButton.matOff);
		this->bGreenButton ? this->sCaseMesh->SetMaterial(this->greenButton.slot, this->greenButton.matOn) : this->sCaseMesh->SetMaterial(this->greenButton.slot, this->greenButton.matOff);
		this->bYellowButton ? this->sCaseMesh->SetMaterial(this->yellowButton.slot, this->yellowButton.matOn) : this->sCaseMesh->SetMaterial(this->yellowButton.slot, this->yellowButton.matOff);
		this->bLeftGreenLed ? this->sCaseMesh->SetMaterial(this->leftGreenLed.slot, this->leftGreenLed.matOn) : this->sCaseMesh->SetMaterial(this->leftGreenLed.slot, this->leftGreenLed.matOff);
		this->bMiddleGreenLed ? this->sCaseMesh->SetMaterial(this->middleGreenLed.slot, this->middleGreenLed.matOn) : this->sCaseMesh->SetMaterial(this->middleGreenLed.slot, this->middleGreenLed.matOff);
		this->bRightGreenLed ? this->sCaseMesh->SetMaterial(this->rightGreenLed.slot, this->rightGreenLed.matOn) : this->sCaseMesh->SetMaterial(this->rightGreenLed.slot, this->rightGreenLed.matOff);
		this->bRightRedKnob ? this->sCaseMesh->SetMaterial(this->rightRedKnob.slot, this->rightRedKnob.matOn) : this->sCaseMesh->SetMaterial(this->rightRedKnob.slot, this->rightRedKnob.matOff);
		this->bLeftRedKnob ? this->sCaseMesh->SetMaterial(this->leftRedKnob.slot, this->leftRedKnob.matOn) : this->sCaseMesh->SetMaterial(this->leftRedKnob.slot, this->leftRedKnob.matOff);
	}else{
		this->blueButton.bSwitch ? this->sCaseMesh->SetMaterial(this->blueButton.slot, this->blueButton.matOn) : this->sCaseMesh->SetMaterial(this->blueButton.slot, this->blueButton.matOff);
		this->redButton.bSwitch ? this->sCaseMesh->SetMaterial(this->redButton.slot, this->redButton.matOn) : this->sCaseMesh->SetMaterial(this->redButton.slot, this->redButton.matOff);
		this->greenButton.bSwitch ? this->sCaseMesh->SetMaterial(this->greenButton.slot, this->greenButton.matOn) : this->sCaseMesh->SetMaterial(this->greenButton.slot, this->greenButton.matOff);
		this->yellowButton.bSwitch ? this->sCaseMesh->SetMaterial(this->yellowButton.slot, this->yellowButton.matOn) : this->sCaseMesh->SetMaterial(this->yellowButton.slot, this->yellowButton.matOff);
		this->leftGreenLed.bSwitch ? this->sCaseMesh->SetMaterial(this->leftGreenLed.slot, this->leftGreenLed.matOn) : this->sCaseMesh->SetMaterial(this->leftGreenLed.slot, this->leftGreenLed.matOff);
		this->middleGreenLed.bSwitch ? this->sCaseMesh->SetMaterial(this->middleGreenLed.slot, this->middleGreenLed.matOn) : this->sCaseMesh->SetMaterial(this->middleGreenLed.slot, this->middleGreenLed.matOff);
		this->rightGreenLed.bSwitch ? this->sCaseMesh->SetMaterial(this->rightGreenLed.slot, this->rightGreenLed.matOn) : this->sCaseMesh->SetMaterial(this->rightGreenLed.slot, this->rightGreenLed.matOff);
		this->rightRedKnob.bSwitch ? this->sCaseMesh->SetMaterial(this->rightRedKnob.slot, this->rightRedKnob.matOn) : this->sCaseMesh->SetMaterial(this->rightRedKnob.slot, this->rightRedKnob.matOff);
		this->leftRedKnob.bSwitch ? this->sCaseMesh->SetMaterial(this->leftRedKnob.slot, this->leftRedKnob.matOn) : this->sCaseMesh->SetMaterial(this->leftRedKnob.slot, this->leftRedKnob.matOff);
	}
	this->renderSegArray(this->sevenSegOne);
	this->renderSegArray(this->sevenSegTwo);

	this->renderDisplay(this->displayUp);
	this->renderDisplay(this->displayDown);
}

void ACase::BeginPlay(){
	Super::BeginPlay();
}

void ACase::setActive(APlayerController* inPlayerController, bool bActive){
	this->bIsActive = bActive;
	this->playerController = this->bIsActive ? inPlayerController : nullptr;
	UE_LOG(LogTemp, Warning, TEXT("Case now active!"));
}

void ACase::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	if (ERunningModesCase::VISUALIZATION == this->mode) {
		this->onReqCompleteFunctorGet = MakeShared<TFunction<void(FHttpRequestPtr request, FHttpResponsePtr response, bool connected)>>([this](FHttpRequestPtr request, FHttpResponsePtr response, bool connected) {
			this->funcForWebHandlerGet(request, response, connected);

			this->webHandlerGet->initRequest();
			this->webHandlerGet->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorGet);
			this->webHandlerGet->sendRequest();
		});
		
		if (!this->bFirstReqGETSent) {
			this->webHandlerGet->initRequest();
			this->webHandlerGet->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorGet);
			this->webHandlerGet->sendRequest();
			this->bFirstReqGETSent = true;
		}

		this->onReqCompleteFunctorPost = MakeShared<TFunction<void(FHttpRequestPtr request, FHttpResponsePtr response, bool connected)>>([this](FHttpRequestPtr request, FHttpResponsePtr response, bool connected) {
			this->funcForWebHandlerPost(request, response, connected);
		});

		if (this->vizInputActualStates.bLeftGreenLED != this->bVizLeftGreenLed) {
			FcasePost data;
			data.HL1 = this->bVizLeftGreenLed;

			this->webHandlerPost->setPostReqData(data);
			this->webHandlerPost->initRequest("HL1");
			this->webHandlerPost->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorPost);
			this->webHandlerPost->sendRequest();
			this->vizInputActualStates.bLeftGreenLED = this->bVizLeftGreenLed;
		}

		else if (this->vizInputActualStates.bMiddleGreenLED != this->bVizMiddleGreenLed) {
			FcasePost data;
			data.HL2 = this->bVizMiddleGreenLed;

			this->webHandlerPost->setPostReqData(data);
			this->webHandlerPost->initRequest("HL2");
			this->webHandlerPost->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorPost);
			this->webHandlerPost->sendRequest();
			this->vizInputActualStates.bMiddleGreenLED = this->bVizMiddleGreenLed;
		}

		else if (this->vizInputActualStates.bRightGreenLED != this->bVizRightGreenLed) {
			FcasePost data;
			data.HL3 = this->bVizRightGreenLed;

			this->webHandlerPost->setPostReqData(data);
			this->webHandlerPost->initRequest("HL3");
			this->webHandlerPost->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorPost);
			this->webHandlerPost->sendRequest();
			this->vizInputActualStates.bRightGreenLED = this->bVizRightGreenLed;
		}

		else if (this->vizInputActualStates.bRedButton != this->bVizRedButton) {
			FcasePost data;
			data.HL4 = this->bVizRedButton;

			this->webHandlerPost->setPostReqData(data);
			this->webHandlerPost->initRequest("HL4");
			this->webHandlerPost->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorPost);
			this->webHandlerPost->sendRequest();
			this->vizInputActualStates.bRedButton = this->bVizRedButton;
		}

		else if (this->vizInputActualStates.bYellowButton != this->bVizYellowButton) {
			FcasePost data;
			data.HL5 = this->bVizYellowButton;

			this->webHandlerPost->setPostReqData(data);
			this->webHandlerPost->initRequest("HL5");
			this->webHandlerPost->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorPost);
			this->webHandlerPost->sendRequest();
			this->vizInputActualStates.bYellowButton = this->bVizYellowButton;
		}

		else if (this->vizInputActualStates.bGreenButton != this->bVizGreenButton) {
			FcasePost data;
			data.HL6 = this->bVizGreenButton;

			this->webHandlerPost->setPostReqData(data);
			this->webHandlerPost->initRequest("HL6");
			this->webHandlerPost->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorPost);
			this->webHandlerPost->sendRequest();
			this->vizInputActualStates.bGreenButton = this->bVizGreenButton;
		}

		else if (this->vizInputActualStates.bBlueButton != this->bVizBlueButton) {
			FcasePost data;
			data.HL7 = this->bVizBlueButton;

			this->webHandlerPost->setPostReqData(data);
			this->webHandlerPost->initRequest("HL7");
			this->webHandlerPost->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorPost);
			this->webHandlerPost->sendRequest();
			this->vizInputActualStates.bBlueButton = this->bVizBlueButton;
		}

		else if (this->vizInputActualStates.sevenSegOneNum != this->vizSevenSegOneNum) {
			FcasePost data;
			data.H7seg1 = this->vizSevenSegOneNum;

			this->webHandlerPost->setPostReqData(data);
			this->webHandlerPost->initRequest("H7seg1");
			this->webHandlerPost->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorPost);
			this->webHandlerPost->sendRequest();
			this->vizInputActualStates.sevenSegOneNum = this->vizSevenSegOneNum;
		}

		else if (this->vizInputActualStates.sevenSegTwoNum != this->vizSevenSegTwoNum) {
			FcasePost data;
			data.H7seg2 = this->vizSevenSegTwoNum;

			this->webHandlerPost->setPostReqData(data);
			this->webHandlerPost->initRequest("H7seg2");
			this->webHandlerPost->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorPost);
			this->webHandlerPost->sendRequest();
			this->vizInputActualStates.sevenSegTwoNum = this->vizSevenSegTwoNum;
		}

		else if (this->vizInputActualStates.displayUpNum != this->vizSetDisplayUpNum) {
			FcasePost data;
			data.A1 = this->vizSetDisplayUpNum;

			this->webHandlerPost->setPostReqData(data);
			this->webHandlerPost->initRequest("A1");
			this->webHandlerPost->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorPost);
			this->webHandlerPost->sendRequest();
			this->vizInputActualStates.displayUpNum = this->vizSetDisplayUpNum;
		}

		else if (this->vizInputActualStates.displayDownNum != this->vizSetDisplayDownNum) {
			FcasePost data;
			data.A2 = this->vizSetDisplayDownNum;

			this->webHandlerPost->setPostReqData(data);
			this->webHandlerPost->initRequest("A2");
			this->webHandlerPost->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorPost);
			this->webHandlerPost->sendRequest();
			this->vizInputActualStates.displayDownNum = this->vizSetDisplayDownNum;
		}
		//if (this->modeBeforeSwitch != this->mode) this->modeBeforeSwitch = this->mode;
	}else{
		this->numToSegments(this->sevenSegOneNum, this->sevenSegOne);
		this->numToSegments(this->sevenSegTwoNum, this->sevenSegTwo);

		this->displayUpNum = this->setDisplayUpNum;
		this->displayDownNum = this->setDisplayDownNum;
	}
	this->numToDisplay(this->displayUpNum, this->displayUp);
	this->numToDisplay(this->displayDownNum, this->displayDown);

	this->renderMaterials();
	if (this->playerController && this->playerController->WasInputKeyJustReleased(EKeys::U)) this->bShowColliders = !this->bShowColliders;
	if (this->bShowColliders) UEngineHelper::drawAllSimpleCollidersForActor(this);

#if 0
	if (this->modeBeforeSwitch != this->mode) {
		if (this->webHandlerGet.IsValid()) this->webHandlerGet->cancelRequest();
		if (this->webHandlerPost.IsValid()) this->webHandlerPost->cancelRequest();
		this->modeBeforeSwitch = this->mode;
	}
#endif
}