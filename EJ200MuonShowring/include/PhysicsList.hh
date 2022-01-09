/// \file PhysicsList.hh
/// \brief Definition of the PhysicsList class (Mandatory)

#ifndef PHYSICS_LIST_HH
#define PHYSICS_LIST_HH

#include <G4VModularPhysicsList.hh>

/// Modular physics list

class PhysicsList : public G4VModularPhysicsList
{
public:
  ///constructor
  PhysicsList();

  /// destructor
  virtual ~PhysicsList();

  /// Builds particles
  void ConstructParticle() override;

  /// Build processes
  void ConstructProcess() override;

  /// Set user cuts
  void SetCuts() override;

protected:
  // these methods Construct physics processes and register them
  void ConstructGeneral();
  void ConstructInteractions();
};

#endif
