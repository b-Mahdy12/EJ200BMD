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

    analysisManager->CreateH3("0", "Hit position (cm)", 200, -200 * cm, 200 * cm, 200, -10 * cm, 10 * cm, 200, -10 * cm, 10 * cm);
    // analysisManager->CreateH3("1", "Generated Muon position (cm)", 200, -10 * cm, 10 * cm, 29, -1.45 * cm, 1.45 * cm, 29, -1.45 * cm, 1.45 * cm);
    analysisManager->CreateH3("1", "Generated Muon position (cm)", 200, -200 * cm, 200 * cm, 200, -10 * cm, 10 * cm, 200, -10 * cm, 10 * cm);

    analysisManager->CreateNtuple("MuonHits", "Hits");
    analysisManager->CreateNtupleIColumn("evt");
    analysisManager->CreateNtupleDColumn("HitEnergy");
    analysisManager->CreateNtupleDColumn("fx");
    analysisManager->CreateNtupleDColumn("fy");
    analysisManager->CreateNtupleDColumn("fz");
    analysisManager->CreateNtupleIColumn("WLS_ID");
    analysisManager->CreateNtupleIColumn("Track_ID");
    analysisManager->FinishNtuple(0);

    analysisManager->CreateNtuple("MuonMap", "MuonsMap");
    analysisManager->CreateNtupleDColumn("fx");
    analysisManager->CreateNtupleDColumn("fy");
    analysisManager->CreateNtupleDColumn("fz");
    analysisManager->FinishNtuple(1);

    analysisManager->OpenFile("MuonHits");
}

void RunAction::EndOfRunAction(const G4Run *)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}
