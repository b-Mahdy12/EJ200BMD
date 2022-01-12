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
    // G4cout << "MuonPosition = " << posMuon << G4endl;

    G4double X_m = posMuon.x();
    G4double Y_m = posMuon.y();
    G4double Z_m = posMuon.z();

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(2, 0, edep);
    man->FillNtupleDColumn(2, 1, X_m);
    man->FillNtupleDColumn(2, 2, Y_m);
    man->FillNtupleDColumn(2, 3, Z_m);
    man->AddNtupleRow(2);

    G4int WLSID = touchable->GetVolume()->GetInstanceID();
    G4cout << "WLSID: " << WLSID << G4endl;

    auto hit = new MuonHit();
    hit->SetEdep(edep);
    hit->SetPos(pos);
    hit->SetTrackID(track->GetTrackID());
    hit->Print();

    hitCollection->insert(hit);

    return true;
}