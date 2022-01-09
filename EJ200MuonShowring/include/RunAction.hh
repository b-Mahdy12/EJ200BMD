#ifndef RUNACTION_HH
#define RUNACTION_HH

#include <G4UserRunAction.hh>
#include <G4Run.hh>
#include "g4root.hh"

class RunAction : public G4UserRunAction
{
public:
  RunAction();
  ~RunAction();

  // Main interface
  void BeginOfRunAction(const G4Run *);
  void EndOfRunAction(const G4Run *);
};

#endif
