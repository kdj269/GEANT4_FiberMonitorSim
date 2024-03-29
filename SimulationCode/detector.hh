#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4Proton.hh"
#include "G4Electron.hh"


class MySensitiveDetector : public G4VSensitiveDetector
{
public:
	MySensitiveDetector(G4String);
	~MySensitiveDetector();


private:
	virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);

};

class ScreenSensitiveDetector : public G4VSensitiveDetector
{
public:
	ScreenSensitiveDetector(G4String);
	~ScreenSensitiveDetector();


private:
	virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);

};


#endif
