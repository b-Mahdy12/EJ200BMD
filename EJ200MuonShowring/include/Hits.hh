#ifndef HITCOLLECTION_HH
#define HITCOLLECTION_HH

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"

class MuonHit : public G4VHit
{
public:
    MuonHit(const G4int layer);
    ~MuonHit();

    void Print();
    // void AddEdep(const double e) { eDep += e; }

    // G4double GetEdep() const { return eDep; }
    G4int GetLayerNumber() const { return layerNumber; }

private:
    const G4int layerNumber;
    G4double eDeb;
};

typedef G4THitsCollection<MuonHit> HitCollection;

#endif