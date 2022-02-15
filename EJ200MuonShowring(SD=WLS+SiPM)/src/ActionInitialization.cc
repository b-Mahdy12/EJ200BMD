
//
/// \file ActionInitialization.cc
/// \brief Implementation of the ActionInitialization class

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction_CRY.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
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
   EventAction *eventAction = new EventAction;
  SetUserAction(new RunAction(eventAction));
}

void ActionInitialization::Build() const
{
  auto generator = MuravesMessenger::Instance()->GetPrimaryGenerator();
  if (generator == "CRY")
    SetUserAction(new PrimaryGeneratorAction_CRY(""));
  else if (generator == "GPS")
    SetUserAction(new PrimaryGeneratorAction());

  EventAction *eventAction = new EventAction;
  SetUserAction(eventAction);

  SetUserAction(new RunAction(eventAction));

  // SteppingAction *steppingAction = new SteppingAction(eventAction);
  // SetUserAction(steppingAction);
}
