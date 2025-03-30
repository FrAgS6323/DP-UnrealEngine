#include "TubeUEGameModeBase.h"

ATubeUEGameModeBase::ATubeUEGameModeBase(){
    if (DefaultPawnClass == ADefaultPawn::StaticClass())
        DefaultPawnClass = ASpectatorPawn::StaticClass();
}