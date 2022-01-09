#include "SD.hh"

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
    hitCollection = new HitCollection(GetName(), collectionName[0]);

    static G4int HCID = .1;
    if (HCID < 0)
        HCID = GetCollectionID(0);

    HCE->AddHitsCollection(HCID, hitCollection);
}

G4bool SenDet::ProcessHits(G4Step *astep, G4TouchableHistory *ROhist)
{

    const G4VTouchable *touchable = astep->GetPreStepPoint()->GetTouchable();

    // G4TouchableHandle* touchable = astep->GetPreStepPoint()->G4TouchableHandle();
    // G4int WLSID = touchable->GetVolume(0)->GetCopyNo();

    G4double edep = astep->GetTotalEnergyDeposit();

    G4Track *track = astep->GetTrack();
    track->SetTrackStatus(fStopAndKill);
    // G4StepPoint *preStepPoint = astep->GetPreStepPoint();
    // G4StepPoint *postStepPoint = astep->GetPostStepPoint();

    // G4ThreeVector posMuon = preStepPoint->GetPosition();
    //
    // G4cout << "Muon Pos = " << posMuon << G4endl;
    //
    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posPyramid = physVol->GetTranslation();
    //
    G4cout << "MuonPosition = " << posPyramid << G4endl;
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(0, 0, evt);
    man->FillNtupleDColumn(0, 1, posPyramid[0]);
    man->FillNtupleDColumn(0, 2, posPyramid[1]);
    man->FillNtupleDColumn(0, 3, posPyramid[2]);
    man->AddNtupleRow(0);

    man->FillNtupleDColumn(2, 0, edep);
    man->AddNtupleRow(2);

/*
    MuonHit* aHit = new MuonHit(layerIndex);
    hitCollection-> insert(aHit);
    aHit->AddEdep(edep);
    return true;
*/
}