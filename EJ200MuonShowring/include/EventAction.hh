/// \file EventAction.hh
/// \brief Definition of the EventAction class

#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include <G4UserEventAction.hh>
#include <globals.hh>

/// Event action class
/// Provides access to the G4Event* information at the begin and 
/// at the end of each event
class RunAction;

class EventAction : public G4UserEventAction
{
public:
    ///constructor
    EventAction(RunAction* );

    ///destructor
    ~EventAction();

    void BeginOfEventAction(const G4Event* anEvent) override;
    void EndOfEventAction(const G4Event* anEvent) override;
private:
    RunAction* fRunAction;
    G4int  fCollID_cryst { -1 };
};

#endif
