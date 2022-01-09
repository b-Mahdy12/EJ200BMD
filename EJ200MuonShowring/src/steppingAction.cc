#include "steppinAction.hh"

// using namespace std;
SteppingAction::SteppingAction(EventAction *event)
{
    fEvent = event;
}

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction(const G4Step *step)
{

    G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    const DetectorConstruction *det = static_cast<const DetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    G4LogicalVolume *fScoringVolume = det->GetScoringVolume();
    if (volume != fScoringVolume)
    {
        return;
    }

    G4double edep = step->GetTotalEnergyDeposit();
    // G4double edep = step->GetDeltaEnergy();
    G4double nSec = step->GetNumberOfSecondariesInCurrentStep();
    fEvent->AddEdep(edep);
    fEvent->AddnSec(nSec);
}
