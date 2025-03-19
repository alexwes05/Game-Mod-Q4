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
const int MAX_UPGRADES = 5; // Maximum upgrade levels
const int BASE_COST = 50;   // Starting cost




FishingSimulator::FishingSimulator(idPlayer* p) {
    player = p;
    fishCaught = 0;
    bait = 10;
    maxBait = 10;
    isFishing = false;
    fishingTime = 0;
    nextReel = 500;
    fishOnHook = false;
    displayMessage = true;
    moneyMultiplier = 1;
    mermaidMoneyMultiplier = 1;
    hasPearl = false;
    hasLuckyCharm = false;


    baitPower = 1.0f;  // Base multiplier for larger fish
    rodPower = 1.0f;   // Faster reeling speed
    fishAmount = 1;    // Base number of fish per reel
    luckBonus = 0.0f;  // Increases rare fish catch chance

    for (int i = 0; i < 5; i++) {
        upgradeLevels[i] = 0;  // Track how many times each upgrade has been purchased
    }


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
        //gameLocal.Printf("hud\n");
        //hud->SetStateBool("fishOnHook::visible", true);
        //hud->SetStateInt("fishOnHook::visible", 1);
        fishOnHook = true;
        displayMessage = false;
    }
}


void FishingSimulator::CastRod() {
    if (bait <= 0) {
        player->ChangeFishText("You have no bait left!");
        return;
    }

    displayMessage = true;
    isFishing = true;
    bait--;

    // Adjust fishing time based on Rod Power
    int minTime = 2000 / rodPower;
    int maxTime = 10000 / rodPower;
    fishingTime = gameLocal.random.RandomInt(maxTime) + minTime + gameLocal.time;

    player->ChangeFishText("You cast your fishing rod...");
}


void FishingSimulator::setIsFishing(bool fish) {
    isFishing = fish;
}

bool FishingSimulator::GetIsFishing( void ) const {
    return isFishing;
}

void FishingSimulator::CatchFish() {
    player->HideFishReeling();

    if (!fishOnHook) {
        player->ChangeFishText("You reeled in too early! The fish got away.");
        gameLocal.Printf("You reeled in too early! The fish got away.\n");
        isFishing = false;
        return;
    }

    int totalMoneyGained = 0;

    //int base = moneyMultiplier;

    for (int i = 0; i < fishAmount; i++) {  // Increase fish caught per reel
        int fishType = gameLocal.random.RandomInt(100);
        int reward = 0;
        const char* fishName;

        // Randomly fish up a special item
        int itemChance = gameLocal.random.RandomInt(100);
        if (itemChance < 5) {
            FishingItem item = static_cast<FishingItem>(gameLocal.random.RandomInt(5) + 1);
            FishUpItem(item);
        }

        int monsterChance = gameLocal.random.RandomInt(100);
        if (monsterChance < 10) {  // 10% chance to fish up a monster_strogg_marine
            float yaw = player->viewAngles.yaw;
            //idVec3		org;
            idDict		dict;
            
            
            const char* monsterClassName = "monster_strogg_marine";  // Replace with your desired monster
            dict.Set("classname", monsterClassName);
            dict.Set("angle", va("%f", yaw + 180));  // Set spawn angle

            // Calculate the spawn position based on player's position
            idVec3 org = player->GetPhysics()->GetOrigin() + idAngles(0, yaw, 0).ToForward() * 80 + idVec3(0, 0, 1);
            dict.Set("origin", org.ToString());  // Set spawn position

            // Now spawn the entity
            idEntity* newEntity = nullptr;
            gameLocal.SpawnEntityDef(dict, &newEntity);

            // Check if the spawn was successful
            if (newEntity) {
                gameLocal.Printf("You fished up a monster: %s!\n", monsterClassName);
                player->ChangeFishText("You fished up a Strogg Marine! Get ready to fight!");
            }
            else {
                gameLocal.Printf("Failed to spawn the monster.\n");
            }

            /*
            const char* key, * value;
            float		yaw;
            idVec3		org;
            idPlayer* player;
            idDict		dict;

            player = gameLocal.GetLocalPlayer();
            if (!player || !gameLocal.CheatsOk(false)) {
                return;
            }

            yaw = player->viewAngles.yaw;

            value = args.Argv(1);
            dict.Set("classname", value);
            dict.Set("angle", va("%f", yaw + 180));

            org = player->GetPhysics()->GetOrigin() + idAngles(0, yaw, 0).ToForward() * 80 + idVec3(0, 0, 1);
            dict.Set("origin", org.ToString());*/

            /*
            const char* key, * value;
            idDict		dict;
            idVec3		org;
            yaw = player->viewAngles.yaw;

            dict.Set("classname", "monster_strogg_marine");
            dict.Set("angle", va("%f", yaw + 180));

            org = player->GetPhysics()->GetOrigin() + idAngles(0, yaw, 0).ToForward() * 80 + idVec3(0, 0, 1);
            dict.Set("origin", org.ToString());
            yaw = player->viewAngles.yaw;
            */
            // Inform the player
            player->ChangeFishText("You fished up a Strogg Marine! Get ready to fight!");
            gameLocal.Printf("You fished up a monster: monster_strogg_marine!\n");
        }


        // Apply Bait Power to increase large fish chances
        if (fishType < 50 - (baitPower * 10)) {
            fishName = "small fish";
            reward = 5;
        }
        else if (fishType < 80 - (baitPower * 5)) {
            fishName = "medium fish";
            reward = 15;
        }
        else {
            if (gameLocal.random.RandomFloat() < luckBonus) { // Luck Bonus
                fishName = "legendary fish";
                reward = 200;
            }
            else {
                fishName = "rare fish";
                reward = 50;
            }
        }

        // Update money and display messages
        player->inventory.money += reward;
        totalMoneyGained += reward;
        gameLocal.Printf("You caught a %s! Gained %d$.\n", fishName, reward);
        player->ChangeFishText(va("You caught a %s!", fishName));

        
    }
    totalMoneyGained= totalMoneyGained * mermaidMoneyMultiplier;
    mermaidMoneyMultiplier = 1;

    player->ChangeFishText(va("Total money gained from this reel: %d$. New total: %d$.\n",
        totalMoneyGained, player->inventory.money));
    gameLocal.Printf("Total money gained from this reel: %d$. New total: %d$.\n",
        totalMoneyGained, player->inventory.money);

    fishCaught++;
    isFishing = false;
    fishOnHook = false;
}


void FishingSimulator::FishUpItem(FishingItem item) {
    switch (item) {
    case ITEM_PEARL:
        if(!hasPearl){
            player->ChangeFishText("You fished up a Pearl! Your luck has permanently increased by 0.05!");
            luckBonus += 0.05f;
            gameLocal.Printf("You fished up a Pearl! Luck bonus increased by 0.05!\n");
            hasPearl = true;
        }
        break;
    case ITEM_GOLDEN_COIN:
        player->ChangeFishText("You fished up a Golden Coin! You've gained 500$!");
        player->inventory.money += 500;
        gameLocal.Printf("You fished up a Golden Coin! Gained 500$\n");
        break;
    case ITEM_MERMAID_PENDANT:
        player->ChangeFishText("You fished up a Mermaid Pendant! Your profits for this catch will be doubled!");
        mermaidMoneyMultiplier *= 2.0;  // Multiplier of 2 for this turn
        gameLocal.Printf("You fished up a Mermaid Pendant! Profits will be doubled for this catch!\n");
        break;
    case ITEM_LUCKY_CHARM:
        if(!hasLuckyCharm){
            player->ChangeFishText("You fished up a Lucky Charm! Your luck bonus has increased by 0.1!");
            luckBonus += 0.1f;
            gameLocal.Printf("You fished up a Lucky Charm! Luck bonus increased by 0.1!\n");
            hasLuckyCharm = true;
        }
        break;
    case ITEM_MAGICAL_FISH:
        player->ChangeFishText("You fished up a Money Fish! You've gained 1000$");
        player->inventory.money += 1000;
        // You could add more functionality to repair the rod here if needed.
        gameLocal.Printf("You fished up a Money Fish! You've gained 1000$\n");
        break;
    default:
        break;
    }
}

void FishingSimulator::Upgrade(const char* upgradeName) {
    int upgradeIndex = -1;

    if (strcmp(upgradeName, "baitPower") == 0) upgradeIndex = 0;
    else if (strcmp(upgradeName, "maxBait") == 0) upgradeIndex = 1;
    else if (strcmp(upgradeName, "rodPower") == 0) upgradeIndex = 2;
    else if (strcmp(upgradeName, "fishAmount") == 0) upgradeIndex = 3;
    else if (strcmp(upgradeName, "luckBonus") == 0) upgradeIndex = 4;

    if (upgradeIndex == -1) {
        gameLocal.Printf("Invalid upgrade name: %s\n", upgradeName);
        gameLocal.Printf("Valid upgrade names: baitPower, maxBait, rodPower, fishAmount, luckBonus\n");
        return;
    }

    // Calculate the cost: 50 * (10 ^ current level)
    int cost = 50;
    for (int i = 0; i < upgradeLevels[upgradeIndex]; i++) {
        cost *= 10;
    }

    if (player->inventory.money < cost) {
        gameLocal.Printf("Not enough money! Upgrade %s costs %d$\n", upgradeName, cost);
        return;
    }

    // Deduct money & apply upgrade
    player->inventory.money -= cost;
    upgradeLevels[upgradeIndex]++;

    switch (upgradeIndex) {
    case 0: baitPower += 0.1f; break;
    case 1: maxBait += 5; break;
    case 2: rodPower += 0.2f; break;
    case 3: fishAmount += 1; break;
    case 4: luckBonus += 0.05f; break;
    }

    gameLocal.Printf("Upgraded %s to level %d! Remaining money: %d$\n", upgradeName, upgradeLevels[upgradeIndex], player->inventory.money);
}

/*
void FishingSimulator::UpgradeBaitPower(float amount) { baitPower += amount; }
void FishingSimulator::UpgradeMaxBait(int amount) { maxBait += amount; }
void FishingSimulator::UpgradeRodPower(float amount) { rodPower += amount; }
void FishingSimulator::UpgradeLuckBonus(float amount) { luckBonus += amount; }
*/

void FishingSimulator::UpgradeFishAmount(int amount) { fishAmount += amount; }

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

/*
    .·:*¨ Save/Restore ¨*:·.
*/



void FishingSimulator::Save(idSaveGame* savefile) const {
    // Save all relevant variables
    savefile->WriteInt(fishCaught);
    savefile->WriteInt(bait);
    savefile->WriteInt(maxBait);
    savefile->WriteInt(moneyMultiplier);
    savefile->WriteInt(mermaidMoneyMultiplier);
    savefile->WriteBool(hasPearl);
    savefile->WriteBool(hasLuckyCharm);

    savefile->WriteFloat(baitPower);
    savefile->WriteFloat(rodPower);
    savefile->WriteInt(fishAmount);
    savefile->WriteFloat(luckBonus);

    for (int i = 0; i < 5; i++) {
        savefile->WriteInt(upgradeLevels[i]);  // Saving upgrade levels
    }
}

void FishingSimulator::Restore(idRestoreGame* savefile) {
    // Restore all relevant variables
    savefile->ReadInt(fishCaught);
    savefile->ReadInt(bait);
    savefile->ReadInt(maxBait);
    savefile->ReadInt(moneyMultiplier);
    savefile->ReadInt(mermaidMoneyMultiplier);
    savefile->ReadBool(hasPearl);
    savefile->ReadBool(hasLuckyCharm);

    savefile->ReadFloat(baitPower);
    savefile->ReadFloat(rodPower);
    savefile->ReadInt(fishAmount);
    savefile->ReadFloat(luckBonus);

    for (int i = 0; i < 5; i++) {
        savefile->ReadInt(upgradeLevels[i]);  // Restoring upgrade levels
    }
}


/*
void FishingSimulator::Save(idSaveGame* savefile) const {
    savefile->WriteInt(fishCaught);
    savefile->WriteInt(bait);

}

void FishingSimulator::Restore(idRestoreGame* savefile) {
    savefile->ReadInt(fishCaught);
    savefile->ReadInt(bait);


}*/


void FishingSimulator::DisplayUpgradeInfo() {
    gameLocal.Printf("=== Upgrade Info ===\n");

    // Bait Power
    int baitPowerCost = 50 * pow(10, upgradeLevels[0]);
    gameLocal.Printf("Bait Power (Level %d): Increases chance of large fish\nNext Level Cost: %d$\n\n", upgradeLevels[0], baitPowerCost);

    // Max Bait
    int maxBaitCost = 50 * pow(10, upgradeLevels[1]);
    gameLocal.Printf("Max Bait (Level %d): Increases the maximum amount of bait you can carry\nNext Level Cost: %d$\n\n", upgradeLevels[1], maxBaitCost);

    // Rod Power
    int rodPowerCost = 50 * pow(10, upgradeLevels[2]);
    gameLocal.Printf("Rod Power (Level %d): Increases reel speed and effectiveness\nNext Level Cost: %d$\n\n", upgradeLevels[2], rodPowerCost);

    // Fish Amount
    int fishAmountCost = 50 * pow(10, upgradeLevels[3]);
    gameLocal.Printf("Fish Amount (Level %d): Increases the amount of fish caught per reel\nNext Level Cost: %d$\n\n", upgradeLevels[3], fishAmountCost);

    // Luck Bonus
    int luckBonusCost = 50 * pow(10, upgradeLevels[4]);
    gameLocal.Printf("Luck Bonus (Level %d): Increases chance of catching rare or legendary fish\nNext Level Cost: %d$\n\n", upgradeLevels[4], luckBonusCost);

    gameLocal.Printf("====================\n");
}


/*
for (int i = 0; i < numFish; i++) {
    savefile->WriteString(caughtFish[i].name);
    savefile->WriteInt(caughtFish[i].size);
    savefile->WriteInt(caughtFish[i].rarity);
}
*/
//gameLocal.Printf("Saved %d fish.\n", numFish);