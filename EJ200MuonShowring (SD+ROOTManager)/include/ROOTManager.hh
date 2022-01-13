#ifndef ROOTMANAGER_HH
#define ROOTMANAGER_HH

#include "globals.hh"
#include "EventAction.hh"
#include "RunAction.hh"

#include <TFile.h>
#include <TTree.h>

class ROOTManager
{

  static const int MaxNMuonHit = 5000;
  
  struct ROOTTreeStruct_t {
    Int_t Event;
    
    Int_t NMuonHit;
    Float_t MuonHitE[MaxNMuonHit];
    Float_t MuonHitPosX[MaxNMuonHit];
    Float_t MuonHitPosY[MaxNMuonHit];
    Float_t MuonHitPosZ[MaxNMuonHit];
    Int_t MuonHitBar[MaxNMuonHit];

  };

public:
  
  ROOTManager();
  ~ROOTManager();
  
  static ROOTManager* Instance();   
    
  void Init();		
  void Save();
  void Fill();

  struct ROOTTreeStruct_t ROOTTreeStruct;
  
private:

  static ROOTManager* fgInstance;    
  TFile* ROOTFile;        
  TTree* ROOTTree;
};

#endif

