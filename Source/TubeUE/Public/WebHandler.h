#pragma once
#include "CoreMinimal.h"
#include "Http.h"
#include <functional>

class TUBEUE_API WebHandler{
	private:
		FHttpRequestPtr request;
		FHttpResponsePtr response;
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
		void setFunctorOnProcessRequestComplete(AActor *actor, TFunction<void(FHttpRequestPtr request, FHttpResponsePtr response, bool connected)>& functor);
		void sendRequest();
		~WebHandler();
};