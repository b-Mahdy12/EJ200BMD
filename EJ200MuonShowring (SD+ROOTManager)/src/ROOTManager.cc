#include "ROOTManager.hh"

#include <TROOT.h>
#include <TFile.h>
#include "TBranch.h"
#include "TTree.h"
#include <CLHEP/Units/SystemOfUnits.h>
#include "G4UnitsTable.hh"

ROOTManager *ROOTManager::fgInstance = 0;

ROOTManager *ROOTManager::Instance()
{
  return fgInstance;
}

ROOTManager::ROOTManager()
{
  fgInstance = this;
}

ROOTManager::~ROOTManager()
{
  if (ROOTFile)
    delete ROOTFile;
  fgInstance = 0;
}

void ROOTManager::Init()
{
  // Creating a tree container to handle histograms and ntuples.
  // This tree is associated to an output file.
  //
  // create ROOT file (dove alla fine salvare dati e grafici)

  ROOTFile = new TFile("MuonHits.root", "RECREATE");
  if (!ROOTFile)
  {
    G4cout << " problem creating the ROOT TFile" << G4endl;
    return;
  }

  // creazione ttree p_incident_TOT
  ROOTTree = new TTree("MuonMap", "MuonHits");

  ROOTTree->Branch("Event", &ROOTTreeStruct.Event, "Event/I");

  ROOTTree->Branch("NMuonHit", &ROOTTreeStruct.NMuonHit, "NMuonHit/I");
  ROOTTree->Branch("MuonHitE", &ROOTTreeStruct.MuonHitE, "MuonHitE[NMuonHit]/F");
  ROOTTree->Branch("MuonHitPosX", &ROOTTreeStruct.MuonHitPosX, "MuonHitPosX[NMuonHit]/F");
  ROOTTree->Branch("MuonHitPosY", &ROOTTreeStruct.MuonHitPosY, "MuonHitPosY[NMuonHit]/F");
  ROOTTree->Branch("MuonHitPosZ", &ROOTTreeStruct.MuonHitPosZ, "MuonHitPosZ[NMuonHit]/F");
  ROOTTree->Branch("MuonHitBar", &ROOTTreeStruct.MuonHitBar, "MuonHitLayer[NMuonHit]/I");
}

void ROOTManager::Save()
{
  if (ROOTFile)
  {
    ROOTFile->Write();
    ROOTFile->Close();
    G4cout << "ROOT Tree closed" << G4endl;
  }
}

void ROOTManager::Fill()
{
  ROOTTree->Fill();
}
