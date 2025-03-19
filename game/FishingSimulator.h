#ifndef __FISHING_SIMULATOR_H__
#define __FISHING_SIMULATOR_H__

class idPlayer;


enum FishingItem {
    ITEM_NONE,
    ITEM_PEARL,
    ITEM_GOLDEN_COIN,
    ITEM_MERMAID_PENDANT,
    ITEM_LUCKY_CHARM,
    ITEM_MAGICAL_FISH
};


class FishingSimulator {
private:
    idPlayer* player; 
    
    int nextReel;
    bool isFishing;
    int fishingTime;
    int castTime;
    bool initialized;
    bool fishOnHook;
    bool displayMessage;
    // Upgrades
    float baitPower;   // Increases the chance of catching larger fish
    float rodPower;    // Reduces fishing time
    int fishAmount;    // Increases the number of fish caught per reel
    float luckBonus;   // Increases rare fish catch chance
    int upgradeLevels[5];
    int mermaidMoneyMultiplier;
    bool hasPearl;
    bool hasLuckyCharm;
    float       yaw;


public:
    int fishCaught;
    int bait;
    int moneyMultiplier;
    int maxBait;
    //idUserInterface* hud;				// Common hud
    FishingSimulator(idPlayer* p);


    // Fishing Mechanics
    void UpdateFishing();
    void CastRod();
    void CatchFish();


    void FishUpItem(FishingItem item);

    void Upgrade(const char* upgradeName); // Upgrade by name


    // Getters & Setters
    void setIsFishing(bool fish);
    bool GetIsFishing() const;

    // Shop System
    void BuyBait(int amount);



    // Upgrades
    /*
    void UpgradeBaitPower(float amount);
    void UpgradeMaxBait(int amount);
    void UpgradeRodPower(float amount);
    
    void UpgradeLuckBonus(float amount);*/

    void UpgradeFishAmount(int amount);

    // Save & Load
    void Save(idSaveGame* savefile) const;
    void Restore(idRestoreGame* savefile);

    void DisplayUpgradeInfo();

};

#endif  // FISHING_SIMULATOR_H
