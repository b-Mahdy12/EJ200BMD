// Michael Tytgat, 03/06/2020
//
/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "RunAction.hh"
#include "EventAction.hh"
#include "Analysis.hh"
#include "ROOTManager.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


RunAction::RunAction(EventAction* eventAction)
 : G4UserRunAction(), 
   fEventAction(eventAction)
{
  
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  
}

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  
}

