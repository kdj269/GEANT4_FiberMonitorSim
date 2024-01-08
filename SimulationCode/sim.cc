#include <iostream>

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"

int main(int argc, char** argv)
{
    G4UIExecutive* ui = nullptr;
    if ( argc == 1 ) { ui = new G4UIExecutive(argc, argv); }

    //G4RunManager* runManager = new G4RunManager;//for non multothread
    G4MTRunManager* runManager = new G4MTRunManager;

    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysicsList());
    runManager->SetUserInitialization(new MyActionInitialization());

    //load in settings from file
    static std::ifstream runFile("runSettings.csv");
    G4int NThreads;
    G4int NRuns;
    G4int RunUI;
    runFile >> NThreads;
    runFile >> NRuns;
    runFile >> RunUI;
    runFile.close();

    runManager->SetNumberOfThreads(NThreads);

    runManager->SetVerboseLevel(0);

    runManager->Initialize();

    runManager->BeamOn(NRuns);

    if (RunUI == 1) {

        // Get the pointer to the User Interface manager
        G4UImanager* UImanager = G4UImanager::GetUIpointer();

        // Process macro or start UI session
        //


        if ( ! ui ) {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
        }
        else {
            G4cout << "you need to add a visulisation macro \n";


            G4VisManager *visManager = new G4VisExecutive();
            visManager->Initialize();

            UImanager->ApplyCommand("/vis/open OGL");
            UImanager->ApplyCommand("/vis/drawVolume");
            UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1");
            UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
            UImanager->ApplyCommand("/vis/viewer/set/autorefresh true");
            UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");

  
            ui->SessionStart();
        }
    }
  return 0;
}
