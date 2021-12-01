#include "StackingAction.hh"
#include "RunAction.hh"

#include <G4SystemOfUnits.hh>

StackingAction::StackingAction(RunAction* aRunAction) :
    G4UserStackingAction(),
    fRunAction(aRunAction)
{
}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack (const G4Track* aTrack)
{
  if (aTrack->GetParentID() > 0) {
    fRunAction->AddSecondary(aTrack->GetParticleDefinition(),
			                       aTrack->GetKineticEnergy());
  }

  return G4UserStackingAction::ClassifyNewTrack(aTrack);
}
