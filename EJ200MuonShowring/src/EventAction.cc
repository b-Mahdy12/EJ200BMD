
#include "EventAction.hh"
#include "g4root.hh"
#include "Hits.hh"
// #include "ROOTManager.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
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

EventAction::EventAction() : G4UserEventAction(), fWLSHCID(-1)
{

    G4RunManager::GetRunManager()->SetPrintProgress(1);
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event *)
{
    // Find hit collections by names (just once)
    if (fWLSHCID == -1)
    {
        auto SDManager = G4SDManager::GetSDMpointer();
        fWLSHCID = SDManager->GetCollectionID("WLSWire/HitCollection");
    }
    G4cout << "BeginOfEventAction fWLSHCID: " << fWLSHCID << G4endl;
}

void EventAction::EndOfEventAction(const G4Event *event)
{
    // G4AnalysisManager *man = G4AnalysisManager::Instance();

    G4int NMuonHit = 0;
    G4int evt = event->GetEventID();

    G4cout << ">>> Event " << event->GetEventID() << " >>> Simulation truth : " << G4endl;

    auto hc = GetHC(event, fWLSHCID);
    if (!hc)
        return;

    auto nhit = hc->GetSize();
    G4cout << "Nhit: " << nhit << G4endl;

    for (unsigned long i = 0; i < nhit; i++)
    {
        auto hit = static_cast<MuonHit *>(hc->GetHit(i));
        auto pos = hit->GetPos();

        G4double eng = hit->GetEdep();
        G4double X = pos.x();
        G4double Y = pos.y();
        G4double Z = pos.z();
        G4int WLS_ID = hit->GetLayerNumber();

        G4AnalysisManager *man = G4AnalysisManager::Instance();
        man->FillNtupleIColumn(0, 0, evt);
        man->FillNtupleDColumn(0, 1, eng);
        // man->FillNtupleDColumn(0,2,??);
        man->FillNtupleDColumn(0, 2, X);
        man->FillNtupleDColumn(0, 3, Y);
        man->FillNtupleDColumn(0, 4, Z);
        man->FillNtupleIColumn(0, 5, WLS_ID);
        man->AddNtupleRow(0);

        hit->Print();
        NMuonHit++;
    }
    G4cout << ">>> NoOfMuonHits " << NMuonHit << G4endl;
}