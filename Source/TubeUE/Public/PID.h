#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

class TUBEUE_API UPID{
	//GENERATED_BODY()
	private:
		bool clamp;
		double kP,
			   kI,
			   kD,
			   saturationLimitMin,
			   saturationLimitMax,
			   error,
			   p,
			   i,
			   d;
	protected:
		// Called when the game starts
		//virtual void BeginPlay() override;
	public:
		static constexpr double idealP = 0.75; //prvotna hodnota vypocitana -> // 1.19f // 7.55f | 2.0f
		static constexpr double idealI = 0.25f;  // 1.315f // 0.033f | 0.85f
		static constexpr double idealD = 0.1f; // 0.329f // 0.008f | 0.5f
		UPID();
		bool detectChange(double P,
						  double I,
						  double D);
		bool setPIDvalues(double P,
						  double I,
						  double D);
		bool setIdealPIDvalues();
		void setSaturationLimits(double min, double max);
		double getPIDOutput(double currentError, double deltaTime);
		double getPIDOutputOverload(double currentError, double deltaTime);
		static double estimateError(double desiredPos, double currentPos);
		template <typename T> static int sgn(T val);
		//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};