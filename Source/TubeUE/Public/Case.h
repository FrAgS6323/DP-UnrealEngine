#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "WebHandler.h"
#include "Case.generated.h"

UENUM(BlueprintType)
enum class ERunningModesCase : uint8
{
	SIMULATION      UMETA(DisplayName = "Simulation"),
	VISUALIZATION   UMETA(DisplayName = "Visualization"),
};

UCLASS()
class TUBEUE_API ACase : public AActor
{
	GENERATED_BODY()
	private:
		typedef struct vizActualStates{
			bool bBlueButton = false,
				bRedButton = false,
				bGreenButton = false,
				bYellowButton = false,
				bLeftGreenLED = false,
				bMiddleGreenLED = false,
				bRightGreenLED = false;
			int sevenSegOneNum = 0,
				sevenSegTwoNum = 0,
				displayUpNum = 0,
				displayDownNum = 0;
		}VizActualStates;
		bool bIsActive = false;
		int32 displayUpNum, displayDownNum;
		static constexpr size_t displaySize = 5;
		typedef struct matElementData {
			bool bSwitch;
			int slot;
			TObjectPtr<UMaterialInterface> matOff;
			TObjectPtr<UMaterialInterface> matOn;
		}MatElementData;
		TObjectPtr<UStaticMeshComponent> sCaseMesh;
		MatElementData blueButton,
						greenButton, 
						redButton, 
						yellowButton,
						leftGreenLed,
						middleGreenLed,
						rightGreenLed,
						leftRedKnob,
						rightRedKnob;
		TArray<MatElementData> sevenSegOne, sevenSegTwo;
		TArray<TArray<MatElementData>> displayUp, displayDown;
		TSharedPtr<FJsonObject> responseObj;
		WebHandler* webHandlerGet, *webHandlerPost;
		VizActualStates vizActualStates;
		TFunction<void(FHttpRequestPtr request, FHttpResponsePtr response, bool connected)> onReqCompleteFunctorGet, 
																							onReqCompleteFunctorPost;
		void initSegArray(TArray<MatElementData>& segArray, 
						  const TArray<int>& slots, 
						  const TCHAR* offMatPath,
						  const TCHAR* onMatPath);
		void initDisplay(TArray<TArray<MatElementData>> &display, 
						 const TArray<TArray<int>>& slots, 
						 const TCHAR* offMatPath,
						 const TCHAR* onMatPath);
		auto numToDigits(const int &num) -> TArray<int>;
		void numToSegments(const int &num, TArray<MatElementData>& segArray);
		void numToDisplay(const int& num, TArray<TArray<MatElementData>>& display);
		void renderSegArray(const TArray<MatElementData>& segArray);
		void renderDisplay(const TArray<TArray<MatElementData>> &display);
		void renderMaterials();
		void funcForWebHandlerGet(FHttpRequestPtr request, FHttpResponsePtr response, bool connected);
		void funcForWebHandlerPost(FHttpRequestPtr request, FHttpResponsePtr response, bool connected);
		//TObjectPtr<UPointLightComponent> pointLight;
	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
	public:	
		// Sets default values for this actor's properties
		ACase();
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons") bool bBlueButton;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons") bool bRedButton;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons") bool bGreenButton;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons") bool bYellowButton;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons") bool bVizBlueButton = false;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons") bool bVizRedButton = false;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons") bool bVizGreenButton = false;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons") bool bVizYellowButton = false;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDs") bool bLeftGreenLed;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDs") bool bMiddleGreenLed;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDs") bool bRightGreenLed;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDs") bool bVizLeftGreenLed = false;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDs") bool bVizMiddleGreenLed = false;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDs") bool bVizRightGreenLed = false;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knobs") bool bRightRedKnob;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knobs") bool bLeftRedKnob;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SevenSegDisplays") int sevenSegOneNum;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SevenSegDisplays") int sevenSegTwoNum;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SevenSegDisplays") int vizSevenSegOneNum = 0;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SevenSegDisplays") int vizSevenSegTwoNum = 0;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Displays") int setDisplayUpNum;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Displays") int setDisplayDownNum;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Displays") int vizSetDisplayUpNum = 0;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Displays") int vizSetDisplayDownNum = 0;
		UPROPERTY(EditAnywhere, Category = "Runnning Mode") ERunningModesCase mode = ERunningModesCase::SIMULATION;
		void setActive(bool bActive);
		virtual void Tick(float DeltaTime) override;
};