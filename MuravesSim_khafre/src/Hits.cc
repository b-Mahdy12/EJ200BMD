//
// Michael Tytgat
//

#include "Hits.hh"
#include "DetectorConstruction.hh"

#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

G4ThreadLocal G4Allocator<ScintbarHit>* ScintbarHitAllocator;

ScintbarHit::ScintbarHit()
  : G4VHit()
{}


ScintbarHit::ScintbarHit( G4int iStationID, G4int iModuleID, G4int iBarID )
  : G4VHit()
{}




ScintbarHit::~ScintbarHit()
{}

ScintbarHit::ScintbarHit( const ScintbarHit &right )
  : G4VHit()
{}

const ScintbarHit& ScintbarHit::operator=( const ScintbarHit &right )
{

}


G4bool ScintbarHit::operator==( const ScintbarHit &right ) const
{
}

void ScintbarHit::Draw()
{}

void ScintbarHit::Print()
{
 
}

const std::map<G4String,G4AttDef>* ScintbarHit::GetAttDefs() const
{
  
}

std::vector<G4AttValue>* ScintbarHit::CreateAttValues() const
{
  

}




