#ifndef SENDET_HH
#define SENDET_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "g4root.hh"

#include "Hits.hh"

class SenDet : public G4VSensitiveDetector
{
public:
    SenDet(G4String name);
    ~SenDet();

    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);

    void Initialize(G4HCofThisEvent *HCE);
    void EndOfEvent(G4HCofThisEvent *HCE);

private:
    SiPMHitCollection *hitCollection;
    G4int hitCounter;
    G4String volName;
    std::vector<G4double> WLS_ID;
    std::vector<G4double> SiPM_ID;

    std::vector<G4double> SiPM_top_timeData;
    std::vector<G4double> SiPM_top_energyData;
    std::vector<G4double> SiPM_bottom_timeData;
    std::vector<G4double> SiPM_bottom_energyData;
};

#endif