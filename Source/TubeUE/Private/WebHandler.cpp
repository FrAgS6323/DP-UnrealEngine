#include "WebHandler.h"
#include "Tube.h"

WebHandler::WebHandler(const TCHAR* url, WebHandler::eRequestType reqType):
	url(url),
	requestType(reqType){
	//this->initRequest();
	//this->request->OnProcessRequestComplete().BindUObject(this, WebHandler::onResponseReceived);
}

void WebHandler::setPostCaseReqData(FcasePost &postData){
	this->data = postData;
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
	this->request = FHttpModule::Get().CreateRequest();
	this->request->SetURL(url);
	this->request->SetVerb(WebHandler::eRequestType::GET == this->requestType ? "GET" : "POST");
}

void WebHandler::initRequest(const FString& key){
	this->request = FHttpModule::Get().CreateRequest();
	this->request->SetURL(url);
	this->request->SetVerb(WebHandler::eRequestType::GET == this->requestType ? "GET" : "POST");


	if (WebHandler::eRequestType::POST == this->requestType) {
		//UE_LOG(LogTemp, Warning, TEXT("Created POST request: %p"), this->request.Get());
		FString jsonStr;

		if (this->serializeJSON(this->data, key, jsonStr)) {
			if (!jsonStr.IsEmpty()) {
				this->request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
				this->request->SetContentAsString(jsonStr);
				UE_LOG(LogTemp, Warning, TEXT("Serialized JSON POST: %s"), *jsonStr);
				//UE_LOG(LogTemp, Warning, TEXT("Header now: %s"), *this->request->GetHeader(TEXT("Content-Type")));
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
	this->request->OnProcessRequestComplete().BindLambda([functorPtr](FHttpRequestPtr request, FHttpResponsePtr response, bool connected){
		if(functorPtr.IsValid())
			(*functorPtr)(request, response, connected);
	});
}

void WebHandler::sendRequest(){
	//if (WebHandler::eRequestType::POST == this->requestType) UE_LOG(LogTemp, Warning, TEXT("Sending POST request: %p"), this->request.Get());
	this->request->ProcessRequest();
}

WebHandler::~WebHandler(){}