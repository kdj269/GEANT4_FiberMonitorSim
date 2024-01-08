#include "run.hh"



MyRunAction::MyRunAction()
{
	man = G4AnalysisManager::Instance();

	man->SetNtupleMerging(true);

	man->CreateNtuple("Hits","Hits");
	man->CreateNtupleIColumn("Event");
	man->CreateNtupleDColumn("wlen");
	man->CreateNtupleDColumn("DetectorID");
	man->CreateNtupleDColumn("TrackID");
	man->CreateNtupleDColumn("Type");
	man->FinishNtuple(0);


	man->CreateNtuple("HitsScreen","HitsScreen");
	man->CreateNtupleIColumn("Event");
	man->CreateNtupleDColumn("xpos");
	man->CreateNtupleDColumn("ypos");
	man->CreateNtupleDColumn("zpos");
	man->CreateNtupleDColumn("xmom");
	man->CreateNtupleDColumn("ymom");
	man->CreateNtupleDColumn("zmom");
	man->FinishNtuple(1);


	man->CreateNtuple("InitialState","InitialState");
	man->CreateNtupleIColumn("Event");
	man->CreateNtupleDColumn("xpos");
	man->CreateNtupleDColumn("ypos");
	man->CreateNtupleDColumn("zpos");
	man->CreateNtupleDColumn("xmom");
	man->CreateNtupleDColumn("ymom");
	man->CreateNtupleDColumn("zmom");

	man->CreateNtupleDColumn("Geom_select");
	man->CreateNtupleDColumn("Nfiber");
	man->CreateNtupleDColumn("R0");
	man->CreateNtupleDColumn("R1");
	man->CreateNtupleDColumn("R2");
	man->CreateNtupleDColumn("rot");
	man->CreateNtupleDColumn("pressure");
	man->CreateNtupleDColumn("bundling");
	man->CreateNtupleDColumn("R3");
	man->CreateNtupleDColumn("R4");
	man->CreateNtupleDColumn("R5");
	man->FinishNtuple(2);


	man->CreateNtuple("EscapePhoton","EscapePhoton");
	man->CreateNtupleIColumn("Event");
	man->CreateNtupleDColumn("wlen");
	man->CreateNtupleDColumn("TrackID");
	man->CreateNtupleDColumn("Type");
	man->FinishNtuple(3);

	//man->CreateNtuple("PhotonCounter","PhotonCounter");
	//man->CreateNtupleIColumn("Event");
	//man->CreateNtupleDColumn("PhotonCounter");
	//man->FinishNtuple(4);

	man->CreateNtuple("TotalPhoton", "TotalPhoton");
	man->CreateNtupleIColumn("Event");
	man->CreateNtupleDColumn("wlen");
	man->CreateNtupleDColumn("TrackID");
	man->CreateNtupleDColumn("Type");
	man->CreateNtupleDColumn("CreationAngle");
	man->FinishNtuple(4);

	static G4Mutex stuffMutex = G4MUTEX_INITIALIZER;
	G4AutoLock al(&stuffMutex);

	static std::ifstream fileFile("fileSettings.csv");
	fileFile >> fileNum;
	fileFile.close();

	
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
	
	//G4int runID = run->GetRunID();
	//std::stringstream strRunID;
	//strRunID << runID;
	//man->OpenFile("rootFiles/output"+strRunID.str()+".root");

	std::stringstream strRunID;
	strRunID << fileNum;
	man->OpenFile("C:/Users/rglarsen/Documents/simulations/FiberArrayComparisonSimulation/SimulationCode/build_Win/Release/rootFiles/output" + strRunID.str() + ".root");
	
	//G4cout << filename.substr(1, filename.length() - 2) << G4endl;
	//man->OpenFile("rootFiles/file.root");
	//man->OpenFile("rootFiles/" + filename.substr(1, filename.length() - 2) + ".root");

}

void MyRunAction::EndOfRunAction(const G4Run* run)
{
	man->Write();
	man->CloseFile();


}



FiberRun::FiberRun():G4Run()
{
	Geom_select=0;
	Nfiber = 0;
	BaseZLocation = 0;
	lenFiber = 0;
	R0 = 0;
	R1 = 0;
	R2 = 0;
	rot = 0;
	/*
	static std::ifstream geomFile("currentGeometry.csv");

	geomFile >> Geom_select;
	geomFile >> Nfiber;
	geomFile >> BaseZLocation;
	geomFile >> lenFiber;
	geomFile >> R0;
	geomFile >> R1;
	geomFile >> R2;
	geomFile >> rot;

	geomFile.close();
	*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FiberRun::~FiberRun() {}
