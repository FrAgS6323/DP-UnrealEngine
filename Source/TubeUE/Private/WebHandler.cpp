#include "WebHandler.h"
#include "Tube.h"

WebHandler::WebHandler(const TCHAR* url, WebHandler::eRequestType reqType):
	request(FHttpModule::Get().CreateRequest()){
	//this->request->OnProcessRequestComplete().BindUObject(this, WebHandler::onResponseReceived);
	this->request->SetURL(url);
	this->request->SetVerb(WebHandler::eRequestType::GET == reqType ? "GET" : "POST");
}

void WebHandler::setFunctorOnProcessRequestComplete(AActor *actor, TFunction<void(FHttpRequestPtr request, FHttpResponsePtr response, bool connected)> &functor){
	this->request->OnProcessRequestComplete().BindLambda([&](FHttpRequestPtr request, FHttpResponsePtr response, bool connected){
			functor(request, response, connected);

#if 0
			TSharedPtr<FJsonObject> responseObj;
			TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(response->GetContentAsString());
			FJsonSerializer::Deserialize(jsonReader, responseObj);

			//const TCHAR* strConnected = connected ? TEXT("True") : TEXT("False");

			//UE_LOG(LogTemp, Warning, TEXT("Request URL: %s"), *request->GetURL());
			//UE_LOG(LogTemp, Warning, TEXT("WH request: %s"), *request->GetContent());
			if (response.IsValid()) {
				UE_LOG(LogTemp, Warning, TEXT("response: %s"), *response->GetContentAsString());
				//UE_LOG(LogTemp, Warning, TEXT("parsed-> target: %s"), *responseObj->GetStringField("target"));
			}
			else{
				UE_LOG(LogTemp, Warning, TEXT("WH response invalid!"));
			}
#endif
	});
}

void WebHandler::sendRequest(){
	this->request->ProcessRequest();
}

WebHandler::~WebHandler(){}