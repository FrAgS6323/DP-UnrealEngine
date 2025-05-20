#pragma once
#include <functional>
#include "CoreMinimal.h"
#include "Http.h"
#include "JsonObjectConverter.h"
#include "WebHandler.generated.h"

USTRUCT()
struct FtubePost {
	GENERATED_BODY()
	UPROPERTY() bool power;
	UPROPERTY() int height;
	UPROPERTY() int angle;
	UPROPERTY() int X;
	UPROPERTY() int Y;
};

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

class TUBEUE_API WebHandler : public TSharedFromThis<WebHandler>{
	public:
		enum class eRequestType { GET, POST };
		//enum class eModelType { TUBE, CASE };
		class ReqData {
			public:
				bool connected;
				FHttpRequestPtr request;
				FHttpResponsePtr response;
		};
		WebHandler() = delete;
		WebHandler(const TCHAR* url, WebHandler::eRequestType reqType);
		void setPostReqData(FtubePost &postData);
		void setPostReqData(FcasePost &postData);
		void initRequest();
		void initRequest(const FString &key);
		void initRequest(const TArray<FString> &keys);
		void setFunctorOnProcessRequestComplete(AActor *actor, TSharedPtr<TFunction<void(FHttpRequestPtr request, FHttpResponsePtr response, bool connected)>> functorPtr);
		void sendRequest();
		void cancelRequest();
		~WebHandler();
	private:
		const FString url;
		WebHandler::eRequestType requestType;
		FHttpRequestPtr request;
		FHttpResponsePtr response;
		FtubePost dataTube;
		FcasePost dataCase;	
		double requestStartTime;
		TArray<float> requestDurations;
		int requestCount = 0;
		const int maxRequestCount = 100;
		bool serializeJSON(FtubePost &postData, const TArray<FString> &keys, FString &jsonStr);
		bool serializeJSON(FcasePost& postData, const FString& key, FString& jsonStr);
};