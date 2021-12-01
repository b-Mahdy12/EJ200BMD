#include "RunAction.hh"
#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <G4AccumulableManager.hh>
#include <G4SystemOfUnits.hh>
#include "g4root.hh"

RunAction::RunAction() : G4UserRunAction(),
    fNFEEvents("NFEEvents", 0),
    fNEvents("NEvents", 0),
    fNGammas("NGammas", 0),
    fAverageGammaEnergy("AvgGammaEnergy",0.)
{
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();

  accumulableManager->RegisterAccumulable(fNFEEvents);
  accumulableManager->RegisterAccumulable(fNEvents);

  accumulableManager->RegisterAccumulable(fNGammas);
  accumulableManager->RegisterAccumulable(fAverageGammaEnergy);
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstNtupleId(1);
  analysisManager->SetFirstHistoId(1);

  analysisManager->CreateH1("EnergyDeposition", "EJ200 Scintillator", 20, 0, 40000);

  analysisManager->CreateNtuple("eDep", "eDep");  
  analysisManager->CreateNtupleDColumn("eDep");  
//   analysisManager->CreateNtupleDColumn("eMis");  
  analysisManager->FinishNtuple();

  analysisManager->OpenFile("BMDResponse");  
}


void RunAction::BeginOfRunAction(const G4Run*)
{
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();
}

void RunAction::EndOfRunAction(const G4Run* run)
{
    G4int nofEvents = run->GetNumberOfEvent();

    if (nofEvents == 0) {
        return;
    }

    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Merge();

    if (IsMaster()) {
        G4cout
            << "\n--------------------End of Global Run-----------------------"
            << " \n The run was " << nofEvents << " events " << G4endl;
        
        if (fNGammas.GetValue()) {
            G4cout 
                << " * Produced " << fNGammas.GetValue()/((G4double)nofEvents) 
                << " secondary gammas/event. Average energy: "
	            << fAverageGammaEnergy.GetValue()/keV/fNGammas.GetValue() 
                << " keV" 
                << G4endl;
        }

        if (fNFEEvents.GetValue()) {
            G4cout
                << "Full Energy Events: " 
                << fNFEEvents.GetValue()
                << " Total Events in Detector: " 
                << fNEvents.GetValue()
                << G4endl;
        }
    } else {
          G4cout << " * No secondary gammas produced" << G4endl;
    }
}

RunAction::~RunAction()
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->Write();
}

void RunAction::AddSecondary(const G4ParticleDefinition* particle,
			     G4double energy)
{
    if (particle == G4Gamma::Definition()) {
        fNGammas += 1;
        fAverageGammaEnergy += energy;
    }

    return;
}

void RunAction::OnFullEnergyEvent()
{
    fNFEEvents += 1;

    return;
}

void RunAction::OnEvent()
{
    fNEvents += 1;

    return;
}
