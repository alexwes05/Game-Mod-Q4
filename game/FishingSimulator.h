#ifndef __FISHING_SIMULATOR_H__
#define __FISHING_SIMULATOR_H__

// Forward declaration of idPlayer
class idPlayer;

class FishingSimulator {
private:
    idPlayer* player; 
    int fishCaught;
    int bait;
    int nextReel;
    bool isFishing;
    int fishingTime;
    int castTime;
    bool initialized;
    bool fishOnHook;
    bool displayMessage;

public:
    FishingSimulator(idPlayer* p);
    void CastRod();
    void UpdateFishing();
    void setIsFishing(bool fish);
    bool GetIsFishing( void ) const;
    void CatchFish();
    void BuyBait(int amount);

    //implement save and load later for bait and fish in inventory
    //void Save(idSaveGame* savefile) const;
    //void Load(idRestoreGame* savefile);
};

#endif  // FISHING_SIMULATOR_H
