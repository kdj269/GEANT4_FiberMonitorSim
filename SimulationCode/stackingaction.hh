#ifndef STACKINGACTION_HH
#define STACKINGACTION_HH


#include "G4UserStackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"

#include "globals.hh"

#include "run.hh"
#include "G4ios.hh"
#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"


class MyStackingAction : public G4UserStackingAction
{
	public:
	MyStackingAction();
	~MyStackingAction();

	virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
	virtual void NewStage();
	virtual void PrepareNewEvent();


	private:
	G4int fCerenkovCounter;

};

#endif
