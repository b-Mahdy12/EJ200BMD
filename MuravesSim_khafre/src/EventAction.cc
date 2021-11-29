//
// Michael Tytgat
//

#include "EventAction.hh"
#include "Analysis.hh"
#include "Hits.hh"
#include "ROOTManager.hh"
#include "TrackingAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include <G4THitsMap.hh>
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

using std::array;
using std::vector;

namespace
{

  // Utility function which finds a hit collection with the given Id
  // and print warnings if not found
  G4VHitsCollection *GetHC(const G4Event *event, G4int collId)
  {
    auto hce = event->GetHCofThisEvent();
    if (!hce)
    {
      G4ExceptionDescription msg;
      msg << "No hits collection of this event found." << G4endl;
      G4Exception("EventAction::EndOfEventAction()",
                  "Code001", JustWarning, msg);
      return nullptr;
    }

    auto hc = hce->GetHC(collId);
    if (!hc)
    {
      G4ExceptionDescription msg;
      msg << "Hits collection " << collId << " of this event not found." << G4endl;
      G4Exception("EventAction::EndOfEventAction()",
                  "Code001", JustWarning, msg);
    }
    return hc;
  }

}

EventAction::EventAction()
    : G4UserEventAction(),
      fScintbarsHCID(-1)
{
  // set printing per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event *)
{

  // Find hit collections by names (just once)
  // if ( fScintbarsHCID == -1 ) {
  auto SDManager = G4SDManager::GetSDMpointer();
  // py from DetectorConstruction.cc "/Py" and PyramidColl from SensetiveDetector.cc
  fScintbarsHCID = SDManager->GetCollectionID("Py/EnergyDeposit");
  // }
  //G4cout << "BeginOfEventAction fScintbarsHCID: " << fScintbarsHCID << G4endl;

  TrackingAction::Instance()->ResetParents();
}

void EventAction::EndOfEventAction(const G4Event *event)
{
  // Get analysis manager
  G4AnalysisManager *analysis = G4AnalysisManager::Instance();

  G4HCofThisEvent *HCE = event->GetHCofThisEvent();

  if (!HCE)
  {
    return;
  }
  //Retrieve the hits-collection.
  //This comes from a Geant4 multiscorer of type "G4PSEnergyDeposit", which scores
  //energy deposit.
  G4THitsMap<G4double> *evtMap = dynamic_cast<G4THitsMap<G4double> *>(HCE->GetHC(fScintbarsHCID));

  //Store the total energy in a variable
  G4double totEdep = 0.;
  //Sum all individual energy deposits in this event
  for (auto pair : *(evtMap->GetMap()))
  {
    G4double edep = *(pair.second);
    //Sum the energy deposited in all crystals, irrespectively of threshold.
    totEdep += edep;
  }

  if (totEdep > 0)
  {
    fRunAction->OnEvent();

    G4double energy = totEdep / keV;
    G4double missing_energy = 662 - totEdep / keV;

    analysis->FillH1(1, energy);

    analysis->FillNtupleDColumn(1, 0, energy);
    analysis->FillNtupleDColumn(1, 1, missing_energy);
    analysis->AddNtupleRow(1);

    G4cout << "The total energy deposited in this event is: " << totEdep / keV << " keV " << G4endl;
  }
}
