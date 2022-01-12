#include "RunAction.hh"
#include "EventAction.hh"
#include "ROOTManager.hh"
#include "g4root.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction(EventAction *event) : G4UserRunAction(), eventAction(event)
{
    ROOTManager::Instance()->Init();
}

RunAction::~RunAction()
{
    delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run*)
{
}

void RunAction::EndOfRunAction(const G4Run*)
{
    ROOTManager::Instance()->Save();
}
