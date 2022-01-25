
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
    
}

void EventAction::EndOfEventAction(const G4Event *event)
{
   
}