#pragma once
#include <functional>
#include "CoreMinimal.h"
#include "Http.h"
#include "JsonObjectConverter.h"
#include "WebHandler.generated.h"

USTRUCT()
struct FcasePost {
	GENERATED_BODY()
	UPROPERTY() bool HL1;
	UPROPERTY() bool HL2;
	UPROPERTY() bool HL3;
	UPROPERTY() bool HL4;
	UPROPERTY() bool HL5;
	UPROPERTY() bool HL6;
	UPROPERTY() bool HL7;
	UPROPERTY() int H7seg1;
	UPROPERTY() int H7seg2;
	UPROPERTY() int A1;
	UPROPERTY() int A2;
};

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
		void setPostCaseReqData(FcasePost &data);
		void initRequest();
		void setFunctorOnProcessRequestComplete(AActor *actor, TSharedPtr<TFunction<void(FHttpRequestPtr request, FHttpResponsePtr response, bool connected)>> functorPtr);
		void sendRequest();
		~WebHandler();
	private:
		const FString url;
		WebHandler::eRequestType requestType;
		FHttpRequestPtr request;
		FHttpResponsePtr response;
		FcasePost data;
		bool serializeJSON(FcasePost &postData, FString &jsonStr);
};