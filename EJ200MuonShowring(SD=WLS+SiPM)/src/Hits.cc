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
}

std::vector<G4AttValue> *MuonHit::CreateAttValues() const
{
}