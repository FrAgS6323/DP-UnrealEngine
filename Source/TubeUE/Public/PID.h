#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

enum class ePIDusage { HEIGHT, ANGLE };

class TUBEUE_API UPID{
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
		ePIDusage usage;
	public:
		static constexpr double idealPBall = 0.75; // 0.75 prvotna hodnota vypocitana -> // 1.19f // 7.55f | 2.0f
		static constexpr double idealIBall = 0.25; //0.25 1.315f // 0.033f | 0.85f
		static constexpr double idealDBall = 0.05; //0.1 0.329f // 0.008f | 0.5f
		static constexpr double idealPServo = 18.0;
		static constexpr double idealIServo = 8.0; 
		static constexpr double idealDServo = 16.0;
		UPID(ePIDusage usage);
		bool detectChange(double P,
						  double I,
						  double D);
		bool setPIDvalues(double P,
						  double I,
						  double D);
		void reset();
		bool setIdealPIDvalues();
		void setSaturationLimits(double min, double max);
		double getPIDOutput(double currentError, double deltaTime);
		double getPIDOutputOverload(double currentError, double deltaTime);
		static double estimateError(double desiredPos, double currentPos);
		template <typename T> static int sgn(T val);
		//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};