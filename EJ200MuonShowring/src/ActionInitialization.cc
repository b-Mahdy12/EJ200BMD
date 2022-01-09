
//
/// \file ActionInitialization.cc
/// \brief Implementation of the ActionInitialization class

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction_CRY.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "steppinAction.hh"

#include "MuravesMessenger.hh"

ActionInitialization::ActionInitialization()
    : G4VUserActionInitialization()
{
}

ActionInitialization::~ActionInitialization()
{
}

void ActionInitialization::BuildForMaster() const
{
}

void ActionInitialization::Build() const
{
  auto generator = MuravesMessenger::Instance()->GetPrimaryGenerator();
  if (generator == "CRY")
    SetUserAction(new PrimaryGeneratorAction_CRY(""));
  else if (generator == "GPS")
    SetUserAction(new PrimaryGeneratorAction());

  RunAction *theRunAction = new RunAction();
  SetUserAction(theRunAction);

  EventAction *eventAction = new EventAction(theRunAction);
  SetUserAction(eventAction);

  SteppingAction *steppingAction = new SteppingAction(eventAction);
  SetUserAction(steppingAction);
}
