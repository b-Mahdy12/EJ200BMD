#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"
#include "globals.hh"

#include <vector>
#include <array>

class EventAction : public G4UserEventAction
{

public:
  EventAction();
  ~EventAction();

  virtual void BeginOfEventAction(const G4Event *);
  virtual void EndOfEventAction(const G4Event *);

private:
  G4int fWLSHCID;
};
#endif