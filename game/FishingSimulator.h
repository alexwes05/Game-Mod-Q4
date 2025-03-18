#ifndef __FISHING_SIMULATOR_H__
#define __FISHING_SIMULATOR_H__

class idPlayer;

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
    

public:
    int fishCaught;
    int bait;
    int moneyMultiplier;
    int maxBait;
    //idUserInterface* hud;				// Common hud
    FishingSimulator(idPlayer* p);
    void CastRod();
    void UpdateFishing();
    void setIsFishing(bool fish);
    bool GetIsFishing( void ) const;
    void CatchFish();
    void BuyBait(int amount);
    void Save(idSaveGame* savefile) const;
    void Restore(idRestoreGame* savefile);


    //implement save and load later for bait and fish in inventory
    //void Save(idSaveGame* savefile) const;
    //void Load(idRestoreGame* savefile);
};

#endif  // FISHING_SIMULATOR_H
