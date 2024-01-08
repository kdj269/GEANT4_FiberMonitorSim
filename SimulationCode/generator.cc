#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
  fParticleGun = new G4ParticleGun(1);

}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
  delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{    


	//Here we should load in the correct values.

	G4int thr = G4Threading::G4GetThreadId();
	std::stringstream thrID;
	thrID << thr;
	std::ifstream geomFile("currentGeometry" + thrID.str() + ".csv");

	geomFile >> Geom_select;
	geomFile >> Nfiber;
	geomFile >> BaseZLocation;
	geomFile >> lenFiber;
	geomFile >> R0;
	geomFile >> R1;
	geomFile >> R2;
	geomFile >> rot;
	geomFile >> pressure;
	geomFile >> bundling;
	geomFile >> R3;
	geomFile >> R4;
	geomFile >> R5;

	geomFile.close();

	std::ifstream particleFile("currentParticle" + thrID.str() + ".csv");

	particleFile >> ParticleSelect;
	particleFile >> Beam_x;
	particleFile >> Beam_y;
	particleFile >> sigmaX;
	particleFile >> sigmaY;
	particleFile >> E_mean;
	particleFile >> sigmaE;

	particleFile.close();

	//----Creating a particle source----//
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* particle;
	if (ParticleSelect == 0) //pick proton
	{
		G4String particleName = "proton";
		particle = particleTable->FindParticle("proton");
	}
	if (ParticleSelect == 1) //pick electron
	{
		G4String particleName = "e-";
		particle = particleTable->FindParticle("e-");
	}

	//basic parameters
	E_mean = E_mean * GeV;
	sigmaE = sigmaE * GeV;//1.0*GeV;
	sigmaX = sigmaX * cm;//1.0*cm;
	sigmaY = sigmaY * cm;//1.0*cm;

	G4ThreeVector mon(0, 0, 1);

	//Creates random gaussian energy spectrum
	G4double W = G4UniformRand();
	G4double Z = G4UniformRand();
	G4double energy = E_mean + (sigmaE * sqrt((-2) * log(W)) * cos(6.2831 * Z));

	//Create random starting point
	G4double X1 = G4UniformRand();
	G4double X2 = G4UniformRand();
	G4double Y1 = G4UniformRand();
	G4double Y2 = G4UniformRand();

	G4double x0 = Beam_x * cm + (sigmaX * sqrt((-2) * log(X1)) * cos(6.2831 * X2));
	G4double y0 = Beam_y * cm + (sigmaY * sqrt((-2) * log(Y1)) * cos(6.2831 * Y2));

	G4ThreeVector pos(x0, y0, 0.);

	
	fParticleGun->SetParticleDefinition(particle);
	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mon);
	fParticleGun->SetParticleMomentum(energy);

	
	//write to root files:
	G4AnalysisManager* man = G4AnalysisManager::Instance();


	man->FillNtupleIColumn(2, 0, simnumber);
	man->FillNtupleDColumn(2, 1, x0);
	man->FillNtupleDColumn(2, 2, y0);
	man->FillNtupleDColumn(2, 3, 0);
	man->FillNtupleDColumn(2, 4, 0);
	man->FillNtupleDColumn(2, 5, 0);
	man->FillNtupleDColumn(2, 6, energy);

	man->FillNtupleDColumn(2, 7, Geom_select);
	man->FillNtupleDColumn(2, 8, Nfiber);
	man->FillNtupleDColumn(2, 9, R0);
	man->FillNtupleDColumn(2, 10, R1);
	man->FillNtupleDColumn(2, 11, R2);
	man->FillNtupleDColumn(2, 12, rot);
	man->FillNtupleDColumn(2, 13, pressure);
	man->FillNtupleDColumn(2, 14, bundling);
	man->FillNtupleDColumn(2, 15, R3);
	man->FillNtupleDColumn(2, 16, R4);
	man->FillNtupleDColumn(2, 17, R5);
	man->AddNtupleRow(2);
	


	//G4cout<< "Saved start of "<< simnumber << " with angle of " << rot << G4endl;
	
	fParticleGun->GeneratePrimaryVertex(anEvent);
	//Count number of runs in termial:
	simnumber += 1;
	G4cout<< "Ran Simulation "<< simnumber << G4endl;

}
