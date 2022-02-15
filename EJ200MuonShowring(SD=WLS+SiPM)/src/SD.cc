#include "SD.hh"

#include <fstream>
#include <vector>
#include <sstream>
#include "g4root.hh"

#include "G4SystemOfUnits.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4Track.hh"
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
    /*
    hitCollection = new SiPMHitCollection(GetName(), collectionName[0]);

    static G4int HCID = -1;
    if (HCID < 0)
        HCID = GetCollectionID(0);

    HCE->AddHitsCollection(HCID, hitCollection);
    */
}

G4bool SenDet::ProcessHits(G4Step *astep, G4TouchableHistory *ROhist)
{

    auto prestep = astep->GetPreStepPoint();
    auto touchable = astep->GetPreStepPoint()->GetTouchable();
    auto track = astep->GetTrack();

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    /*
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        man->FillNtupleIColumn(0, 0, evt);

       // G4cout << "Hit Phys.Vol: " << prestep->GetPhysicalVolume()->GetName() << G4endl;
   
    G4ThreeVector pos = prestep->GetPosition();
    G4double X_m = pos.x();
    G4double Y_m = pos.y();
    G4double Z_m = pos.z();
    man->FillNtupleDColumn(0, 2, X_m);
    man->FillNtupleDColumn(0, 3, Y_m);
    man->FillNtupleDColumn(0, 4, Z_m);
    // man->FillNtupleIColumn(0, 5, WLSID);

    // G4double edep = astep->GetTotalEnergyDeposit();

    G4int TrackID = track->GetTrackID();
    man->FillNtupleIColumn(0, 6, TrackID);

    G4double TrackEnergy = track->GetTotalEnergy();
    man->FillNtupleDColumn(0, 1, TrackEnergy);

    man->AddNtupleRow(0);


*/
    if (astep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary)
    {
        G4double preTime = astep->GetPreStepPoint()->GetGlobalTime();
        track->SetTrackStatus(fKillTrackAndSecondaries);

        G4double time = astep->GetPreStepPoint()->GetGlobalTime();

        if (track->GetVolume()->GetName() == "WLSupWire")
        {
            WLS_ID.push_back(touchable->GetVolume()->GetCopyNo());

            if (track->GetVolume()->GetName() == "SiPM_top")
            {
                // SiPM_ID.push_back(touchable->GetVolume()->GetInstanceID());
                SiPM_top_timeData.push_back(astep->GetPreStepPoint()->GetGlobalTime());
                SiPM_top_energyData.push_back(astep->GetPreStepPoint()->GetTotalEnergy());
            }

            if (track->GetVolume()->GetName() == "SiPM_bottom")
            {
                // SiPM_ID.push_back(touchable->GetVolume()->GetInstanceID());
                SiPM_bottom_timeData.push_back(astep->GetPreStepPoint()->GetGlobalTime());
                SiPM_bottom_energyData.push_back(astep->GetPreStepPoint()->GetTotalEnergy());
            }
        }
        if (track->GetVolume()->GetName() == "WLSdownWire")
        {
            WLS_ID.push_back(touchable->GetVolume()->GetCopyNo());

            if (track->GetVolume()->GetName() == "SiPM_top")
            {
                // SiPM_ID.push_back(touchable->GetVolume()->GetInstanceID());
                SiPM_top_timeData.push_back(astep->GetPreStepPoint()->GetGlobalTime());
                SiPM_top_energyData.push_back(astep->GetPreStepPoint()->GetTotalEnergy());
            }

            if (track->GetVolume()->GetName() == "SiPM_bottom")
            {
                // SiPM_ID.push_back(touchable->GetVolume()->GetCopyNo());
                SiPM_bottom_timeData.push_back(astep->GetPreStepPoint()->GetGlobalTime());
                SiPM_bottom_energyData.push_back(astep->GetPreStepPoint()->GetTotalEnergy());
            }
        }
    }
    if (astep->GetTotalEnergyDeposit() <= 0.)

        return true;
}

void SenDet::EndOfEvent(G4HCofThisEvent *HCE)
{
    G4String fileName;

    if (GetName() == "SiPM_top")
    {
        fileName = "hits/SiPM_top_hits.dat";
    }

    if (GetName() == "SiPM_bottom")
    {
        fileName = "hits/SiPM_bottom_hits.dat";
    }
    std::ofstream fileOut(fileName);

    if (GetName() == "SiPM_top")
    {
        for (std::vector<G4double>::const_iterator i = SiPM_top_timeData.begin(); i != SiPM_top_timeData.end(); i++)
        {
            std::vector<G4double>::const_iterator j = SiPM_top_energyData.begin();
            std::vector<G4double>::const_iterator n = WLS_ID.begin();
            // fileOut << *i / ns << '\t' << *j / eV << '\n';

            fileOut << *n << '\t' << *i / ns << '\t' << *j / eV << '\n';
        }
    }

    if (GetName() == "SiPM_bottom")
    {
        for (std::vector<G4double>::const_iterator i = SiPM_bottom_timeData.begin(); i != SiPM_bottom_timeData.end(); i++)
        {
            std::vector<G4double>::const_iterator j = SiPM_bottom_energyData.begin();
            std::vector<G4double>::const_iterator n = WLS_ID.begin();

            fileOut << *n << '\t' << *i / ns << '\t' << *j / eV << '\n';
        }
    }
}