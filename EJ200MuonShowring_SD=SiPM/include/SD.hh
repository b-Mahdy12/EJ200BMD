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
    void EndOFEvent(G4HCofThisEvent *HCE);

private:
    WLSHitCollection *hitCollection;
};

#endif