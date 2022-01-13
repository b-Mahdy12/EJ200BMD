#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(EventAction *event);
    ~SteppingAction();

    virtual void UserSteppingAction(const G4Step *);

private:
    EventAction *fEvent;
    // virtual G4bool ProcessHits(G4Step *);
};
#endif