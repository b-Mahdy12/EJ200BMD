/// \file PrimaryGeneratorAction.hh
/// \brief Definition of the PrimaryGeneratorAction class

#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"

#include <array>

// FOWARD DECLARATIONS
class G4GeneralParticleSource;

/// The primary generator action class
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        ///constructor
        PrimaryGeneratorAction();
        ///destructor
        ~PrimaryGeneratorAction();

        ///main interface
        void GeneratePrimaries(G4Event* anEvent) override;

    private:
        bool virgin;

        G4GeneralParticleSource* gps;
};

#endif
