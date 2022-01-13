/// \file PrimaryGeneratorAction.hh
/// \brief Implementation of the PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include <stdlib.h>

#include <G4Event.hh>
#include <G4Gamma.hh>
#include <G4GeneralParticleSource.hh>
#include <G4ParticleTable.hh>
#include <G4SystemOfUnits.hh>

PrimaryGeneratorAction::PrimaryGeneratorAction() :
    G4VUserPrimaryGeneratorAction(),
    virgin(true) // VIRGIN SINCE IT HASN'T BEEN USED BY ANY THREAD
{
    // CREATE SOURCE
    gps = new G4GeneralParticleSource();

    G4ParticleDefinition* myParticle = G4Gamma::Definition();
    myParticle = G4ParticleTable::GetParticleTable()->FindParticle("mu-");

    gps->SetParticleDefinition(myParticle);
    gps->GetCurrentSource()->GetPosDist()->SetPosDisType("Surface");
    gps->GetCurrentSource()->GetPosDist()->SetPosDisShape("Sphere");
    gps->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0, 0, 0));
    gps->GetCurrentSource()->GetPosDist()->SetRadius(70 * m);
    gps->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
    gps->GetCurrentSource()->GetEneDist()->SetMonoEnergy(4 * GeV);
    gps->GetCurrentSource()->GetAngDist()->SetAngDistType("focused");
    gps->GetCurrentSource()->GetAngDist()->SetFocusPoint({0, 0, 0});
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete gps;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    if (virgin) {
        virgin = false;

        // THE PHYSICAL VOLUME DOESN'T EXIST UNTIL THIS IS CALLED FOR THE
        // FIRST TIME, HENCE WE ONLY DO THIS ONCE
        gps->GetCurrentSource()
            ->GetPosDist()
            ->ConfineSourceToVolume("world");
    }

    gps->GeneratePrimaryVertex(anEvent);
}
