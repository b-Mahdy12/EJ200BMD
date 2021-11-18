//
// 
//

#include "SensitiveDetector.hh"

#include <vector>
#include <sstream>

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

pyrSD::pyrSD( const G4String &SDname ): G4VSensitiveDetector(SDname),
    fHitsCollection(0), fHCID(-1)
{
  // to collect data SDManager
  collectionName.insert("PyramidColl");
}

pyrSD::~pyrSD() {}

void pyrSD::Initialize( G4HCofThisEvent *hitcollection )
{
  fHitsCollection = new ScintbarHitsCollection(SensitiveDetectorName, collectionName[0]);
  if ( fHCID < 0 ) {
    fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
  }
  hitcollection->AddHitsCollection( fHCID, fHitsCollection);
  //G4cout << "Initialize pyrSD hitcoll ID: " << fHCID << G4endl; 
}

G4bool pyrSD::ProcessHits( G4Step *step, G4TouchableHistory* history )
{

  // geometry info from PreStepPoint
  auto prestep = step->GetPreStepPoint();
  auto touchable = step->GetPreStepPoint()->GetTouchable();
  auto track = step->GetTrack();
  
  G4cout << "Hit Phys.Vol: " << prestep->GetPhysicalVolume()->GetName() << G4endl;
  G4cout << "particle: " << track->GetDefinition()->GetPDGEncoding() << G4endl;
  G4cout << "parent: " << track->GetParentID() << G4endl;
  
  G4ThreeVector pos = prestep->GetPosition();
  
  G4double edep = step->GetTotalEnergyDeposit();
  if ( edep == 0. ) return true;


  return true;
}

void pyrSD::EndOfEvent( G4HCofThisEvent *hitCollection ) {}

