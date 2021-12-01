/// \file PhysicsList.hh
/// \brief Implementation of the PhysicsList class (Mandatory)

#include "PhysicsList.hh"

#include <G4EmStandardPhysics.hh>
#include <G4DecayPhysics.hh>
#include <G4ProductionCutsTable.hh>
#include <G4SystemOfUnits.hh>

#include <G4EmLivermorePhysics.hh>
#include <G4EmExtraPhysics.hh>
#include <G4HadronElasticPhysics.hh>
#include <G4HadronPhysicsFTFP_BERT.hh>

PhysicsList::PhysicsList() : 
  G4VModularPhysicsList()
{
  // Standard EM physics
  RegisterPhysics(new G4EmStandardPhysics());
  RegisterPhysics(new G4DecayPhysics());
  RegisterPhysics(new G4EmExtraPhysics());
  RegisterPhysics(new G4HadronElasticPhysics());
  RegisterPhysics(new G4HadronPhysicsFTFP_BERT());
}

PhysicsList::~PhysicsList()
{;}

void PhysicsList::ConstructParticle()
{  
  // Call parent method. Replace it, if required
  G4VModularPhysicsList::ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
  // Call parent method. Replace it, if required
  G4VModularPhysicsList::ConstructProcess();
}

void PhysicsList::SetCuts()
{
  // The method SetCuts() is mandatory in the interface. Here, one just use 
  // the default SetCuts() provided by the base class.
  // Call parent method. Replace it, if required
   G4VUserPhysicsList::SetCuts();

}
