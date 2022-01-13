#include "RunAction.hh"
#include "EventAction.hh"
// #include "ROOTManager.hh"
#include "g4root.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction(EventAction *event) : G4UserRunAction(), eventAction(event)
{
    // ROOTManager::Instance()->Init();
}

RunAction::~RunAction()
{
    // delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run *)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->CreateNtuple("MuonHits", "Hits");
    analysisManager->CreateNtupleIColumn("evt");
    analysisManager->CreateNtupleDColumn("HitEnergy");
    // analysisManager->CreateNtupleDColumn("NoOfHits");
    analysisManager->CreateNtupleDColumn("fx");
    analysisManager->CreateNtupleDColumn("fy");
    analysisManager->CreateNtupleDColumn("fz");
    analysisManager->CreateNtupleIColumn("WLS_ID");
    analysisManager->FinishNtuple(0);

    analysisManager->CreateNtuple("MuonMap", "MuonsMap");
    analysisManager->CreateNtupleDColumn("fx");
    analysisManager->CreateNtupleDColumn("fy");
    analysisManager->CreateNtupleDColumn("fz");
    analysisManager->FinishNtuple(1);

    analysisManager->CreateNtuple("MuonPosition", "MuonHit");
    analysisManager->CreateNtupleDColumn("edep");
    analysisManager->CreateNtupleDColumn("fx");
    analysisManager->CreateNtupleDColumn("fy");
    analysisManager->CreateNtupleDColumn("fz");
    analysisManager->FinishNtuple(2);

    analysisManager->OpenFile("MuonHits");
}

void RunAction::EndOfRunAction(const G4Run *)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}
