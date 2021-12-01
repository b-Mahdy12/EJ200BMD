//
//
//

#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

// Standard headers
#include <vector>
#include <sstream>

// CADMesh
#include "CADMesh.hh"

// GEANT4 headers
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4TessellatedSolid.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Transform3D.hh"
#include "G4GenericTrap.hh"
#include "G4GenericMessenger.hh"
#include "G4TwoVector.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4SDManager.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSDoseDeposit.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include <cmath>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction(const char *detectorName)
    : G4VUserDetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *DetectorConstruction::Construct()
{

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

  G4LogicalVolume *Khafre_log = new G4LogicalVolume(Khafre_py, limestone, "Pyramid");
  G4VPhysicalVolume *khafre_py = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), Khafre_log, "KhafrePyramid", logicWorld, false, 0, checkOverlaps);

/*
  //
  //  EJ200
  //
  ConstructMaterials();

  G4Material *air = G4Material::GetMaterial("G4_AIR");
  G4Material *polyvinyltoluene = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  // teflon wrapping?

  //G4Element* Cl = new G4Element("Chlorine", "Cl", 17., 35.5*g/mole);
  //G4Element* C = new G4Element("Carbon", "C", 6., 12.0*g/mole);
  //G4Element* H = new G4Element("Hydrozen", "H", 1., 1.00794*g/mole);
  //G4Element* O  = new G4Element("Oxygen", "O", 8., 16.00*g/mole);
  //G4Element *Al  = new G4Element("Aluminum", "Al", 13., 26.98*g/mole);

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

  // const G4int nCherenkovMomenta = 101;
  // G4double pCherenkov[nCherenkovMomenta] = {0.24796837486619944*eV, 0.25605986665241576*eV, 0.26447138957572464*eV, 0.2730929238614531*eV, 0.2820386429324379*eV, 0.29124779758773717*eV, 0.3007864809148465*eV, 0.310659452350538*eV, 0.32078703087477284*eV, 0.3313313400136283*eV, 0.34211972249751577*eV, 0.35333196760643976*eV, 0.3648740065718061*eV, 0.37685163353525747*eV, 0.38915313067514035*eV, 0.40189363835688724*eV, 0.41507930175125446*eV, 0.42856615082302013*eV, 0.44264258276722496*eV, 0.45716883271791925*eV, 0.47196112460258743*eV, 0.4875508746877692*eV, 0.5033868754896456*eV, 0.5198498424867913*eV, 0.5369605345738402*eV, 0.5544909992535765*eV, 0.572675230637874*eV, 0.5912455290085823*eV, 0.6107595440054173*eV, 0.630641848591555*eV, 0.6511774550057758*eV, 0.6727302627948981*eV, 0.6945892853394943*eV, 0.71750108468229*eV, 0.740646280962364*eV, 0.7648623530727928*eV, 0.7902115196500938*eV, 0.8156854436388139*eV, 0.842856474732153*eV, 0.8700644732147348*eV, 0.8984361408195632*eV, 0.9280253550381714*eV, 0.9581467344134444*eV, 0.9894987025786092*eV, 1.0221285031582827*eV, 1.055184573898721*eV, 1.090450197300789*eV, 1.1261052446239757*eV, 1.1630786813611604*eV, 1.2002341474646634*eV, 1.2398418743309971*eV, 1.280431554612204*eV, 1.3222159265553985*eV, 1.3656150174369392*eV, 1.4101932146621896*eV, 1.456410048550449*eV, 1.5039324045742324*eV, 1.5531026861217552*eV, 1.6039351543738642*eV, 1.6564353698476915*eV, 1.7105986124875787*eV, 1.766659838032199*eV, 1.8243700328590307*eV, 1.8839718497659885*eV, 1.9457656533757017*eV, 2.0094681917844364*eV, 2.075049162060246*eV, 2.1428307541151006*eV, 2.2132129138361245*eV, 2.285422809826723*eV, 2.360254853095369*eV, 2.437275160862978*eV, 2.5169343774482282*eV, 2.5992492124339566*eV, 2.684802672869201*eV, 2.7724549962678826*eV, 2.8633761531893698*eV, 2.956932683832571*eV, 3.0537977200270863*eV, 3.153209242957775*eV, 3.2567425120330897*eV, 3.362739013645232*eV, 3.472946426697471*eV, 3.586467672348849*eV, 3.7043378378577745*eV, 3.8254917443103893*eV, 3.9497988987926*eV, 4.079769247551817*eV, 4.212850405473996*eV, 4.350322366073675*eV, 4.493808895726702*eV, 4.640126775190858*eV, 4.792585521186692*eV, 4.949468560203582*eV, 5.110642515791414*eV, 5.278168898812249*eV, 5.449854392663723*eV, 5.627970378261448*eV, 5.8126670151476665*eV, 6.004076873273594*eV, 6.199209371654986*eV};
  // G4double refractiveIndex[nCherenkovMomenta] = {1.6240317502663,1.6332058556533,1.6417334442682,1.6495581507485,1.6568352356965,1.663555792699,1.6698063350772,1.6756189316007,1.6809814756374,1.6860063236089,1.6906368746825,1.6949749939614,1.6990024419669,1.7027737381029,1.7062709711188,1.709543722848,1.7126051747702,1.715438147562,1.7181145792243,1.7206148427182,1.7229226773359,1.7251280028472,1.7271600353641,1.7290784375914,1.730889247653,1.7325755211079,1.7341669205685,1.7356472403196,1.7370650289788,1.7383827319917,1.7396263439883,1.7408195373476,1.7419273828406,1.742992131424,1.7439802356172,1.7449319592359,1.7458498390026,1.7467019118788,1.7475432140301,1.7483249348844,1.7490839326181,1.7498225628591,1.7505264589423,1.7512147150232,1.751889641765,1.7525364667588,1.7531915603611,1.7538227008788,1.7544492215126,1.7550547342523,1.7556780777757,1.7562973768044,1.7569182191283,1.7575488420438,1.7581849835841,1.7588353492177,1.7594974153468,1.7601781977445,1.7608801757554,1.7616058134504,1.7623575309256,1.7631412029905,1.7639560666816,1.7648083776856,1.7657055418929,1.7666467187031,1.7676347594555,1.768678092594,1.7697870034397,1.7709534510436,1.7721946418157,1.7735082113872,1.7749069668851,1.7763969806669,1.7779954669163,1.7796876970942,1.7815032886078,1.7834376078764,1.7855131978094,1.7877228296252,1.7901123299044,1.792654904549,1.7954050494599,1.7983552384705,1.8015488081449,1.8049747310816,1.8086465749755,1.812662211763,1.8169687643807,1.8216335887885,1.8267479542496,1.8322345043326,1.8382585951889,1.8448039892896,1.8519174110987,1.859757418441,1.8682985747984,1.8777446762354,1.8882235599463,1.8998877983536,1.9127094080587};
  // G4MaterialPropertiesTable *sapphireProperties = new G4MaterialPropertiesTable();
  // sapphireProperties->AddProperty("RINDEX", pCherenkov, refractiveIndex, nCherenkovMomenta)->SetSpline(true);
  // sapphire->SetMaterialPropertiesTable(sapphireProperties);

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

  // WLS_FIBER_DIFINITION
  // const G4int nAEntries = 15;
  // G4double PhotonEnergy_h1[nAEntries] = {
  //     0.548304131 * eV, 0.535216907 * eV, 0.518731399 * eV,
  //     0.506254792 * eV, 0.495073725 * eV, 0.484343772 * eV, 0.475383605 * eV, 0.470665497 * eV, 0.464103308 * eV,
  //     0.457092843 * eV, 0.452104715 * eV, 0.446625037 * eV, 0.441262784 * eV, 0.436048961 * eV, 0.430408888 * eV};
  // G4double RIndexFiber[nAEntries] = {1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57};
  // G4double AbsFiber[nAEntries] = {
  //     0.13527684 * eV, 0.23580736 * eV, 0.46826649 * eV, 0.70358544 * eV, 0.90098064 * eV,
  //     0.97239321 * eV, 0.90155025 * eV, 0.74183769 * eV, 0.60341824 * eV, 0.469225 * eV,
  //     0.31787044 * eV, 0.18939904 * eV, 0.07279204 * eV, 0.02666689 * eV, 0.00564001 * eV};
  // const G4int nEEntries = 17;
  // G4double PhotonEnergy_h2[nEEntries] = {
  //     0.423852298 * eV, 0.419616975 * eV,
  //     0.417552542 * eV, 0.41706186 * eV, 0.415041636 * eV, 0.413573284 * eV, 0.411576333 * eV,
  //     0.410594897 * eV, 0.400337959 * eV, 0.38641104 * eV, 0.377208808 * eV, 0.367629515 * eV,
  //     0.359309413 * eV, 0.350625247 * eV, 0.343416853 * eV, 0.33515463 * eV, 0.39234228 * eV};
  // G4double EmissionFiber[nEEntries] = {
  //     0.00096721 * eV, 0.006241 * eV,
  //     0.02795584 * eV, 0.06528025 * eV, 0.16208676 * eV, 0.37920964 * eV, 0.53260804 * eV,
  //     0.66928761 * eV, 1.00420441 * eV, 0.64016001 * eV, 0.35772361 * eV, 0.17181025 * eV,
  //     0.08392609 * eV, 0.03359889 * eV, 0.01646089 * eV, 0.00840889 * eV, 0.89000356 * eV};

  //
  // Scintillator
  //
  G4double envSizeX = 50. * cm;
  // auto meshSci = CADMesh::TessellatedMesh::FromSTL("EJ200Scibar_Ascii.stl");
  // meshSci->SetScale(0.5);
  // mesh->SetOffset(G4ThreeVector(-0.5 * scintillatorSizeX, 0, -0.5 * scintillatorSizeX));
  // mesh->SetOffset(G4ThreeVector(0,0,0));
  // auto EJ200Scibar = meshSci->GetSolid();
  // fScintillatorLogical = new G4LogicalVolume(EJ200Scibar, polyvinyltoluene, "ScintillatorLogical");
  // G4VPhysicalVolume *physScint1 = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), fScintillatorLogical, "ScintillatorPhysical", Khafre_log, false, 0, checkOverlaps);
  // G4VPhysicalVolume *physScint2 = new G4PVPlacement(0, G4ThreeVector(0., 0., -2.), fScintillatorLogical, "ScintillatorPhysical", Khafre_log, false, 0, checkOverlaps);

  //
  // WLS Fiber
  //
  auto meshWire = CADMesh::TessellatedMesh::FromSTL("WLS_Ascii.stl");
  // meshWire->SetScale(0.5);
  // meshWire->SetOffset(G4ThreeVector(-0.5 * scintillatorSizeX, 0, -0.5 * scintillatorSizeX));
  // auto Wire = meshWire->GetSolid();
  // G4Material *Fib = G4Material::GetMaterial("G4_PLEXIGLASS");
  // double density = 2.700 * g / cm3;
  // double a = 26.98 * g / mole;
  // G4Material *Al = new G4Material("Aluminum", 13., a, density);
  // WLSFiberLogical = new G4LogicalVolume(Wire, Fib, "WireLogical");
  // G4VPhysicalVolume *WirePhy = new G4PVPlacement(0, G4ThreeVector(0, 0, 0.), WLSFiberLogical, "WwirePhysical", fScintillatorLogical, false, 0, checkOverlaps);

  //
  // SiPM
  //
  auto meshSiPM = CADMesh::TessellatedMesh::FromSTL("SiPM_Ascii.stl");
  // meshSiPM->SetScale(0.5);
  // mesh->SetOffset(G4ThreeVector(-0.5 * scintillatorSizeX, 0, -0.5 * scintillatorSizeX));
  // meshSiPM->SetOffset(G4ThreeVector(-0.065, -0.05, 0));
  // auto SiPM = meshSiPM->GetSolid();
  // fSipmLogical = new G4LogicalVolume(SiPM, silicon, "SiPMLogical");
  // G4VPhysicalVolume *SiPMPhy = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), fSipmLogical, "SiPMPhysical", fScintillatorLogical, false, 0, checkOverlaps);

  //
  // VISUAL ATTRIBUTE
  //
  G4VisAttributes *bore = new G4VisAttributes(G4Colour(0.9, 0.5, 1.));
  bore->SetVisibility(true);
  bore->SetForceWireframe(true);
  //  // bore->SetForceSolid(false);
  // fScintillatorLogical->SetVisAttributes(bore);

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
  // G4MaterialPropertiesTable *teflonSurfaceProperties = new G4MaterialPropertiesTable();
  // teflonSurfaceProperties->AddProperty("REFLECTIVITY", scintResponseWavelengths, teflonReflectivity, NPHOTONENERGIES);
  // teflonSurfaceProperties->AddProperty("EFFICIENCY", scintResponseWavelengths, teflonEfficiency, NPHOTONENERGIES);
  // teflonSurface->SetMaterialPropertiesTable(teflonSurfaceProperties);
  //
  // auto rod1 = new G4LogicalBorderSurface("TeflonBorderSurface1", WirePhy, physScint1, teflonSurface);
  // auto rod2 = new G4LogicalBorderSurface("TeflonBorderSurface1", WirePhy, physScint2, teflonSurface);

  // pyrSD *pySD = new pyrSD("/Py");
  // G4SDManager *sdman = G4SDManager ::GetSDMpointer();

  // Khafre_log->SetSensitiveDetector(pySD);
  // fScintillatorLogical->SetSensitiveDetector(pySD);
*/
  return physWorld;
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

//method 2
void DetectorConstruction::ConstructSDandField()
{
  G4SDManager *sdManager = G4SDManager::GetSDMpointer();
  sdManager->SetVerboseLevel(2);

  G4MultiFunctionalDetector *pySD = new G4MultiFunctionalDetector("Py");
  G4VPrimitiveScorer *scorer = new G4PSEnergyDeposit("EnergyDeposit");
  pySD->RegisterPrimitive(scorer);

  SetSensitiveDetector("Pyramid", pySD);
  sdManager->AddNewDetector(pySD);

  // approach with hits
  // auto Scintbars = new ScintbarSD(SDname = "/Scintbars");
  // G4SDManager::GetSDMpointer()->AddNewDetector(Scintbars);
  // //fScoringVolume->SetSensitiveDetector(Scintbars);
  // GetScoringVolume()->SetSensitiveDetector(Khafre_log);
}

// Track must hit front and back layer
G4bool DetectorConstruction::IsInsideAcceptance(const G4ThreeVector &pos, const G4ThreeVector &dir) const
{
}

void DetectorConstruction::DeleteMessenger()
{
}
