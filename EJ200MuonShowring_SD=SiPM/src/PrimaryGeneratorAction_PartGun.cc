//******************************************************************************
// PrimaryGeneratorAction_PartGun.cc
//
// Michael Tytgat
//******************************************************************************
//

#include "PrimaryGeneratorAction_PartGun.hh"

#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
//----------------------------------------------------------------------------//
PrimaryGeneratorAction_PartGun::PrimaryGeneratorAction_PartGun()
{
  // define a particle gun
  particleGun = new G4ParticleGun();

  G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
  particleGun->SetParticleDefinition(particleDefinition);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,1.,0.));
  particleGun->SetParticleEnergy(4*GeV);

  // Create the table containing all particle names
  //particleTable = G4ParticleTable::GetParticleTable();
}

//----------------------------------------------------------------------------//
PrimaryGeneratorAction_PartGun::~PrimaryGeneratorAction_PartGun()
{
  // if (particleGun) delete particleGun;
}

//----------------------------------------------------------------------------//
void PrimaryGeneratorAction_PartGun::GeneratePrimaries(G4Event* anEvent)
{ 
  particleGun->GeneratePrimaryVertex(anEvent);
}