#pragma once
#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "Components/ActorComponent.h"
#include "TurtleBotTypeEnum.h"
#include "RotateWheelLeft.generated.h"

#if 0
UENUM(BlueprintType) // Add this if the enum needs to be exposed to Blueprints
enum class TurtleBotType : uint8 { // Use `uint8` for Unreal compatibility
	ONE UMETA(DisplayName = "One"),
	TWO UMETA(DisplayName = "Two"),
	THREE UMETA(DisplayName = "Three")
};
#endif

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TUBEUE_API URotateWheelLeft : public UActorComponent {
	GENERATED_BODY()
	private:
		int ticksPassed = 0,
			idx = 0,
			actualSeriesOfTicks = 0;
		double rotationVel;
		static constexpr double rotationVelForward = 1000;
		static constexpr double rotationVelStop = 1;
		UStaticMeshComponent* sMeshComp;
		AActor* owner;
		UPROPERTY(EditAnywhere, Category = "TurtleBot Type") TurtleBotTypeEnum eTurtleType;
		TArray<int> ticksOne = { 656,-40,1312,-40,656,-40,1312 },
					ticksTwo = { 656,-40,328,-40,246,40,984,40,246,40,574,40,1066,-40,738,-40,410 },
					ticksThree = { 1230,40,738,40,1230,40,738 },
					ticks;
	protected:
		virtual void BeginPlay() override;
	public:
		URotateWheelLeft();
		//UPROPERTY(EditAnywhere, Category = "Velocity") double rotationVel;
		
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	};