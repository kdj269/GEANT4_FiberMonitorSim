#ifndef STEPPING_HH
#define STEPPING_HH

#include "G4UserSteppingAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4Step.hh"

#include "construction.hh"
#include "event.hh"

#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

#include "G4Proton.hh"
#include "G4Electron.hh"
#include "G4OpticalPhoton.hh"


#include <algorithm>

class MySteppingAction : public G4UserSteppingAction
{
public:
	MySteppingAction(MyEventAction* eventAction);
	~MySteppingAction();

	virtual void UserSteppingAction(const G4Step*);

private:
	MyEventAction *fEventAction;


	G4int recordedEvent;

	G4int recorded[50000] = { 0 };

	G4ThreeVector ParentMom;

};


#endif
