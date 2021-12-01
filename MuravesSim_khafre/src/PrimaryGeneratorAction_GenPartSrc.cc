//******************************************************************************
// PrimaryGeneratorAction_GenPartSrc.cc
//
//
//******************************************************************************
//

#include "PrimaryGeneratorAction_GenPartSrc.hh"

#include "G4Event.hh"
#include <G4Gamma.hh>
#include "G4GeneralParticleSource.hh"

//----------------------------------------------------------------------------//
PrimaryGeneratorAction_GenPartSrc::PrimaryGeneratorAction_GenPartSrc() : 
 virgin(true)
 {
  // define a particle gun
  particleGun = new G4GeneralParticleSource();

  // Create the table containing all particle names
  particleTable = G4ParticleTable::GetParticleTable();

  G4ParticleDefinition *myParticle = G4Gamma::Definition();
  myParticle = G4ParticleTable::GetParticleTable()->FindParticle("mu-");

  particleGun->SetParticleDefinition(myParticle);
  particleGun->GetCurrentSource()->GetPosDist()->SetPosDisType("Surface");
  particleGun->GetCurrentSource()->GetPosDist()->SetPosDisShape("Sphere");
  particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0, 0, 0));
  particleGun->GetCurrentSource()->GetPosDist()->SetRadius(70 * m);
  particleGun->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
  particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy(10 * GeV);
  particleGun->GetCurrentSource()->GetAngDist()->SetAngDistType("focused");
  particleGun->GetCurrentSource()->GetAngDist()->SetFocusPoint({0, 0, 0});

  // Create the messenger file
  //gunMessenger = new PrimaryGeneratorMessenger(this);
}

//----------------------------------------------------------------------------//
PrimaryGeneratorAction_GenPartSrc::~PrimaryGeneratorAction_GenPartSrc()
{
  delete particleGun;
}

//----------------------------------------------------------------------------//
void PrimaryGeneratorAction_GenPartSrc::GeneratePrimaries(G4Event *anEvent)
{
  if (virgin)
  {
    virgin = false;

    // THE PHYSICAL VOLUME DOESN'T EXIST UNTIL THIS IS CALLED FOR THE
    // FIRST TIME, HENCE WE ONLY DO THIS ONCE
    particleGun->GetCurrentSource()
        ->GetPosDist()
        ->ConfineSourceToVolume("world");
  }

  particleGun->GeneratePrimaryVertex(anEvent);
}
