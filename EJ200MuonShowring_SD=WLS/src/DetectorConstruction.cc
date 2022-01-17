// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
#include "DetectorConstruction.hh"
#include "SD.hh"

#include "CADMesh.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4NistManager.hh"

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
// #include <G4MultiFunctionalDetector.hh>
// #include <G4PSEnergyDeposit.hh>

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

  G4Material *air = G4Material::GetMaterial("G4_AIR");
  G4Material *polyvinyltoluene = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  G4Material *Fib = G4Material::GetMaterial("G4_PLEXIGLASS");

  G4MaterialPropertiesTable *scintillatorProperties = new G4MaterialPropertiesTable();
  scintillatorProperties->AddConstProperty("SCINTILLATIONYIELD", 10000. / MeV);
  scintillatorProperties->AddConstProperty("FASTTIMECONSTANT", 0.7 * ns);
  scintillatorProperties->AddConstProperty("RESOLUTIONSCALE", 1.0);
  scintillatorProperties->AddConstProperty("YIELDRATIO", 1.0);
  //
  const G4int NPHOTONENERGIES = 37;
  G4double scintResponseWavelengths[NPHOTONENERGIES] = {
      0.4906 * eV, 0.4868 * eV, 0.4838 * eV, 0.4816 * eV, 0.4797 * eV, 0.4779 * eV, 0.4761 * eV, 0.4740 * eV, 0.4732 * eV,
      0.4718 * eV, 0.4704 * eV, 0.4680 * eV, 0.4662 * eV, 0.4631 * eV, 0.4598 * eV, 0.4561 * eV,
      0.4525 * eV, 0.4506 * eV, 0.4483 * eV, 0.4448 * eV, 0.4423 * eV, 0.4396 * eV, 0.4365 * eV,
      0.4332 * eV, 0.4297 * eV, 0.4256 * eV, 0.4227 * eV, 0.4202 * eV, 0.4180 * eV, 0.4152 * eV,
      0.4122 * eV, 0.4090 * eV, 0.4064 * eV, 0.4030 * eV, 0.3999 * eV, 0.3969 * eV, 0.3525 * eV};
  G4double scintResponseSpectrum[NPHOTONENERGIES] = {
      0.00076176 * eV, 0.00687241 * eV, 0.02745649 * eV, 0.06180196 * eV, 0.09235521 * eV, 0.16265089 * eV, 0.25836889 * eV,
      0.33651601 * eV, 0.454276 * eV, 0.59830225 * eV, 0.72386064 * eV, 0.85119076 * eV, 0.97792321 * eV,
      1.01103025 * eV, 0.95628841 * eV, 0.88209664 * eV, 0.76195441 * eV, 0.67765824 * eV, 0.57289761 * eV,
      0.47692836 * eV, 0.38975049 * eV, 0.33016516 * eV, 0.28718881 * eV, 0.23107249 * eV, 0.20025625 * eV,
      0.16265089 * eV, 0.13293316 * eV, 0.10265616 * eV, 0.07033104 * eV, 0.05130225 * eV, 0.03323329 * eV,
      0.01907161 * eV, 0.01221025 * eV, 0.00881721 * eV, 0.00781456 * eV, 0.00439569 * eV, 0.37613689 * eV};
  G4double scintRefractiveIndex[NPHOTONENERGIES];
  G4double scintAbsLength[NPHOTONENERGIES];
  for (int i = 0; i < NPHOTONENERGIES; i++)
  {
    scintRefractiveIndex[i] = 1.58;
    scintAbsLength[i] = 380. * cm;
  }
  scintillatorProperties->AddProperty("FASTCOMPONENT", scintResponseWavelengths, scintResponseSpectrum, NPHOTONENERGIES);
  scintillatorProperties->AddProperty("RINDEX", scintResponseWavelengths, scintRefractiveIndex, NPHOTONENERGIES);
  scintillatorProperties->AddProperty("ABSLENGTH", scintResponseWavelengths, scintAbsLength, NPHOTONENERGIES);

  polyvinyltoluene->SetMaterialPropertiesTable(scintillatorProperties);

  // SiPM_WINDOW_DEFINITION
  double siliconDensity = 2.33 * g / cm3;
  double siliconAtomicWeight = 28.0855 * g / mole;
  G4Material *silicon = new G4Material("silicon", 14., siliconAtomicWeight, siliconDensity);
  const G4int k_ = 1;
  G4double pCherenkovsilicon[k_] = {2. * GeV};
  G4double siliconRefractiveIndex[k_] = {1.57};
  G4MaterialPropertiesTable *siliconProperties = new G4MaterialPropertiesTable();
  siliconProperties->AddProperty("RINDEX", pCherenkovsilicon, siliconRefractiveIndex, k_)->SetSpline(true);
  silicon->SetMaterialPropertiesTable(siliconProperties);
  /*

  // WLS_FIBER_DIFINITION
  const G4int nAEntries = 15;
  G4double PhotonEnergy_h1[nAEntries] = {
      0.548304131 * eV, 0.535216907 * eV, 0.518731399 * eV,
      0.506254792 * eV, 0.495073725 * eV, 0.484343772 * eV, 0.475383605 * eV, 0.470665497 * eV, 0.464103308 * eV,
      0.457092843 * eV, 0.452104715 * eV, 0.446625037 * eV, 0.441262784 * eV, 0.436048961 * eV, 0.430408888 * eV};
  G4double RIndexFiber[nAEntries] = {1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57};
  G4double AbsFiber[nAEntries] = {
      0.13527684 * eV, 0.23580736 * eV, 0.46826649 * eV, 0.70358544 * eV, 0.90098064 * eV,
      0.97239321 * eV, 0.90155025 * eV, 0.74183769 * eV, 0.60341824 * eV, 0.469225 * eV,
      0.31787044 * eV, 0.18939904 * eV, 0.07279204 * eV, 0.02666689 * eV, 0.00564001 * eV};
  const G4int nEEntries = 17;
  G4double PhotonEnergy_h2[nEEntries] = {
      0.423852298 * eV, 0.419616975 * eV,
      0.417552542 * eV, 0.41706186 * eV, 0.415041636 * eV, 0.413573284 * eV, 0.411576333 * eV,
      0.410594897 * eV, 0.400337959 * eV, 0.38641104 * eV, 0.377208808 * eV, 0.367629515 * eV,
      0.359309413 * eV, 0.350625247 * eV, 0.343416853 * eV, 0.33515463 * eV, 0.39234228 * eV};
  G4double EmissionFiber[nEEntries] = {
      0.00096721 * eV, 0.006241 * eV,
      0.02795584 * eV, 0.06528025 * eV, 0.16208676 * eV, 0.37920964 * eV, 0.53260804 * eV,
      0.66928761 * eV, 1.00420441 * eV, 0.64016001 * eV, 0.35772361 * eV, 0.17181025 * eV,
      0.08392609 * eV, 0.03359889 * eV, 0.01646089 * eV, 0.00840889 * eV, 0.89000356 * eV};
  //
  G4Material *WLS;
  // was Causes segmintation violation
  G4MaterialPropertiesTable *WLSmaterial = new G4MaterialPropertiesTable();
  WLSmaterial->AddProperty("RINDEX", PhotonEnergy_h1, RIndexFiber, nEEntries);
  WLSmaterial->AddProperty("WLSABSLENGTH", PhotonEnergy_h1, AbsFiber, nAEntries);
  WLSmaterial->AddProperty("WLSCOMPONENT", PhotonEnergy_h1, EmissionFiber, nEEntries);
  WLSmaterial->AddConstProperty("WLSTIMECONSTANT", 2.7 * ns);
  //
  WLS->SetMaterialPropertiesTable(WLSmaterial);
*/

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4Material *worldMaterial = air;

  G4Box *solidWorld = new G4Box("World", 200 * cm, 200 * cm, 200 * cm);
  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMaterial, "World");
  G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, checkOverlaps);

  G4RotationMatrix *rota = new G4RotationMatrix();
  rota->rotateX(0 * deg);

  //
  // Scintillator
  //
  auto mesh = CADMesh::TessellatedMesh::FromSTL("EJ200Scibar_Ascii.stl");
  mesh->SetScale(10.0);
  // mesh->SetOffset(G4ThreeVector(-0.5 * scintillatorSizeX, 0, -0.5 * scintillatorSizeX));
  // mesh->SetOffset(G4ThreeVector(0,0,0));
  auto EJ200Scibar = mesh->GetSolid();
  fScintillatorLogical = new G4LogicalVolume(EJ200Scibar, polyvinyltoluene, "ScintillatorLogical");

  // fScoringVolume = fScintillatorLogical;
  // G4VPhysicalVolume *physScint1 = new G4PVPlacement(rota, G4ThreeVector(-0.5 * envSizeX, 0, 0.5 * envSizeX), fScintillatorLogical, "ScintillatorPhysical", logicWorld, false, 0, checkOverlaps);
  G4VPhysicalVolume *physScint1 = new G4PVPlacement(rota, G4ThreeVector(-25. * cm, -25 * cm, 0. * cm), fScintillatorLogical, "ScintillatorPhysical", logicWorld, false, 0, false);
  G4VPhysicalVolume *physScint2 = new G4PVPlacement(rota, G4ThreeVector(-25. * cm, -25 * cm, 0. + 2. * cm), fScintillatorLogical, "ScintillatorPhysical", logicWorld, false, 0, false);

  //
  // WLS Fiber
  //
  auto meshSWire = CADMesh::TessellatedMesh::FromSTL("EJ200Scibar_singlewire_Ascii.stl");
  meshSWire->SetScale(10.0);
  auto wire = meshSWire->GetSolid();
  WLSFiberLogical = new G4LogicalVolume(wire, Fib, "SingleWireLogical");
  fScoringVolume = WLSFiberLogical;

  G4RotationMatrix *rotb = new G4RotationMatrix();
  rotb->rotateZ(90 * deg);

  for (int d = 1.1 * cm; d < 60 * cm; d = d + 1 * cm)
  {
    G4VPhysicalVolume *wirePhyX = new G4PVPlacement(rotb, G4ThreeVector(d, 60 * cm, 0.05 * cm), WLSFiberLogical, "WLS_x", fScintillatorLogical, false, 0, checkOverlaps);
    G4VPhysicalVolume *wirePhyY = new G4PVPlacement(0, G4ThreeVector(0, d, 1.7 * cm), WLSFiberLogical, "WLS_y", fScintillatorLogical, false, 0, checkOverlaps);
  }

  /*
  auto meshWire = CADMesh::TessellatedMesh::FromSTL("WLS_Ascii.stl");
  meshWire->SetScale(10.0);
  // meshWire->SetOffset(G4ThreeVector(-0.5 * scintillatorSizeX, 0, -0.5 * scintillatorSizeX));
  auto Wire = meshWire->GetSolid();
  // double density = 2.700 * g / cm3;
  // double a = 26.98 * g / mole;
  // G4Material *Al = new G4Material("Aluminum", 13., a, density);
  WLSFiberLogical = new G4LogicalVolume(Wire, Fib, "WireLogical");
  // fScoringVolume = WLSFiberLogical;

  G4VPhysicalVolume *WirePhy = new G4PVPlacement(0, G4ThreeVector(0, 0, 0.), WLSFiberLogical, "wirePhysical", fScintillatorLogical, false, 0, checkOverlaps);
*/
  //
  // SiPM
  //
  auto meshSSipm = CADMesh::TessellatedMesh::FromSTL("EJ200Scibar_singlesipm_Ascii.stl");
  meshSWire->SetScale(10.0);
  auto SiPM = meshSWire->GetSolid();
  fSipmLogical = new G4LogicalVolume(SiPM, silicon, "SiPMLogical");
  // fScoringVolume = fSipmLogical;

  G4VPhysicalVolume *SipmPhy = new G4PVPlacement(0, G4ThreeVector(0 * cm, 0 * cm, 0 * cm), fSipmLogical, "SiPM", WLSFiberLogical, false, 0, checkOverlaps);

  /*  
  auto meshSiPM = CADMesh::TessellatedMesh::FromSTL("SiPM_Ascii.stl");
  meshSiPM->SetScale(10.0);
  // mesh->SetOffset(G4ThreeVector(-0.5 * scintillatorSizeX, 0, -0.5 * scintillatorSizeX));
  // meshSiPM->SetOffset(G4ThreeVector(-1.5 * mm, -2 * mm, -0.15 * mm));
  auto SiPM = meshSiPM->GetSolid();
  fSipmLogical = new G4LogicalVolume(SiPM, silicon, "SiPMLogical");
  G4VPhysicalVolume *SiPMPhy = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), fSipmLogical, "SiPMPhysical", WLSFiberLogical, false, 0, checkOverlaps);
*/
  //
  // visualization attributes
  //
  G4VisAttributes *whitecol = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
  G4VisAttributes *pinkcol = new G4VisAttributes(G4Colour(0.6, 0.0, 0.6));
  G4VisAttributes *graycol = new G4VisAttributes(G4Colour(0.9, 0.9, 0.9));
  G4VisAttributes *cyancol = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0, 0.3));
  G4VisAttributes *redcol = new G4VisAttributes(G4Colour(0.5, 0.0, 0.0));
  G4VisAttributes *darkgraycol = new G4VisAttributes(G4Colour(0.8, 0.8, 0.8));
  G4VisAttributes *orangecol = new G4VisAttributes(G4Colour(0.8, 0.5, 0.));
  G4VisAttributes *yellowcol = new G4VisAttributes(G4Colour(1.0, 1.0, 0.));
  G4VisAttributes *greencol = new G4VisAttributes(G4Colour(0., 1.0, 0., 0.4));
  G4VisAttributes *bluecol = new G4VisAttributes(G4Colour(0., 0., 0.8));
  //  // bore->SetForceSolid(false);
  fScintillatorLogical->SetVisAttributes(redcol);
  WLSFiberLogical->SetVisAttributes(yellowcol);
  fSipmLogical->SetVisAttributes(cyancol);

  // G4OpticalSurface *teflonSurface = new G4OpticalSurface("TeflonSurface");
  // teflonSurface->SetType(dielectric_LUTDAVIS);
  // teflonSurface->SetModel(DAVIS);
  // teflonSurface->SetFinish(Rough_LUT);
  // //teflonSurface->SetSigmaAlpha(sigma_alpha);
  //
  // G4double teflonReflectivity[NPHOTONENERGIES];
  // G4double teflonEfficiency[NPHOTONENERGIES];
  // for (int i = 0; i < NPHOTONENERGIES; i++)
  // {
  //   teflonReflectivity[i] = 0.99;
  //   teflonEfficiency[i] = 1.;
  // }
  /*const G4int NUM = 2;
  G4double pp[NUM] = {2.038*eV, 4.144*eV};
  G4double specularlobe[NUM] = {0.3, 0.3};
  G4double specularspike[NUM] = {0.2, 0.2};
  G4double backscatter[NUM] = {0.1, 0.1};
  G4double rindex[NUM] = {1.35, 1.40};
  G4double reflectivity[NUM] = {0.3, 0.5};
  G4double efficiency[NUM] = {0.8, 0.1};*/

  //   G4MaterialPropertiesTable *teflonSurfaceProperties = new G4MaterialPropertiesTable();
  /*teflonSurfaceProperties->AddProperty("RINDEX",pp,rindex,NUM);
  teflonSurfaceProperties->AddProperty("SPECULARLOBECONSTANT",pp,specularlobe,NUM);
  teflonSurfaceProperties->AddProperty("SPECULARSPIKECONSTANT",pp,specularspike,NUM);
  teflonSurfaceProperties->AddProperty("BACKSCATTERCONSTANT",pp,backscatter,NUM);
  teflonSurfaceProperties->AddProperty("REFLECTIVITY", scintResponseWavelengths, teflonReflectivity, NPHOTONENERGIES);
  teflonSurfaceProperties->AddProperty("EFFICIENCY", scintResponseWavelengths, teflonEfficiency, NPHOTONENERGIES);
  teflonSurface->SetMaterialPropertiesTable(teflonSurfaceProperties);
*/
  //   auto rod1 = new G4LogicalBorderSurface("TeflonBorderSurface1", WirePhy, physScint1, teflonSurface);
  // auto rod2 = new G4LogicalBorderSurface("TeflonBorderSurface1", WirePhy, physScint2, teflonSurface);

  // auto rod1 = new G4LogicalBorderSurface("TeflonBorderSurface1", W, W, teflonSurface);
  // auto rod2 = new G4LogicalBorderSurface("TeflonBorderSurface2", W, W, teflonSurface);

  return physWorld;
}

// Implement the following only if you have fields / sensitive detector
void DetectorConstruction::ConstructSDandField()
{
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);
  G4String SDname;
/*
  //
  // EJ200 SensitiveDetector
  //
  SenDet *SciPlaneSD = new SenDet(SDname = "/ScinPlane");
  G4SDManager::GetSDMpointer()->AddNewDetector(SciPlaneSD);
  GetScoringVolume()->SetSensitiveDetector(SciPlaneSD);
*/
  
  //
  // WLS SensitiveDetector
  //
  SenDet *WLSSD = new SenDet(SDname = "/WLSWire");
  G4SDManager::GetSDMpointer()->AddNewDetector(WLSSD);
  GetScoringVolume()->SetSensitiveDetector(WLSSD);
  
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
