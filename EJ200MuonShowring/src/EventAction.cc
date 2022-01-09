
#include "EventAction.hh"
#include "g4root.hh"
#include "Hits.hh"
#include "ROOTManager.hh"

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
    auto runManager = ROOTManager::Instance();

    runManager->ROOTTreeStruct.NMuonHit = 0;
    runManager->ROOTTreeStruct.Event = event->GetEventID();

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

        runManager->ROOTTreeStruct.MuonHitE[runManager->ROOTTreeStruct.NMuonHit] = (Float_t)hit->GetEdep();
        runManager->ROOTTreeStruct.MuonHitPosX[runManager->ROOTTreeStruct.NMuonHit] = (Float_t)pos.x();
        runManager->ROOTTreeStruct.MuonHitPosY[runManager->ROOTTreeStruct.NMuonHit] = (Float_t)pos.y();
        runManager->ROOTTreeStruct.MuonHitPosZ[runManager->ROOTTreeStruct.NMuonHit] = (Float_t)pos.z();
        runManager->ROOTTreeStruct.MuonHitBar[runManager->ROOTTreeStruct.NMuonHit] = (Int_t)hit->GetLayerNumber();

        hit->Print();
        runManager->ROOTTreeStruct.NMuonHit++;
    }

    runManager->Fill();
}