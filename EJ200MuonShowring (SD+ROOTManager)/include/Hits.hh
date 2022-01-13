#ifndef HITCOLLECTION_HH
#define HITCOLLECTION_HH

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"

class G4AttDef;
class G4AttValue;

class MuonHit : public G4VHit
{
public:
    MuonHit();
    MuonHit(G4int iLayerID);
    MuonHit(const MuonHit &right);
    virtual ~MuonHit();

    const MuonHit &operator=(const MuonHit &right);
    G4bool operator==(const MuonHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    virtual void Draw();
    virtual void Print();

    virtual const std::map<G4String, G4AttDef> *GetAttDefs() const;
    virtual std::vector<G4AttValue> *CreateAttValues() const;

    void SetEdep(G4double de) { eDep = de; };
    void AddEdep(const double e) { eDep += e; };
    G4double GetEdep() const { return eDep; };

    void SetLayerID(G4int z) { flayerID = z; };
    G4int GetLayerNumber() const { return flayerID; };

    void SetPos(G4ThreeVector xyz) { fPos = xyz; };
    G4ThreeVector GetPos() const { return fPos; };

    void SetTrackID(G4int id) { fTrackID = id; };
    G4int GetTrackID() const { return fTrackID; };

private:
    G4int flayerID;
    G4double eDep;
    G4ThreeVector fPos;
    G4int fTrackID;
};

typedef G4THitsCollection<MuonHit> WLSHitCollection;
// typedef G4THitsCollection<MuonHit> EJ200HitCollection;

extern G4ThreadLocal G4Allocator<MuonHit> *WLSHitAllocator;

inline void *MuonHit::operator new(size_t)
{
    if (!WLSHitAllocator)
    {
        WLSHitAllocator = new G4Allocator<MuonHit>;
    }
    return (void *)WLSHitAllocator->MallocSingle();
};

inline void MuonHit::operator delete(void *aHit)
{
    WLSHitAllocator->FreeSingle((MuonHit *)aHit);
};
#endif