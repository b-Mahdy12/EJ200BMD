#include "Hits.hh"
#include "DetectorConstruction.hh"

#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

G4ThreadLocal G4Allocator<MuonHit> *WLSHitAllocator;

MuonHit::MuonHit() : G4VHit(),
                     flayerID(-1)
{
}

MuonHit::MuonHit(G4int iLayerID) : G4VHit(),
                                   flayerID(iLayerID)
{
}

MuonHit::~MuonHit()
{
}

MuonHit::MuonHit(const MuonHit &right) : G4VHit(),
                                         flayerID(right.flayerID)
{
}

const MuonHit &MuonHit::operator=(const MuonHit &right)
{
  flayerID = right.flayerID;
  return *this;
}

void MuonHit::Draw()
{
}

void MuonHit::Print()
{
  /* G4cout << "LayerID: " << flayerID
           << " Edep: " << eDep / MeV << " (MeV), "
           << " pos: " << fPos / mm << "(mm),"
           << " Track ID: " << fTrackID
           << G4endl;
 */
}

const std::map<G4String, G4AttDef> *MuonHit::GetAttDefs() const
{
  G4bool isNew;
  auto store = G4AttDefStore::GetInstance("MuonHit", isNew);

  if (isNew)
  {
    (*store)["HitType"] = G4AttDef("HitType", "Hit Type", "Physics", "", "G4String");
    (*store)["Layer"] = G4AttDef("Layer", "Layer ID", "Physics", "", "G4int");
    (*store)["Energy"] = G4AttDef("Energy", "Energy deposit", "Physics", "G4BestUnit", "G4double");
    (*store)["Pos"] = G4AttDef("Pos", "Position", "Physics", "G4BestUnit", "G4ThreeVector");
    (*store)["TrackID"] = G4AttDef("TrackID", "Track ID", "Physics", "", "G4int");
  }

  return store;
}

std::vector<G4AttValue> *MuonHit::CreateAttValues() const
{
  auto values = new std::vector<G4AttValue>;

  values->push_back(G4AttValue("HitType", "MuonHit", ""));
  values->push_back(G4AttValue("Layer", G4UIcommand::ConvertToString(flayerID), ""));
  values->push_back(G4AttValue("Energy", G4BestUnit(eDep, "Energy"), ""));
  values->push_back(G4AttValue("Pos", G4BestUnit(fPos, "Length"), ""));
  values->push_back(G4AttValue("TrackID", G4UIcommand::ConvertToString(fTrackID), ""));

  return values;
}