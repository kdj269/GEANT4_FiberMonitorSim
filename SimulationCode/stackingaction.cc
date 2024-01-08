#include "stackingaction.hh"


MyStackingAction::MyStackingAction()
{
	fCerenkovCounter = 0;
}


MyStackingAction::~MyStackingAction() {}


G4ClassificationOfNewTrack MyStackingAction::ClassifyNewTrack( const G4Track *aTrack)
{	
	G4cout << "New Track!" << G4endl;
	G4ClassificationOfNewTrack classification = fUrgent;
	if(aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
	{  // particle is optical photon
		G4cout << "Optical photon generated!" << G4endl;
		if(aTrack->GetCreatorProcess()->GetProcessName() == "Cerenkov")
		{	
        		++fCerenkovCounter;
		}
	}
	
  return classification;
}


void MyStackingAction::NewStage()
{
	//G4cout << "Number of Cerenkov photons produced in this event : " << fCerenkovCounter << G4endl;

	//G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

	//G4AnalysisManager *man = G4AnalysisManager::Instance();

	//man->FillNtupleIColumn(4, 0, evt);
	//man->FillNtupleDColumn(4, 1, fCerenkovCounter);
	//man->AddNtupleRow(4);

}

void MyStackingAction::PrepareNewEvent()
{
	fCerenkovCounter      = 0;
}
