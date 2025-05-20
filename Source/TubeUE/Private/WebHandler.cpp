#include "WebHandler.h"
#include "Tube.h"

WebHandler::WebHandler(const TCHAR* url, WebHandler::eRequestType reqType):
	url(url),
	requestType(reqType){
	//this->initRequest();
	//this->request->OnProcessRequestComplete().BindUObject(this, WebHandler::onResponseReceived);
}

void WebHandler::setPostReqData(FtubePost &postData){
	this->dataTube = postData;
}

void WebHandler::setPostReqData(FcasePost &postData){
	this->dataCase = postData;
}

bool WebHandler::serializeJSON(FtubePost& postData, const TArray<FString> &keys, FString& jsonStr) {
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject);
	
	for (const FString& key : keys){
		if ("power" == key) jsonObject->SetNumberField("power", postData.power);
		if ("height" == key) jsonObject->SetNumberField("height", postData.height);
		if ("angle" == key) jsonObject->SetNumberField("angle", postData.angle);
		if ("X" == key) jsonObject->SetNumberField("X", postData.X);
		if ("Y" == key) jsonObject->SetNumberField("Y", postData.Y);
	}

	TSharedRef<TJsonWriter<>> writer = TJsonWriterFactory<>::Create(&jsonStr);
	bool bSerialized = FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);
	//UE_LOG(LogTemp, Warning, TEXT("Serialized JSON: '%s' (Len: %d)"), *jsonStr, jsonStr.Len());
	writer->Close();
	return bSerialized;
}

bool WebHandler::serializeJSON(FcasePost &postData, const FString& key, FString &jsonStr) {
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject);

	if("HL1" == key) jsonObject->SetNumberField("HL1", postData.HL1);
	else if("HL2" == key) jsonObject->SetNumberField("HL2", postData.HL2);
	else if("HL3" == key) jsonObject->SetNumberField("HL3", postData.HL3);
	else if("HL4" == key) jsonObject->SetNumberField("HL4", postData.HL4);
	else if("HL5" == key) jsonObject->SetNumberField("HL5", postData.HL5);
	else if("HL6" == key) jsonObject->SetNumberField("HL6", postData.HL6);
	else if("HL7" == key) jsonObject->SetNumberField("HL7", postData.HL7);
	else if("H7seg1" == key) jsonObject->SetNumberField("H7seg1", postData.H7seg1);
	else if("H7seg2" == key) jsonObject->SetNumberField("H7seg2", postData.H7seg2);
	else if("A1" == key) jsonObject->SetNumberField("A1", postData.A1);
	else if("A2" == key) jsonObject->SetNumberField("A2", postData.A2);

	TSharedRef<TJsonWriter<>> writer = TJsonWriterFactory<>::Create(&jsonStr);
	bool bSerialized = FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);
	//UE_LOG(LogTemp, Warning, TEXT("Serialized JSON: '%s' (Len: %d)"), *jsonStr, jsonStr.Len());
	writer->Close();
	return bSerialized;
}

void WebHandler::initRequest(){
	this->requestStartTime = FPlatformTime::Seconds();
	this->request = FHttpModule::Get().CreateRequest();
	this->request->SetHeader(TEXT("Cache-Control"), TEXT("no-cache"));
	this->request->SetHeader(TEXT("Pragma"), TEXT("no-cache"));
	this->request->SetHeader(TEXT("Expires"), TEXT("0"));
	this->request->SetURL(url);
	this->request->SetVerb(WebHandler::eRequestType::GET == this->requestType ? "GET" : "POST");
}

void WebHandler::initRequest(const FString& key){
	this->initRequest();

	if (WebHandler::eRequestType::POST == this->requestType) {
		//UE_LOG(LogTemp, Warning, TEXT("Created POST request: %p"), this->request.Get());
		FString jsonStr;

		if (this->serializeJSON(this->dataCase, key, jsonStr)) {
			if (!jsonStr.IsEmpty()) {
				this->request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
				this->request->SetContentAsString(jsonStr);
				UE_LOG(LogTemp, Warning, TEXT("Serialized JSON POST: %s"), *jsonStr);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("jsonStr is EMPTY — Content will NOT be set!"));
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Json POST Serialization failed!"));
		}
	}
}

void WebHandler::initRequest(const TArray<FString> &keys) {
	this->initRequest();

	if (WebHandler::eRequestType::POST == this->requestType) {
		//UE_LOG(LogTemp, Warning, TEXT("Created POST request: %p"), this->request.Get());
		FString jsonStr;

		if (this->serializeJSON(this->dataTube, keys, jsonStr)) {
			if (!jsonStr.IsEmpty()) {
				this->request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
				this->request->SetContentAsString(jsonStr);
				UE_LOG(LogTemp, Warning, TEXT("Serialized JSON POST: %s"), *jsonStr);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("jsonStr is EMPTY — Content will NOT be set!"));
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Json POST Serialization failed!"));
		}
	}
}

void WebHandler::setFunctorOnProcessRequestComplete(AActor *actor, TSharedPtr<TFunction<void(FHttpRequestPtr request, FHttpResponsePtr response, bool connected)>> functorPtr){
	TSharedPtr<WebHandler> thisShared = AsShared();
	TWeakPtr<WebHandler> weakThis = thisShared;

	this->request->OnProcessRequestComplete().BindLambda([weakThis, functorPtr](FHttpRequestPtr request, FHttpResponsePtr response, bool connected){
		TSharedPtr<WebHandler> _this = weakThis.Pin();
		if (!_this.IsValid())
			return;

		if (!functorPtr.IsValid())
			return;
		
		(*functorPtr)(request, response, connected);

		if (response.IsValid()) {
			double durationMs = (FPlatformTime::Seconds() - _this->requestStartTime) * 1000.0f;
			UE_LOG(LogTemp, Log, TEXT("Request took %.6f ms"), durationMs);


			_this->requestDurations.Add(durationMs);
			_this->requestCount++;

			if (_this->requestCount >= _this->maxRequestCount) {
				float sum = 0.0f;

				for (float time : _this->requestDurations)
					sum += time;

				float avg = sum / _this->requestDurations.Num();
				UE_LOG(LogTemp, Log, TEXT("Avg of %d requests: %.3f ms"), _this->maxRequestCount, avg);

				_this->requestDurations.Empty();
				_this->requestCount = 0;
			}
		}
		else{
			UE_LOG(LogTemp, Warning, TEXT("Response not valid!"));
		}
	});
}

void WebHandler::sendRequest(){
	this->request->ProcessRequest();
}

void WebHandler::cancelRequest(){
	if (this->request.IsValid()) {
		this->request->CancelRequest();
		UE_LOG(LogTemp, Warning, TEXT("Request canceled!"));
	}
}

WebHandler::~WebHandler(){}