// Michael Tytgat, 03/06/2020
//
/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "RunAction.hh"
#include "EventAction.hh"
#include <G4AccumulableManager.hh>
#include "Analysis.hh"
#include "ROOTManager.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction(EventAction *eventAction)
    : G4UserRunAction(),
      fEventAction(eventAction), fNEvents("NEvents", 0)
{
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fNEvents);


  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstNtupleId(1);
  analysisManager->SetFirstHistoId(1);

  analysisManager->CreateH1("eDep", "Title", 20, 0, 665);

  analysisManager->CreateNtuple("eDep", "eDep");
  analysisManager->CreateNtupleDColumn("eDep");
  analysisManager->CreateNtupleDColumn("eMis");
  analysisManager->FinishNtuple();

  ROOTManager::Instance()->Init();
  // analysisManager->OpenFile("Pyramid");
}

RunAction::~RunAction()
{
  // G4AnalysisManager* man = G4AnalysisManager::Instance();
  // man->Write();
  delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run * /*run*/)
{
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  /*
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file 
  // The default file name is set in RunAction::RunAction(),
  // it can be overwritten in a macro
  analysisManager->OpenFile();
  */
}

void RunAction::EndOfRunAction(const G4Run * /*run*/)
{
  // save histograms & ntuple
  //
  /*
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
  */
  ROOTManager::Instance()->Save();
}

void RunAction::OnEvent()
{
  fNEvents += 1;

  return;
}
