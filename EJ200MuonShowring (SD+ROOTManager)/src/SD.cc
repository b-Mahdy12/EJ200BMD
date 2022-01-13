#include "SD.hh"

#include <vector>
#include <sstream>

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

SenDet ::SenDet(G4String name) : G4VSensitiveDetector(name)
{

    G4cout << "Creating SD with name " << name << G4endl;
    collectionName.insert("HitCollection");
}
SenDet::~SenDet()
{
}

void SenDet::Initialize(G4HCofThisEvent *HCE)
{
    hitCollection = new WLSHitCollection(GetName(), collectionName[0]);

    static G4int HCID = .1;
    if (HCID < 0)
        HCID = GetCollectionID(0);

    HCE->AddHitsCollection(HCID, hitCollection);
}

G4bool SenDet::ProcessHits(G4Step *astep, G4TouchableHistory *ROhist)
{

    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    auto prestep = astep->GetPreStepPoint();
    auto touchable = astep->GetPreStepPoint()->GetTouchable();
    auto track = astep->GetTrack();

    G4cout << "Hit Phys.Vol: " << prestep->GetPhysicalVolume()->GetName() << G4endl;

    G4ThreeVector pos = prestep->GetPosition();
    G4double edep = astep->GetTotalEnergyDeposit();

    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posMuon = physVol->GetTranslation();
    G4cout << "MuonPosition = " << posMuon << G4endl;

    G4int LayerID = touchable->GetVolume()->GetCopyNo();

    auto hit = new MuonHit();
    hit->SetEdep(edep);
    hit->SetPos(pos);
    hit->SetTrackID(track->GetTrackID());

    G4cout << "LayerID: " << LayerID << G4endl;
    hit->Print();

    hitCollection->insert(hit);

/*
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(0, 0, evt);
    man->FillNtupleDColumn(0, 1, posPyramid[0]);
    man->FillNtupleDColumn(0, 2, posPyramid[1]);
    man->FillNtupleDColumn(0, 3, posPyramid[2]);
    man->AddNtupleRow(0);

    man->FillNtupleDColumn(2, 0, edep);
    man->AddNtupleRow(2);
*/
    return true;
}