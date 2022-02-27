#include "SD.hh"

#include <vector>
#include <sstream>
#include "g4root.hh"

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
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    auto prestep = astep->GetPreStepPoint();
    auto touchable = astep->GetPreStepPoint()->GetTouchable();
    auto track = astep->GetTrack();

    // G4cout << "particle: " << track->GetDefinition()->GetParticleName() << G4endl;
    // G4cout << "parent: " << track->GetParentID() << G4endl;

    auto nSec = astep->GetSecondary();

    G4ThreeVector pos = prestep->GetPosition();
    // G4double edep = astep->GetTotalEnergyDeposit();

    // G4int WLSCopyNo = touchable->GetVolume()->GetCopyNo();
    // G4int WLSID = (WLSCopyNo & 0xFF);

    G4VPhysicalVolume *physVol = touchable->GetVolume();
     G4ThreeVector posMuon = physVol->GetTranslation();
    G4double X_m = posMuon.x();
    G4double Y_m = posMuon.y();
    G4double Z_m = posMuon.z();

    G4int TrackID = track->GetTrackID();
    G4double TrackEnergy = track->GetTotalEnergy();

    auto hit = new MuonHit();
    hit->SetEdep(TrackEnergy);
    hit->SetPos(pos);
    hit->SetTrackID(TrackID);
    hit->Print();
    hitCollection->insert(hit);
man->FillH3(0,X_m,Y_m,TrackEnergy);
    man->FillNtupleIColumn(0, 0, evt);
    man->FillNtupleDColumn(0, 1, TrackEnergy);
    man->FillNtupleDColumn(0, 2, X_m);
    man->FillNtupleDColumn(0, 3, Y_m);
    man->FillNtupleDColumn(0, 4, Z_m);
    // man->FillNtupleIColumn(0, 5, WLSID);
    man->FillNtupleIColumn(0, 6, TrackID);
    man->AddNtupleRow(0);

    G4int currentCopyNo = -999;
    G4int motherCopyNo = -999;
    G4TouchableHistory *theTouchable = (G4TouchableHistory *)(track->GetTouchable());
    if (theTouchable)
    {
        currentCopyNo = theTouchable->GetReplicaNumber(0);
        if (theTouchable->GetHistoryDepth() > 0)
        {
            motherCopyNo = theTouchable->GetReplicaNumber(1);
            G4cout << "|_________________________________________________________________| \n"
                   << "WLS_SD createNewHit for"
                   << "\n"
                   << " PV " << prestep->GetPhysicalVolume()->GetName() << "\n"
                   << " PVid = " << currentCopyNo << "\n"
                   << " MVid = " << motherCopyNo
                   //    << " Unit " << UnitID
                   << G4endl;
            G4cout
                << " primary " << track->GetDefinition()->GetPDGEncoding() << "\n"
                << " at time " << track->GetGlobalTime() << "\n"
                << " For Track  " << track->GetTrackID() << "\n"
                << " which is a " << track->GetDefinition()->GetParticleName() << "\n"
                << " of energy " << track->GetTotalEnergy() << "\n"
                << " daughter of part. " << track->GetParentID() << "\n";

            G4cout << " and created by ";
            if (track->GetCreatorProcess() != NULL)
                G4cout << track->GetCreatorProcess()->GetProcessName();
            else
                G4cout << "NO process";
            G4cout << "\n|_________________________________________________________________|" << G4endl;
        }
    }
    return true;
}
