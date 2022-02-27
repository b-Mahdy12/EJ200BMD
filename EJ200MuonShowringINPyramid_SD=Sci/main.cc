#include <vector>
#include <fstream>
#include <math.h>
#include "G4ios.hh"

// #ifdef G4VIS_USE
#include <G4VisExecutive.hh>
// #endif

// #ifdef G4UI_USE
#include <G4UIExecutive.hh>
// #endif

#include <G4RunManager.hh>
#include <G4String.hh>
#include <G4UImanager.hh>
#include <G4ScoringManager.hh>

// #include "ROOTManager.hh"
#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "MuravesMessenger.hh"

#include "Randomize.hh"

using namespace std;

// extern DetectorConstruction* theDetector;

/* Main function that enables to:
 * - run any number of macros (put them as command-line arguments)
 * - start interactive UI mode (no arguments or "-i")
 */
int main(int argc, char *argv[])
{

    auto theMessenger = new MuravesMessenger();

     G4String inputfile="";
  
  G4int iarg = 1;
  if (argc > 1) {
    while ( iarg < argc ) {
      if (G4String(argv[iarg]).compare("--generator") == 0) {
	if ( ++iarg < argc ) {
	  if( (G4String(argv[iarg]).compare("CRY") == 0)
	       || (G4String(argv[iarg]).compare("PartGun") == 0)
	       || (G4String(argv[iarg]).compare("GPS") == 0) ) {
	    theMessenger->SetPrimaryGenerator(G4String(argv[iarg]));
	    iarg++;
	  }
	  else {
	    G4cout << "Unknown generator option " << argv[iarg] << G4endl;
	    return EXIT_FAILURE;
	  }
	} else {
	  G4cout << "Missing generator choice" << G4endl;
	  return EXIT_FAILURE;
	}
      }
      else if (G4String(argv[iarg]).compare("--inputfile") == 0) {
	if ( ++iarg < argc ) {
	  inputfile = G4String(argv[iarg]);
	  iarg++;
	} else {
	  G4cout << "Missing inputfile" << G4endl;
	  return EXIT_FAILURE;
	}
      }
      else {
	G4cout << "Unknown option " << argv[iarg] << G4endl;
	return EXIT_FAILURE;
      }      
    } 
  }

    // vector<G4String> macros;
    bool interactive = false;

    // Parse command line arguments
    /*
    if (argc == 1)
    {
        interactive = true;
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            G4String arg = argv[i];
            if (arg == "-i" || arg == "--interactive")
            {
                interactive = true;
                continue;
            }
            else
            {
                macros.push_back(arg);
            }
        }
    }
    */

    //Use sequential mode only
    G4RunManager *runManager = new G4RunManager();
    runManager->SetVerboseLevel(2);

#ifdef G4VIS_USE
    G4VisManager *visManager = new G4VisExecutive();
    visManager->SetVerboseLevel(0); // Default, you can always override this using macro commands
    visManager->Initialize();
#endif

    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new ActionInitialization());

// User interactions
  //------------------
  G4UImanager* UI = G4UImanager::GetUIpointer();  

  //if (argc > 1) {  //....geant command file specified on command line
  //  UI->ApplyCommand("/control/execute "+G4String(argv[1]));
  if ( inputfile != "" ) {
    UI->ApplyCommand("/control/execute " + inputfile);
  } else {           //....no command file specified, Start interactive session 
    UI->ApplyCommand("/control/execute init_vis.mac");
    //G4UIsession* theUIsession = new G4UIterminal(new G4UItcsh);
    G4UIExecutive* theUIsession = new G4UIExecutive(argc, argv);
    theUIsession->SessionStart();
    delete theUIsession;
  }

    delete runManager;
    return 0;
}
