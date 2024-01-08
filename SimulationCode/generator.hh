#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4GenericMessenger.hh"

//#include "G4RootAnalysisManager.hh"
#include "G4AnalysisManager.hh"

#include "G4RunManager.hh"
#include "run.hh"
//#include "construction.hh"




class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
  MyPrimaryGenerator();
  ~MyPrimaryGenerator();

  virtual void GeneratePrimaries(G4Event*);
  G4int simnumber = 0;

  
	G4int Geom_select;
	G4int Nfiber;
	G4double BaseZLocation;
	G4double lenFiber;
	G4double R0;
	G4double R1;
	G4double R2;
	G4double rot;
	G4double pressure;
	G4int bundling;
	G4double R3;
	G4double R4;
	G4double R5;



private:
  G4ParticleGun *fParticleGun;

  G4int ParticleSelect;


  G4double Beam_x;
  G4double Beam_y;

  G4double E_mean;
  G4double sigmaE;
  G4double sigmaX;
  G4double sigmaY;
};

#endif








