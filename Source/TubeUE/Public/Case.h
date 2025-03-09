#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "Case.generated.h"

UCLASS()
class TUBEUE_API ACase : public AActor
{
	GENERATED_BODY()
	private:
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
		void clearSegArray(const TArray<MatElementData>& segArray);
		void clearDisplay(const TArray<TArray<MatElementData>>& display);
		void renderMaterials();
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
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDs") bool bLeftGreenLed;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDs") bool bMiddleGreenLed;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDs") bool bRightGreenLed;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knobs") bool bRightRedKnob;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knobs") bool bLeftRedKnob;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SevenSegDisplays") int sevenSegOneNum;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SevenSegDisplays") int sevenSegTwoNum;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Displays") int displayUpNum;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Displays") int displayDownNum;
		// Called every frame
		virtual void Tick(float DeltaTime) override;
};