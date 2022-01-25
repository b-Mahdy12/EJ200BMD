/// \file PhysicsList.hh
/// \brief Implementation of the PhysicsList class (Mandatory)

#include "PhysicsList.hh"

#include <G4EmStandardPhysics.hh>
#include <G4DecayPhysics.hh>
#include <G4ProductionCutsTable.hh>
#include <G4SystemOfUnits.hh>

#include "G4OpticalPhysics.hh"


#include <G4EmLivermorePhysics.hh>
#include <G4EmExtraPhysics.hh>
#include <G4HadronElasticPhysics.hh>
#include <G4HadronPhysicsFTFP_BERT.hh>

#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"

PhysicsList::PhysicsList() : 
  G4VModularPhysicsList()
{
  // Standard EM physics
  RegisterPhysics(new G4EmStandardPhysics());
  RegisterPhysics(new G4DecayPhysics());
  RegisterPhysics(new G4EmExtraPhysics());
  RegisterPhysics(new G4HadronElasticPhysics());
  RegisterPhysics(new G4HadronPhysicsFTFP_BERT());

    // * Optical Physics
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  RegisterPhysics( opticalPhysics );
   // adjust some parameters for the optical physics
  opticalPhysics->SetWLSTimeProfile("delta");

  opticalPhysics->SetScintillationYieldFactor(1.0);
  opticalPhysics->SetScintillationExcitationRatio(0.0);

  opticalPhysics->SetMaxNumPhotonsPerStep(100);
  opticalPhysics->SetMaxBetaChangePerStep(10.0);

  opticalPhysics->SetTrackSecondariesFirst(kScintillation,true);

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



//----------------------------------------------------------------------------//
// Define electromagnetic transportation processes
//----------------------------------------------------------------------------//
// gamma
#include "G4PhotoElectricEffect.hh" 
#include "G4ComptonScattering.hh"  
#include "G4PolarizedCompton.hh"
#include "G4GammaConversion.hh" 
// e-
// e+
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
// muon
#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh" 
#include "G4MuBremsstrahlung.hh" 
#include "G4MuPairProduction.hh" 
// neutron
#include "G4HadronElasticProcess.hh"
//#include "G4LElastic.hh"
#include "G4HadronElastic.hh"
#include "G4NeutronInelasticProcess.hh"
//#include "G4LENeutronInelastic.hh"
#include "G4HadronCaptureProcess.hh"
//#include "G4LCapture.hh"
#include "G4NeutronRadCapture.hh"
#include "G4HadronFissionProcess.hh"
#include "G4LFission.hh"

#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"

//----------------------------------------------------------------------------//
void PhysicsList::ConstructInteractions()
{
  auto theParticleIterator=GetParticleIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
     
    if (particleName == "gamma") {
      // Standard classes
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());
      pmanager->AddDiscreteProcess(new G4ComptonScattering());
      pmanager->AddDiscreteProcess(new G4PolarizedCompton());
      pmanager->AddDiscreteProcess(new G4GammaConversion());
    } else if (particleName == "e-") {
      // Standard classes:
      pmanager->AddProcess(new G4eMultipleScattering(),-1, 1,1);
      pmanager->AddProcess(new G4eIonisation(),       -1, 2,2);
      pmanager->AddProcess(new G4eBremsstrahlung(),   -1,-1,3); 

    } else if (particleName == "e+") {
      // Standard classes:
      pmanager->AddProcess(new G4eMultipleScattering(),-1, 1,1);
      pmanager->AddProcess(new G4eIonisation(),       -1, 2,2);
      pmanager->AddProcess(new G4eBremsstrahlung(),   -1,-1,3);
      pmanager->AddProcess(new G4eplusAnnihilation(),  0,-1,4);
      
    } else if (particleName == "neutron") {
      /*
      // elastic scattering
      G4HadronElasticProcess* theElasticProcess =
                                    new G4HadronElasticProcess;
      G4HadronElastic* theElasticModel = new G4HadronElastic;
      theElasticProcess->RegisterMe(theElasticModel);
      pmanager->AddDiscreteProcess(theElasticProcess);
      // inelastic scattering
      //G4NeutronInelasticProcess* theInelasticProcess =
      //                           new G4NeutronInelasticProcess("inelastic");
      //G4RPGNeutronInelastic* theInelasticModel = new G4RPGNeutronInelastic;
      //theInelasticProcess->RegisterMe(theInelasticModel);
      //pmanager->AddDiscreteProcess(theInelasticProcess);
      // capture
      G4HadronCaptureProcess* theCaptureProcess =
                               new G4HadronCaptureProcess;
      G4NeutronRadCapture* theCaptureModel = new G4NeutronRadCapture;
      theCaptureProcess->RegisterMe(theCaptureModel);
      pmanager->AddDiscreteProcess(theCaptureProcess);
      // fission
      G4HadronFissionProcess* theFissionProcess =
                                 new G4HadronFissionProcess;
      G4LFission* theFissionModel = new G4LFission;
      theFissionProcess->RegisterMe(theFissionModel);
      pmanager->AddDiscreteProcess(theFissionProcess);
      */
    } else if( particleName == "mu+" ||
               particleName == "mu-"    ) {
    //muon  
     // Construct processes for muon+
     pmanager->AddProcess(new G4MuMultipleScattering(),-1,1,1);
     pmanager->AddProcess(new G4MuIonisation(),-1,2,2);
     pmanager->AddProcess(new G4MuBremsstrahlung(),-1,-1,3);
     pmanager->AddProcess(new G4MuPairProduction(),-1,-1,4);

    } else if( particleName == "GenericIon" ) {
      pmanager->AddProcess(new G4hMultipleScattering(),-1,1,1);
      pmanager->AddProcess(new G4hIonisation(),-1,2,2);
    } else {
      if ((particle->GetPDGCharge() != 0.0) &&
          (particle->GetParticleName() != "chargedgeantino")&&
          (!particle->IsShortLived()) ) {
     // all others charged particles except geantino
       pmanager->AddProcess(new G4hMultipleScattering(),-1,1,1);
       pmanager->AddProcess(new G4hIonisation(),-1,2,2);
      }
    }
  }
}
