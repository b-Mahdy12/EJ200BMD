#include "EventAction.hh"
#include "G4SystemOfUnits.hh"

EventAction::EventAction(RunAction *)
{
    fEdep = 0;
    fNsec = 0;
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event *)
{
    fEdep = 0;
    fNsec = 0;
    // posMuon = 0;
}

void EventAction::EndOfEventAction(const G4Event *)
{
    G4cout << "Energy deposition = " << fEdep / MeV << "MeV" << G4endl;
    G4cout << "Number of Secondries / step =" << fNsec << G4endl;

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(2, 0, fEdep);
    man->FillNtupleDColumn(2, 1, fNsec);
    man->AddNtupleRow(2);

    // G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    // G4cout << "Muon Pos = " << posMuon << G4endl;

    // man->FillNtupleIColumn(0, 0, evt);
    // man->FillNtupleDColumn(0, 1, posMuon[0]);
    // man->FillNtupleDColumn(0, 2, posMuon[1]);
    // man->FillNtupleDColumn(0, 3, posMuon[2]);
    // man->AddNtupleRow(0);
}
