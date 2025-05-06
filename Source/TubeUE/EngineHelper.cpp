#include "EngineHelper.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include <numbers>
#include "GameFramework/SpectatorPawn.h"
#include "GameFramework/FloatingPawnMovement.h"

void UEngineHelper::loadMeshStatic(const TCHAR* path, UStaticMeshComponent* parentComp){
	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(path);

	if (meshAsset.Succeeded()){
		parentComp->SetStaticMesh(meshAsset.Object);
	}else{
		UE_LOG(LogTemp, Error, TEXT("Failed to load TurtlebotBody mesh!"));
	}
}

void UEngineHelper::loadMeshDynamic(const TCHAR* path, UStaticMeshComponent *parentComp){
	UStaticMesh* meshAsset = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, path));

	if (meshAsset) {
		UE_LOG(LogTemp, Warning, TEXT("Mesh loaded successfully (dynamic): %s"), *meshAsset->GetName());
		parentComp->SetStaticMesh(meshAsset);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Mesh not loaded!"));
	}
}

auto UEngineHelper::loadMaterialDynamic(const TCHAR* path) -> UMaterialInterface* {
	UMaterialInterface* mat = LoadObject<UMaterialInterface>(nullptr, path);

#if 0
	if (mat && mesh) {
		//UE_LOG(LogTemp, Warning, TEXT("Material loaded successfully (dynamic): %s"), *mat->GetName());
		mesh->SetMaterial(slot, mat);
	}
	else {
		//UE_LOG(LogTemp, Error, TEXT("Material not loaded!"));
	}
#endif
	return mat;
}

void UEngineHelper::setupConstraint(UPhysicsConstraintComponent* constraint,
									USceneComponent* attachToComponent,
									UPrimitiveComponent* stillComponent,
									UPrimitiveComponent* moveableComponent,
									ELinearConstraintMotion xMotionType,
									float xLimit,
									ELinearConstraintMotion yMotionType,
									float yLimit,
									ELinearConstraintMotion zMotionType,
									float zLimit,
									EAngularConstraintMotion swing1MotionType,
									float swing1LimitAngle,
									EAngularConstraintMotion swing2MotionType,
									float swing2LimitAngle,
									EAngularConstraintMotion twistMotionType,
									float twistLimitAngle){
	if (!constraint){
		UE_LOG(LogTemp, Error, TEXT("Constraint not initialized!"));
		return;
	}

	if (!stillComponent || !moveableComponent){
		UE_LOG(LogTemp, Error, TEXT("Constraint not set: One or both constrained components are NULL!"));
		return;
	}

	constraint->SetupAttachment(attachToComponent);
	constraint->SetConstrainedComponents(moveableComponent, 
										 NAME_None,
										 stillComponent,
										 NAME_None);
	constraint->SetLinearXLimit(xMotionType, xLimit);
	constraint->SetLinearYLimit(yMotionType, yLimit);
	constraint->SetLinearZLimit(zMotionType, zLimit);
	//constraint->SetLinearPositionDrive(true, true, true);
	//constraint->SetLinearDriveParams(10000.0f, 1000.0f, 0.0f);  // Stiffness, Damping, Force Limit
	//constraint->SetAngularDriveParams(5000.0f, 500.0f, 0.0f);

	constraint->SetAngularSwing1Limit(swing1MotionType, swing1LimitAngle);
	constraint->SetAngularSwing2Limit(swing2MotionType, swing2LimitAngle);
	constraint->SetAngularTwistLimit(twistMotionType, twistLimitAngle);
}

auto UEngineHelper::performRaycast(UWorld *world,
								   UStaticMeshComponent *mesh,
								   FVector startOffset, 
								   FVector rayDirection,
								   TArray<UPrimitiveComponent*> meshesToExclude,
								   bool enableDebugRay,
								   double rayLength,
								   double &hitDistance) -> bool {
	FVector startVec = mesh->GetComponentLocation() + startOffset,
			endVec = (rayDirection * rayLength) + startVec;

	FHitResult hitResult;
	FCollisionQueryParams collisionParams;

	if(!meshesToExclude.IsEmpty())  collisionParams.AddIgnoredComponents(meshesToExclude);

	bool bIsHit = world->LineTraceSingleByChannel(hitResult,
												  startVec,
												  endVec,
												  ECC_Visibility,
												  collisionParams);
	if (bIsHit) {
		//UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *hitResult.GetActor()->GetName());

		hitDistance = hitResult.Distance;
		//UE_LOG(LogTemp, Warning, TEXT("Hit Distance: %f"), this->distance);
		//DrawDebugLine(GetWorld(), startVec, endVec, FColor::Red, false, 1, 0, 1);
		// 
		if(enableDebugRay) DrawDebugPoint(world, hitResult.Location, 10, FColor::Green, false, 1);
	}else{
		if(enableDebugRay) DrawDebugLine(world, startVec, endVec, FColor::Red, false, 1, 0, 1);
	}
	return bIsHit;
}

auto UEngineHelper::degToRad(double deg) -> double {
		return (deg * PI) / 180;
}

auto UEngineHelper::radToDeg(double rad) -> double {
	return (rad * 180) / std::numbers::pi;
}

auto UEngineHelper::findActorsOfClass(const UObject* worldContextObject, TSubclassOf<AActor> actorClass, size_t index) -> TObjectPtr<AActor> {
	TArray<AActor*> foundActors;
	TObjectPtr<AActor> outActor;
	UGameplayStatics::GetAllActorsOfClass(worldContextObject, actorClass, foundActors);

	if (foundActors.Num() > 0) {
		outActor = foundActors[index];
		//UE_LOG(LogTemp, Warning, TEXT("tube initialized in level!"));
	}
	else {
		//UE_LOG(LogTemp, Warning, TEXT("tube NOT initialized in level!"));
	}
	return outActor;
}

void UEngineHelper::setSpectatorCameraSpeed(APlayerController* playerController, float speed){
	ASpectatorPawn* spectatorPawn = Cast<ASpectatorPawn>(playerController->GetPawn());

	if (spectatorPawn){
		UFloatingPawnMovement* MoveComp = spectatorPawn->FindComponentByClass<UFloatingPawnMovement>();
		if (MoveComp) MoveComp->MaxSpeed = speed;
	}
}

void UEngineHelper::setKinematicTarget(UStaticMeshComponent *component, bool bState){
	if (bState){
		component->SetSimulatePhysics(false);
		component->SetMobility(EComponentMobility::Movable);
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}else{
		component->SetPhysicsLinearVelocity(FVector::ZeroVector);
		component->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		component->SetSimulatePhysics(true);
	}
}

void UEngineHelper::Profiler::profileFPS(bool bScreen, float deltaTime, FColor color){
	double fps = 1.0f / deltaTime;

	if (bScreen) {
		FString FPSstring = FString::Printf(TEXT("FPS: %.2f"), fps);
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FPSstring);
	}
	else{
		UE_LOG(LogTemp, Warning, TEXT("FPS: %f"), fps);
	}
	UEngineHelper::Profiler::totalFPS += fps;
}

void UEngineHelper::Profiler::profileRAM(bool bScreen, float delatTime, FColor color){
	const FPlatformMemoryStats Stats = FPlatformMemory::GetStats();
	float RAMusage = Stats.UsedPhysical / (1024.0f * 1024.0f),
		  peakRAM = Stats.PeakUsedPhysical / (1024.0f * 1024.0f),
		VRAMusage = Stats.UsedVirtual / (1024.0f * 1024.0f);

	if (bScreen) {
		FString RAMPhysicalString = FString::Printf(TEXT("Used Physical RAM: %.2f MB"), RAMusage),
			RAMPeakPhysicalString = FString::Printf(TEXT("Peak Used Physical RAM: %.2f MB"),peakRAM),
			RAMVirtualString = FString::Printf(TEXT("Used Virtual RAM: %.2f MB"), VRAMusage);

		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 0.f, color, RAMPhysicalString);
			GEngine->AddOnScreenDebugMessage(-1, 0.f, color, RAMPeakPhysicalString);
			GEngine->AddOnScreenDebugMessage(-1, 0.f, color, RAMVirtualString);
		}

	}
	else{
		UE_LOG(LogTemp, Warning, TEXT("Used Physical RAM: %.2f MB"), RAMusage);
		UE_LOG(LogTemp, Warning, TEXT("Peak Used Physical RAM: %.2f MB"), peakRAM);
		UE_LOG(LogTemp, Warning, TEXT("Used Virtual RAM: %.2f MB"), VRAMusage);
	}
	UEngineHelper::Profiler::totalRAM += RAMusage;
	//UEngineHelper::Profiler::totalVRAM += VRAMusage;
}

void UEngineHelper::Profiler::initCPUprofiler(){
	PdhOpenQuery(NULL, NULL, &cpuQuery);
	PdhAddCounter(cpuQuery, TEXT("\\Processor(_Total)\\% Processor Time"), NULL, &UEngineHelper::Profiler::cpuTotal);
	PdhCollectQueryData(UEngineHelper::Profiler::cpuQuery);
}

void UEngineHelper::Profiler::profileCPU(bool bScreen, float delatTime, FColor color){
	PDH_FMT_COUNTERVALUE counterVal;
	PdhCollectQueryData(cpuQuery);
	PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);

	if (bScreen) {
		FString CPUstring;
		
		if(counterVal.doubleValue > 0) CPUstring = FString::Printf(TEXT("CPU Usage: %.2f%%"), counterVal.doubleValue);
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, color, CPUstring);
	}else {
		UE_LOG(LogTemp, Warning, TEXT("CPU Usage: %.2f%%"), counterVal.doubleValue);
	}
	UEngineHelper::Profiler::totalCPU += counterVal.doubleValue;
}

void UEngineHelper::Profiler::profileGPU(bool bScreen, float delatTime, FColor color){
	IDXGIFactory4* factory = nullptr;
	HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));

	auto logGpuVramUsage = [bScreen, color](float &_totalUsedVRAM, float &_totalBudgetVRAM, float &_totalPercentVRAM){
		IDXGIFactory6* dxgiFactory = nullptr;
		HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
		if (FAILED(hr)) return;

		IDXGIAdapter3* adapter = nullptr;

		for (UINT index = 0; ; ++index){
			IDXGIAdapter1* tempAdapter = nullptr;
			if (dxgiFactory->EnumAdapters1(index, &tempAdapter) == DXGI_ERROR_NOT_FOUND)
				break;

			DXGI_ADAPTER_DESC1 desc;
			tempAdapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE){
				tempAdapter->Release();
				continue;
			}

			tempAdapter->QueryInterface(IID_PPV_ARGS(&adapter));
			tempAdapter->Release();
			break;
		}

			if (!adapter){
				dxgiFactory->Release();
				return;
			}

			DXGI_QUERY_VIDEO_MEMORY_INFO memoryInfo;
			if (SUCCEEDED(adapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &memoryInfo))){
				uint64 usedMB = memoryInfo.CurrentUsage / (1024 * 1024);
				uint64 budgetMB = memoryInfo.Budget / (1024 * 1024);
				float percent = (float)usedMB / (float)budgetMB * 100.0f;
				
				if (bScreen) {
					FString GPUVRAMstring = FString::Printf(TEXT("VRAM used: %llu MB / %llu MB (%.2f%%)"), usedMB, budgetMB, percent);
					if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, color, GPUVRAMstring);
				}else{
					UE_LOG(LogTemp, Warning, TEXT("VRAM used: %llu MB / %llu MB (%.2f%%)"), usedMB, budgetMB, percent);
				}
				_totalUsedVRAM += usedMB;
				_totalBudgetVRAM += budgetMB;
				_totalPercentVRAM += percent;
			}else{
				if (bScreen) {
					FString queryFailed = FString::Printf(TEXT("QueryVideoMemoryInfo failed!"));
					if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, queryFailed);
				}else{
					UE_LOG(LogTemp, Error, TEXT("QueryVideoMemoryInfo failed!"));
				}
			}
	
			adapter->Release();
			dxgiFactory->Release();
	};

	if (FAILED(hr)){
		UE_LOG(LogTemp, Error, TEXT("Failed to create DXGIFactory."));
		return;
	}

	IDXGIAdapter1* adapter = nullptr;

	for (UINT index = 0; factory->EnumAdapters1(index, &adapter) != DXGI_ERROR_NOT_FOUND; ++index){
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE){
			adapter->Release();
			continue;
		}

		FString gpuName(desc.Description);
		uint64 vramMB = desc.DedicatedVideoMemory / (1024 * 1024);
		uint64 sharedMB = desc.SharedSystemMemory / (1024 * 1024);
		
		if (bScreen){
			FString GPUnameString = FString::Printf(TEXT("GPU: %s"), * gpuName),
				VRAMstring = FString::Printf(TEXT("VRAM: %llu MB"), vramMB),
				VRAMVirtualString = FString::Printf(TEXT("Shared system memory: %llu MB"), sharedMB);

			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 0.f, color, GPUnameString);
				GEngine->AddOnScreenDebugMessage(-1, 0.f, color, VRAMstring);
				//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, VRAMVirtualString);
			}

		}else{
			UE_LOG(LogTemp, Warning, TEXT("GPU: %s"), *gpuName);
			UE_LOG(LogTemp, Warning, TEXT("VRAM (dedicated): %llu MB"), vramMB);
			//UE_LOG(LogTemp, Warning, TEXT("Shared system memory: %llu MB"), sharedMB);
		}

		logGpuVramUsage(UEngineHelper::Profiler::totalUsedVRAM,
						UEngineHelper::Profiler::totalBudgetVRAM,
						UEngineHelper::Profiler::totalPercentVRAM);

		adapter->Release();
		break;
	}

	factory->Release();
}

void UEngineHelper::Profiler::profilePerformance(bool bScreen, float deltaTime){
	UEngineHelper::Profiler::profileFPS(bScreen, deltaTime, FColor::Green);
	UEngineHelper::Profiler::profileRAM(bScreen, deltaTime, FColor::Orange);
	UEngineHelper::Profiler::profileCPU(bScreen, deltaTime, FColor::Blue);
	UEngineHelper::Profiler::profileGPU(bScreen, deltaTime, FColor::Red);
	UEngineHelper::Profiler::sampleCount++;
}

void UEngineHelper::Profiler::writeAvgsToLOG(){
	float avgFPS = UEngineHelper::Profiler::totalFPS / UEngineHelper::Profiler::sampleCount,
		avgCPU = UEngineHelper::Profiler::totalCPU / UEngineHelper::Profiler::sampleCount,
		avgRAM = UEngineHelper::Profiler::totalRAM / UEngineHelper::Profiler::sampleCount,
		avgUsedVRAM = UEngineHelper::Profiler::totalUsedVRAM / UEngineHelper::Profiler::sampleCount,
		avgBudgetVRAM = UEngineHelper::Profiler::totalBudgetVRAM / UEngineHelper::Profiler::sampleCount,
		avgPercentVRAM = UEngineHelper::Profiler::totalPercentVRAM / UEngineHelper::Profiler::sampleCount;

	UE_LOG(LogTemp, Warning, TEXT("Avg FPS: %.2f, Avg CPU: %.2f%%, Avg RAM: %.2f MB"), avgFPS, avgCPU, avgRAM);
	UE_LOG(LogTemp, Warning, TEXT("Avg used VRAM: %.2f MB, Avg budget VRAM: %.2f MB, Avg % VRAM: %.2f%%"), avgUsedVRAM, avgBudgetVRAM, avgPercentVRAM);
}