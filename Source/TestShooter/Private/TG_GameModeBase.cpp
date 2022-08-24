// TestShooter. All rights reserved.


#include "TG_GameModeBase.h"
#include "Player/TSBase_Character.h"
#include "Player/TS_PlayerController.h"

ATG_GameModeBase::ATG_GameModeBase() {
    DefaultPawnClass = ATSBase_Character::StaticClass();
    PlayerControllerClass = ATS_PlayerController::StaticClass();
}
