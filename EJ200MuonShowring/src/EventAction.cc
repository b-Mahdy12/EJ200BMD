/// \file EventAction.cc
/// \brief Implementation of the EventAction class
#include "EventAction.hh"
#include "RunAction.hh"

#include <G4SDManager.hh>
#include <G4THitsMap.hh>
#include <G4SystemOfUnits.hh>
#include <G4Event.hh>
#include "g4root.hh"

EventAction::EventAction(RunAction *aRunAction) : G4UserEventAction(), fRunAction(aRunAction)
{
    ;
}

EventAction::~EventAction()
{
    ;
}

void EventAction::BeginOfEventAction(const G4Event * /*anEvent*/)
{
    // Implement if necessary
}

void EventAction::EndOfEventAction(const G4Event *event)
{
    G4SDManager *sdm = G4SDManager::GetSDMpointer();
    G4AnalysisManager *analysis = G4AnalysisManager::Instance();

    // Retrieve the collectionID corresponding to hits in the NaI
    // The variable fCollID_cryst should be initialized to -1 in EventAction.hh) so  that
    // this block of code is executed only at the end of the first event.
    if (fCollID_cryst < 0)
    {
        fCollID_cryst = sdm->GetCollectionID("detector/EnergyDeposit");
    }

    //Hits collections
    // Get all hits-collections available for this events:
    G4HCofThisEvent *HCE = event->GetHCofThisEvent();

    if (!HCE)
    {
        return;
    }

    //ok, let's start the game: retrieve the hits-collection in the NaI.
    //This comes from a Geant4 multiscorer of type "G4PSEnergyDeposit", which scores
    //energy deposit.
    G4THitsMap<G4double> *evtMap = dynamic_cast<G4THitsMap<G4double> *>(HCE->GetHC(fCollID_cryst));

    //Store the total energy in a variable
    G4double totEdep = 0.;

    //Sum all individual energy deposits in this event
    for (auto pair : *(evtMap->GetMap()))
    {
        G4double edep = *(pair.second);
        //Sum the energy deposited in all crystals, irrespectively of threshold.
        totEdep += edep;
    }

    // if (totEdep > (661.5 * keV))
    // {
    //     fRunAction->OnFullEnergyEvent();
    // }

    if (totEdep > 0)
    {
        fRunAction->OnEvent();

        G4double energy = totEdep / keV;
        // G4double missing_energy = 662 - totEdep / keV;

        analysis->FillH1(1, energy);

        analysis->FillNtupleDColumn(1, 0, energy);
        // analysis->FillNtupleDColumn(1, 1, missing_energy);
        analysis->AddNtupleRow(1);

        G4cout << "The total energy deposited in this event is: " << totEdep / keV << " keV " << G4endl;
    }
}
