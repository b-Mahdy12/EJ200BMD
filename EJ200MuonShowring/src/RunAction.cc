#include "RunAction.hh"
#include "EventAction.hh"
#include "ROOTManager.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction(EventAction *event) : G4UserRunAction(), eventAction(event)
{
    ROOTManager::Instance()->Init();
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run *)
{
}

void RunAction::EndOfRunAction(const G4Run *run)
{
    ROOTManager::Instance()->Save();
}
