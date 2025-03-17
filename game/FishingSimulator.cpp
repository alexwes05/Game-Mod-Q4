#include "../idlib/precompiled.h"
#pragma hdrstop

#include "Game_local.h"
#include "FishingSimulator.h"
#include "Player.h"
#include "Weapon.h"
//#include "../ui/UserInterface.h"

/*
===============================================================================

    FishingSimulator class implementation.

===============================================================================
*/
/*
CLASS_DECLARATION(idEntity, FishingSimulator)   // Assuming idEntity is the base class if necessary
END_CLASS
*/

//FISHING CONSTRUCTOR
FishingSimulator::FishingSimulator(idPlayer* p) {
    player = p;
    fishCaught = 0; 
    bait = 10; 
    isFishing = false;
    fishingTime = 0;
    nextReel = 500;
    fishOnHook = false;
    displayMessage = true;
    //hud = player->hud;

        //player->hud;
}

/*
     .·:*¨ Fishing Mechanics ¨*:·.
        
*/
void FishingSimulator::UpdateFishing() {
    if (!isFishing) return;

    if (fishingTime <= gameLocal.time && displayMessage) {
        //gameLocal.Printf("A fish is biting! Fire to reel it in!\n");
        player->ChangeFishText("A fish is biting! Fire to reel it in!");
        player->ShowFishReeling();
        gameLocal.Printf("hud\n");
        //hud->SetStateBool("fishOnHook::visible", true);
        //hud->SetStateInt("fishOnHook::visible", 1);
        fishOnHook = true;
        displayMessage = false;
    }
}

void FishingSimulator::CastRod() {
    if (bait <= 0) {
        gameLocal.Printf("You have no bait left!\n");
        player->ChangeFishText("You have no bait left!");
        return;
    }
    displayMessage = true;
    isFishing = true;
    bait--;
    castTime = gameLocal.time;
    fishingTime = gameLocal.random.RandomInt(10000) + 2000 + castTime; // Random time between 2-5 seconds plus time casted
    player->ChangeFishText("You cast your fishing rod...");
}

void FishingSimulator::setIsFishing(bool fish) {
    isFishing = fish;
}

bool FishingSimulator::GetIsFishing( void ) const {
    return isFishing;
}

void FishingSimulator::CatchFish() {
    //hud->SetStateString("gui::fishOnHook::visible", "0");
    //hud->HandleNamedEvent("hideFishReel");
    player->HideFishReeling();
    if (!fishOnHook) {
        player->ChangeFishText("You reeled in too early! The fish got away.");
        gameLocal.Printf("You reeled in too early! The fish got away.\n");
        isFishing = false;
        return;
    }

    int reward;
    int fishType = gameLocal.random.RandomInt(100); // Random number 0-99
    if (fishType < 50) {
        player->ChangeFishText("You caught a small fish!");
        gameLocal.Printf("You caught a small fish!\n");
        reward = 5;
    }
    else if (fishType < 80) {
        player->ChangeFishText("You caught a medium fish!");
        gameLocal.Printf("You caught a medium fish!\n");
        reward = 15;
    }
    else {
        player->ChangeFishText("You caught a rare fish!");
        gameLocal.Printf("You caught a rare fish!\n");
        reward = 50;
    }
    gameLocal.Printf("Gained %d$\nTotal money: %d + %d ", reward, player->inventory.money, reward);
    player->inventory.money += reward;
    gameLocal.Printf("= %d$\n", player->inventory.money);

    fishCaught++;
    isFishing = false;
    fishOnHook = false;
}



/*
arg = item->spawnArgs.MatchPrefix( "inv_armor", NULL );
    if ( arg && hud ) {
        hud->HandleNamedEvent( "armorPulse" );
    }
    = idPlayer::hud;
    */
/*
    .·:*¨ Shop ¨*:·.
*/


void FishingSimulator::BuyBait(int amount) {
    if (player->inventory.money >= amount * 5) {  // 5 per bait
        player->ChangeMoney(-amount * 5);  // Deduct money
        bait += amount;  // Add bait
        gameLocal.Printf("Bought %d bait.\n", amount);
    }
    else {
        gameLocal.Printf("Not enough money to buy bait!\n");
    }
}
