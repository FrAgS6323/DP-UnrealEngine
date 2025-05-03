#include "PID.h"

UPID::UPID(ePIDusage usage):
	clamp(false),
	bFirstRun(true),
	usage(usage),
	p(0.0),
	i(0.0),
	d(0.0),
	saturationLimitMin(0.0),
	saturationLimitMax(0.0){
	//PrimaryComponentTick.bCanEverTick = true;
	this->kP = ePIDusage::HEIGHT == this->usage ? UPID::idealPBall : UPID::idealPServo,
	this->kI = ePIDusage::HEIGHT == this->usage ? UPID::idealIBall : UPID::idealIServo,
	this->kD = ePIDusage::HEIGHT == this->usage ? UPID::idealDBall : UPID::idealDServo;
}

#if 0
void UPID::BeginPlay(){
	Super::BeginPlay();	
}
#endif

bool UPID::detectChange(double P,
						double I,
						double D) {
	return this->kP != P ||
			this->kI != I ||
			this->kD != D;
}

bool UPID::setPIDvalues(double P,
						double I,
						double D) {
	bool change = this->detectChange(P, I, D);

	if (change) {
		this->kP = P; // / 100.0f
		this->kI = I; // * 10.0f
		this->kD = D; // * 10.0f
	}
	return change;
}

void UPID::reset(){
	this->p = 0.0;
	this->i = 0.0;
	this->d = 0.0;
	this->error = 0.0;
}

void UPID::setSaturationLimits(double min, double max){
	this->saturationLimitMin = min;
	this->saturationLimitMax = max;
}

bool UPID::setIdealPIDvalues() {
	double idealP = ePIDusage::HEIGHT == this->usage ? UPID::idealPBall : UPID::idealPServo,
		   idealI = ePIDusage::HEIGHT == this->usage ? UPID::idealIBall : UPID::idealIServo,
		   idealD = ePIDusage::HEIGHT == this->usage ? UPID::idealDBall : UPID::idealDServo;
	bool change = this->detectChange(idealP,
									 idealI,
									 idealD);
	if (change) {
		this->kP = idealP;
		this->kI = idealI;
		this->kD = idealD;
	}
	return change;
}

double UPID::getPIDOutput(double currentError, double deltaTime) {
	this->p = currentError;

	if (!this->clamp) this->i += currentError * deltaTime;
	
	if (this->bFirstRun) {
		this->error = currentError;
		this->d = 0.0;
		this->bFirstRun = false;
	}else{
		this->d = (currentError - this->error) / deltaTime;
		this->error = currentError;
	}

	double preSaturationFilterValue = this->p * this->kP +
		this->i * this->kI +
		this->d * this->kD,
		output = std::clamp(preSaturationFilterValue,
			this->saturationLimitMin,
			this->saturationLimitMax);

	if (preSaturationFilterValue != output) {
		if (UPID::sgn(currentError) == UPID::sgn(preSaturationFilterValue)) this->clamp = true;
		else this->clamp = false;
	}
	else {
		this->clamp = false;
	}
	return output;
}

double UPID::getPIDOutputOverload(double currentError, double deltaTime){
	this->p = currentError;

	//if(!this->clamp) this->i += currentError * deltaTime;
	this->i += currentError * deltaTime;

	this->d = (currentError - this->error) / deltaTime;

	this->error = currentError;

	double preSaturationFilterValue = this->saturationLimitMin + (this->p * this->kP +
		this->i * this->kI +
		this->d * this->kD),
		output = std::clamp(preSaturationFilterValue,
			this->saturationLimitMin,
			this->saturationLimitMax);

	if (preSaturationFilterValue != output) {
		if (UPID::sgn(currentError) == UPID::sgn(preSaturationFilterValue)) this->clamp = true;
		else this->clamp = false;
	}
	else {
		this->clamp = false;
	}
#if 0
	std::cout << "clamp: " << this->clamp <<
		" preSaturationFilterValue: " << preSaturationFilterValue <<
		" output: " << output;
#endif
	return output;
}

double UPID::estimateError(double desiredPos, double currentPos) {
	return desiredPos - currentPos;
}

template <typename T> static int UPID::sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

#if 0
void UPID::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
#endif