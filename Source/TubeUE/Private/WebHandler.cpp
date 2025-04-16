#include "WebHandler.h"
#include "Tube.h"

WebHandler::WebHandler(const TCHAR* url, WebHandler::eRequestType reqType):
	url(url),
	requestType(reqType){
	this->initRequest();
	//this->request->OnProcessRequestComplete().BindUObject(this, WebHandler::onResponseReceived);
}

void WebHandler::initRequest(){
	this->request = FHttpModule::Get().CreateRequest();
	this->request->SetURL(url);
	this->request->SetVerb(WebHandler::eRequestType::GET == this->requestType ? "GET" : "POST");
}

void WebHandler::setFunctorOnProcessRequestComplete(AActor *actor, TSharedPtr<TFunction<void(FHttpRequestPtr request, FHttpResponsePtr response, bool connected)>> functorPtr){
	this->request->OnProcessRequestComplete().BindLambda([functorPtr](FHttpRequestPtr request, FHttpResponsePtr response, bool connected){
		if(functorPtr.IsValid())
			(*functorPtr)(request, response, connected);
	});
}

void WebHandler::sendRequest(){
	this->request->ProcessRequest();
}

WebHandler::~WebHandler(){}