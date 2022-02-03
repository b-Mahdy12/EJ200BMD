// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
#include "DetectorConstruction.hh"
#include "SD.hh"

#include "CADMesh.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4NistManager.hh"

#include <vector>
// #include "G4GDMLParser.hh"

#include "G4RunManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"

#include "G4MultiUnion.hh"
#include "G4TessellatedSolid.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include <G4SDManager.hh>
#include <G4MultiFunctionalDetector.hh>
#include <G4PSEnergyDeposit.hh>

using namespace std;

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction()
{
  ;
}

DetectorConstruction::~DetectorConstruction()
{
  ;
}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
  ConstructMaterials();

  // Get nist material manager
  G4NistManager *nist = G4NistManager::Instance();

  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box *solidWorld = new G4Box("World", 200 * m, 200 * m, 200 * m);
  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
  G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, checkOverlaps);

  //
  // Pyramid
  //
  G4Material *limestone = nist->FindOrBuildMaterial("G4_CALCIUM_CARBONATE");
  auto mesh = CADMesh::TessellatedMesh::FromSTL("Pyramid_of_Khafre.stl");
  mesh->SetScale(1000.0);
  // mesh->SetOffset(G4ThreeVector(-0.5 * scintillatorSizeX, 0, -0.5 * scintillatorSizeX));
  // mesh->SetOffset(G4ThreeVector(0,0,0));
  auto Khafre_py = mesh->GetSolid();

  G4RotationMatrix *rota = new G4RotationMatrix();
  rota->rotateX(90 * deg);

  G4LogicalVolume *Khafrelog = new G4LogicalVolume(Khafre_py, limestone, "Pyramid");

  fScoringVolume = Khafrelog;
  G4VPhysicalVolume *khafre_py = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), Khafrelog, "KhafrePyramid", logicWorld, false, 0, checkOverlaps);

  return physWorld;
}

// Implement the following only if you have fields / sensitive detector
void DetectorConstruction::ConstructSDandField()
{
  // SenDet* SD = new SenDet("detector");
  // Khafrelog-> SetSensitiveDetector(SD);
  /* 
  G4SDManager *sdManager = G4SDManager::GetSDMpointer();
  sdManager->SetVerboseLevel(2);

  G4MultiFunctionalDetector *detector = new G4MultiFunctionalDetector("detector");
  G4VPrimitiveScorer *scorer = new G4PSEnergyDeposit("EnergyDeposit");
  detector->RegisterPrimitive(scorer);

  SetSensitiveDetector("Pyramid", detector);
  sdManager->AddNewDetector(detector);
  */
}

void DetectorConstruction::ConstructMaterials()
{
  auto nistManager = G4NistManager::Instance();

  // Air
  nistManager->FindOrBuildMaterial("G4_AIR");

  // // Argon gas
  // nistManager->FindOrBuildMaterial("G4_Ar");

  // PLEXIGLASS
  nistManager->FindOrBuildMaterial("G4_PLEXIGLASS");

  // Polyvyneltoluene
  nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  G4cout << G4endl << "The materials defined are: " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}
