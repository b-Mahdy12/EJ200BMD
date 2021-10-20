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
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

using std::array;
using std::vector;

EventAction::EventAction()
    : G4UserEventAction()
{
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
