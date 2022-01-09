#include "RunAction.hh"
#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <G4AccumulableManager.hh>
#include <G4SystemOfUnits.hh>
#include "g4root.hh"

RunAction::RunAction() : G4UserRunAction()
{
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run *)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->CreateNtuple("Hits", "Hits");
    analysisManager->CreateNtupleIColumn("evt");
    analysisManager->CreateNtupleDColumn("fx");
    analysisManager->CreateNtupleDColumn("fy");
    analysisManager->CreateNtupleDColumn("fz");
    analysisManager->FinishNtuple(0);

    analysisManager->CreateNtuple("MuonMap", "MuonsMap");
    analysisManager->CreateNtupleDColumn("fx");
    analysisManager->CreateNtupleDColumn("fy");
    analysisManager->CreateNtupleDColumn("fz");
    analysisManager->FinishNtuple(1);

    analysisManager->CreateNtuple("Edep", "Edep");
    analysisManager->CreateNtupleDColumn("EnergyDeposition");
    analysisManager->CreateNtupleDColumn("NofSecandries");
    analysisManager->FinishNtuple(2);

    // analysisManager->OpenFile("PyramidHits");
    analysisManager->OpenFile("EJ200Hits");
}

void RunAction::EndOfRunAction(const G4Run *run)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}
