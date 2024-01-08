#include "detector.hh"
#include <stdio.h>
#include <vector>
MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}


MySensitiveDetector::~MySensitiveDetector()
{}


G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
  	G4Track *track = aStep->GetTrack();

	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();	
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
	G4String volume_name = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName(); 
   
	const G4VTouchable *touchable = aStep -> GetPreStepPoint() -> GetTouchable();

	G4ThreeVector posPhoton = preStepPoint->GetPosition();
	G4ThreeVector monPhoton = preStepPoint->GetMomentum();
	G4double enPhoton = preStepPoint->GetKineticEnergy();

	G4int type = -1;

	if (aStep->GetTrack()->GetCreatorProcess()->GetProcessName() == "Cerenkov")
	{
		type = 0;
	}
	if (aStep->GetTrack()->GetCreatorProcess()->GetProcessName() == "Scintillation")
	{
		type = 1;
	}



	G4double wlen = (1.239841939*eV/monPhoton.mag());//*1E+03;

	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	
	G4int detectorNumber = touchable -> GetCopyNumber();

	G4int Tracknumber = track->GetTrackID();
	if (aStep->GetTrack()->GetParentID() == 1) //only note down, if the photon was generated by the primary. 
	{
		G4AnalysisManager* man = G4AnalysisManager::Instance();
		man->FillNtupleIColumn(0, 0, evt);
		man->FillNtupleDColumn(0, 1, wlen);
		man->FillNtupleDColumn(0, 2, detectorNumber);
		man->FillNtupleDColumn(0, 3, Tracknumber);
		man->FillNtupleDColumn(0, 4, type);
		man->AddNtupleRow(0);

	}
	
	//G4cout<< "P_photon "<< sqrt(monPhoton[0]*monPhoton[0] + monPhoton[1]*monPhoton[1] + monPhoton[2]*monPhoton[2]) << G4endl;
	//G4cout<< "E_photon "<< enPhoton << G4endl;

	track->SetTrackStatus(fStopAndKill); //kill track

	return true;
}

//For screen to 'observe' protons after interacting with monitor. 

ScreenSensitiveDetector::ScreenSensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}


ScreenSensitiveDetector::~ScreenSensitiveDetector()
{}


G4bool ScreenSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
	if (aStep->GetTrack()->GetParentID()==0)// GetParticleDefinition()==G4Proton::Proton() || aStep->GetTrack()->GetParticleDefinition() == G4Electron::Electron()) // only react to primary
	{
	  	G4Track *track = aStep->GetTrack();

		G4StepPoint *preStepPoint = aStep->GetPreStepPoint();	
		G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

		G4ThreeVector posProton = preStepPoint->GetPosition();
		G4ThreeVector momProton = preStepPoint->GetMomentum();
		G4double enProton = preStepPoint->GetKineticEnergy();


		G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
		
		

		G4AnalysisManager *man = G4AnalysisManager::Instance();
		

		track->SetTrackStatus(fStopAndKill); //kill track

		man->FillNtupleIColumn(1, 0, evt);
		man->FillNtupleDColumn(1, 1, posProton[0]);
		man->FillNtupleDColumn(1, 2, posProton[1]);
		man->FillNtupleDColumn(1, 3, posProton[2]);
		man->FillNtupleDColumn(1, 4, momProton[0]);
		man->FillNtupleDColumn(1, 5, momProton[1]);
		man->FillNtupleDColumn(1, 6, momProton[2]);
		man->AddNtupleRow(1);
		/*
		G4cout<< "X "<< posProton[0] << G4endl;
		G4cout<< "Y "<< posProton[1] << G4endl;
		G4cout<< "Z "<< posProton[2] << G4endl;

		G4cout<< "PX_f "<< momProton[0] << G4endl;
		G4cout<< "PY_f "<< momProton[1] << G4endl;
		G4cout<< "PZ_f "<< momProton[2] << G4endl;

		G4cout<< "E_f_geant "<< enProton << G4endl;
		G4cout<< "E_f_computed "<< momProton.mag() << G4endl;
		*/
		return true;
	}
}

