/// \file ActionInitialization.hh
/// \brief Definition of the ActionInitialization class (Mandatory)

#ifndef ACTION_INITIALIZATION_HH
#define ACTION_INITIALIZATION_HH

#include <G4VUserActionInitialization.hh>

/// Action initialization class.
///
class ActionInitialization : public G4VUserActionInitialization
{
public:
  ActionInitialization();
  ~ActionInitialization();

  void Build() const;
  void BuildForMaster() const;
};

#endif
