/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class (Mandatory)

#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include <G4VUserDetectorConstruction.hh>
#include "G4Material.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials (with their physical properties) and detector geometry.
class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  /// constructor
  DetectorConstruction();
  virtual ~DetectorConstruction();

   virtual G4VPhysicalVolume *Construct();

  /// Defines the detector geometry and returns a pointer to the physical World Volume
    void ConstructMaterials();

  G4LogicalVolume *GetScintillator() const { return fScintillatorLogical; }
  G4LogicalVolume *GetWLS() const { return WLSFiberLogical; }
  G4LogicalVolume *GetSipm() const { return fSipmLogical; }

protected:
  G4LogicalVolume *fScintillatorLogical;
  G4LogicalVolume *fScintillatorLogicalR;
  G4LogicalVolume *WLSFiberLogical;
  G4LogicalVolume *fSipmLogical;
  
  /// Register some of the detector's volumes as "sensitive"
  // Add the following, if you have fields / sensitive detectors
  void ConstructSDandField() override;  
};

#endif
