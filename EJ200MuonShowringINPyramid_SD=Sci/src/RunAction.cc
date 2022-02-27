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

analysisManager->CreateH3("Energy_distribution","Energy loss distribution",  200, -61 * cm, 61 * cm, 200, -61 * cm, 61 * cm, 200, 0,2*MeV );
analysisManager->CreateH3("1","momentum_Z", 10,-200,200,10,-200,200,10,0,4500);
analysisManager->CreateH2("Energy_track","Energy loss per Track", 100, 0,50000, 200, 0,2 );
analysisManager->CreateH2("1","Momemtum vs Position [Z-direction]", 200, 0,200, 200, 0,200 );


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
