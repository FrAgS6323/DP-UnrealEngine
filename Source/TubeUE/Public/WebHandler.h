#pragma once
#include "CoreMinimal.h"
#include "Http.h"
#include <functional>

class TUBEUE_API WebHandler{
	public:
		enum class eRequestType { GET, POST };
		class ReqData {
			public:
				bool connected;
				FHttpRequestPtr request;
				FHttpResponsePtr response;
		};
		WebHandler() = delete;
		WebHandler(const TCHAR* url, WebHandler::eRequestType reqType);
		void initRequest();
		void setFunctorOnProcessRequestComplete(AActor *actor, TSharedPtr<TFunction<void(FHttpRequestPtr request, FHttpResponsePtr response, bool connected)>> functorPtr);
		void sendRequest();
		~WebHandler();
	private:
		const FString url;
		WebHandler::eRequestType requestType;
		FHttpRequestPtr request;
		FHttpResponsePtr response;
};