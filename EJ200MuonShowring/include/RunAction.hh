#ifndef RUNACTION_HH
#define RUNACTION_HH

#include <G4UserRunAction.hh>
#include "globals.hh"
#include "g4root.hh"

class EventAction;

class G4Run;

class RunAction : public G4UserRunAction
{
public:
  RunAction(EventAction *event);
  ~RunAction();

  // Main interface
  void BeginOfRunAction(const G4Run *);
  void EndOfRunAction(const G4Run *);

private:
  EventAction *eventAction;
};

#endif
