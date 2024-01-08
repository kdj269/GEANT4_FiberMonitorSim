#include "construction.hh"


MyDetectorConstruction::MyDetectorConstruction()
{
	//-----Non messenger variables-----//
	worldsize = 50*cm;
	polish_hand  = 0.90;
	polish_core  = 0.98;
	polish_trap = 0.98;
	polish_coating = 0.8;
	polish_semiperm = 0.99;
	

	
	//-----Other variables for geometry-----//
	FiberZLocation = BaseZLocation*cm;
	
	//-----Run method to define materials-----//
	DefineMaterial();

}


MyDetectorConstruction::~MyDetectorConstruction()
{
}

void MyDetectorConstruction::DefineMaterial()
{	
	//-----REading the geometry we are building-----//

	static G4Mutex stuffMutex = G4MUTEX_INITIALIZER;
	G4AutoLock al(&stuffMutex);

	static std::ifstream geomFile("currentGeometry.csv");

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

	geomFile.close();


	FiberZLocation = BaseZLocation * cm;

	//-----G4NistManager-----//	
	G4NistManager *nist = G4NistManager::Instance();

	//-----photon properties-----//
	const G4int nEntries = 22;
	G4double PhotonEnergy[nEntries] = {0.248*eV, 1.127*eV, 1.181*eV, 1.24*eV, 1.305*eV, 1.378*eV, 1.459*eV, 1.55*eV, 1.653*eV, 1.771*eV, 1.907*eV, 2.066*eV, 2.254*eV, 2.48*eV, 2.755*eV, 3.1*eV, 3.542*eV, 4.133*eV, 4.959*eV, 6.199*eV, 8.265*eV, 12.398*eV};

	//-----Elements-----//
	G4Element* H = new G4Element("H", "H", 1., 1.01  * g/mole);
	G4Element* C = new G4Element("C", "C", 6., 12.01 * g/mole);
	G4Element* F = new G4Element("F", "F", 9., 18.99 * g/mole);
  
	//-----World material-----//
	worldMat = nist->FindOrBuildMaterial("G4_Galactic");
	G4double rindexWorld[nEntries] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
	WORLD_properties = new G4MaterialPropertiesTable();
	WORLD_properties->AddProperty("RINDEX", PhotonEnergy, rindexWorld, 5);
	worldMat->SetMaterialPropertiesTable(WORLD_properties);

	//-----CF4-----//

	G4double bolzman = 1.380649e-23 * joule / kelvin;
	
	G4double mass = 88.0043 * 1.66053906660e-27 * kg;
	G4double temperature = 300 * kelvin;
	G4double pressureCF4 = pressure * bar;

	//G4double density = 3.72 * mg / cm3;
	G4double densityCF4 = pressureCF4 * mass/(bolzman* temperature);

	CF4_properties = new G4MaterialPropertiesTable();
	CF4 = new G4Material("CF4_gas", densityCF4, 2, kStateGas, temperature, pressureCF4);
	CF4->AddElement(C, 1);
	CF4->AddElement(F, 4);	
	
	G4double RefractiveIndex_CF4[nEntries] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
	G4double Absorption_CF4[nEntries] = { 10000.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m, 10.0 * m, 1.0 * m, 1 * m, 1 * m, 10.0 * m, 10000.0 * m, 10000.0 * m, 10.0 * m, 10.0 * m, 1.0 * m, 1.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m};
	G4double Rayleigh_CF4[nEntries] = { 7. * m, 7. * m, 7. * m, 7. * m, 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m };
	//G4double Scintil_CF4[nEntries] = { 0, 0, 0, 0.075, 0.075, 0.04, 0.01, 0, 0, 0.05, 0.2, 0.25, 0.2, 0.05, 0.03, 0.02, 0, 0, 0, 0, 0, 0};
	G4double Scintil_CF4[nEntries] = { 0, 0, 0, 0, 0, 0, 0.02, 0.03, 0.05, 0.2, 0.25, 0.2, 0.05, 0, 0, 0.01, 0.04, 0.075, 0.075, 0, 0, 0};

	CF4_properties->AddConstProperty("RESOLUTIONSCALE", 5.);
	CF4_properties->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 3.5 * ns);
	CF4_properties->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex_CF4, nEntries);
	CF4_properties->AddProperty("ABSLENGTH", PhotonEnergy, Absorption_CF4, nEntries);
	CF4_properties->AddProperty("SCINTILLATIONCOMPONENT1", PhotonEnergy, Scintil_CF4, nEntries);
	CF4_properties->AddConstProperty("SCINTILLATIONYIELD", 2000 / MeV);
	CF4_properties->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
	CF4_properties->AddProperty("RAYLEIGH", PhotonEnergy, Rayleigh_CF4, nEntries);

	CF4->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

	CF4->SetMaterialPropertiesTable(CF4_properties);

	//-----EJ309-----//


	G4double density = 0.95 * g / cm3;

	EJ309_properties = new G4MaterialPropertiesTable();
	EJ309 = new G4Material("EJ309", density, 2, kStateLiquid);
	EJ309->AddElement(C, 5);
	EJ309->AddElement(H, 4);

	G4double RefractiveIndex_EJ309[nEntries] = { 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57};
	G4double Absorption_EJ309[nEntries] = { 10000.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m, 10.0 * m, 1.0 * m, 0.1 * m, 10.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m, 10000.0 * m };
	G4double Rayleigh_EJ309[nEntries] = { 7. * m, 7. * m, 7. * m, 7. * m, 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m };
	//G4double Scintil_EJ309[nEntries] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.15, 0.6, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	G4double Scintil_EJ309[nEntries] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.25, 0.6, 0.15, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	//G4double Scintil_EJ309[nEntries] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0, 1, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

	EJ309_properties->AddConstProperty("RESOLUTIONSCALE", 5.);
	EJ309_properties->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 3.5 * ns);
	EJ309_properties->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex_EJ309, nEntries);
	EJ309_properties->AddProperty("ABSLENGTH", PhotonEnergy, Absorption_EJ309, nEntries);
	EJ309_properties->AddProperty("SCINTILLATIONCOMPONENT1", PhotonEnergy, Scintil_EJ309, nEntries);
	EJ309_properties->AddConstProperty("SCINTILLATIONYIELD", 12300 / MeV);
	EJ309_properties->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
	EJ309_properties->AddProperty("RAYLEIGH", PhotonEnergy, Rayleigh_EJ309, nEntries);

	EJ309->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

	EJ309->SetMaterialPropertiesTable(EJ309_properties);

	//-----WATER-----//
	WATER = nist->FindOrBuildMaterial("G4_WATER");  
	WATER_properties = new G4MaterialPropertiesTable();
	G4double RefractiveIndex_WATER[nEntries] = { 1.325, 1.31, 1.325, 1.327, 1.327, 1.328, 1.329, 1.329, 1.33, 1.331, 1.331, 1.332, 1.333, 1.335, 1.337, 1.339, 1.343, 1.349, 1.362, 1.396, 1.396, 1.396};
	G4double Absorption_WATER[nEntries] = { 0.0001*m, 0.0001*m, 0.002*m, 0.071*m, 0.090*m, 0.11*m, 0.2*m, 0.83*m, 0.869*m, 0.90*m, 1.0*m, 1.25*m, 2.5*m, 10.0*m, 20.0*m, 50.0*m, 25.0*m, 10.0*m, 3.33*m, 0.1*m, 1e-06*m, 1e-08*m };
	G4double Rayleigh_WATER[nEntries] = { 7.*m, 7.*m, 7.*m, 7.*m, 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m };

	WATER_properties -> AddConstProperty("RESOLUTIONSCALE",    5.);
	WATER_properties -> AddProperty("RINDEX", PhotonEnergy, RefractiveIndex_WATER, nEntries);
	WATER_properties -> AddProperty("ABSLENGTH",     PhotonEnergy, Absorption_WATER,      nEntries);
	WATER_properties -> AddProperty("RAYLEIGH", PhotonEnergy, Rayleigh_WATER, nEntries);

	WATER -> GetIonisation() -> SetBirksConstant(0.126*mm/MeV);

	WATER->SetMaterialPropertiesTable(WATER_properties);

	//-----WbLS-----//
	G4double densityWbLS = 0.99 * g / cm3;

	WbLS_properties = new G4MaterialPropertiesTable();
	WbLS = new G4Material("WbLS", densityWbLS, 2, kStateLiquid);
	WbLS->AddMaterial(EJ309, 0.07);
	WbLS->AddMaterial(WATER, 0.93);

	//G4double RefractiveIndex_WbLS = *RefractiveIndex_WATER;
	//G4double Absorption_WbLS = *Absorption_WATER;
	G4double Rayleigh_WbLS[nEntries] = { 7. * m, 7. * m, 7. * m, 7. * m, 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m };
	//G4double Scintil_WbLS[nEntries] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.15, 0.6, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	G4double Scintil_WbLS[nEntries] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.25, 0.6, 0.15, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	//G4double Scintil_WbLS[nEntries] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0, 1, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

	WbLS_properties->AddConstProperty("RESOLUTIONSCALE", 5.);
	WbLS_properties->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 3.5 * ns);
	WbLS_properties->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex_WATER, nEntries);
	WbLS_properties->AddProperty("ABSLENGTH", PhotonEnergy, Absorption_WATER, nEntries);
	WbLS_properties->AddProperty("SCINTILLATIONCOMPONENT1", PhotonEnergy, Scintil_WbLS, nEntries);
	WbLS_properties->AddConstProperty("SCINTILLATIONYIELD", 0.1*12300 / MeV);
	WbLS_properties->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
	WbLS_properties->AddProperty("RAYLEIGH", PhotonEnergy, Rayleigh_WbLS, nEntries);

	WbLS->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

	WbLS->SetMaterialPropertiesTable(WbLS_properties);



	//-----QUARTZ-----//
	QUARTZ = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

	QUARTZ_properties = new G4MaterialPropertiesTable();
	G4double RefractiveIndex_QUARTZ[nEntries] = { 1.3405, 1.4492, 1.4498, 1.4504, 1.4511, 1.4518, 1.4525, 1.4533, 1.4542, 1.4553, 1.4565, 1.458, 1.4599, 1.4623, 1.4656, 1.4701, 1.4769, 1.4878, 1.5074, 1.5384, 1.56, 1.58}; 
	//G4double RefractiveIndex_QUARTZ[nEntries] = { 1.4, 1.4, 1.4, 1.4, 1.4, 1.4, 1.4, 1.4, 1.4, 1.4, 1.4, 1.4, 1.4, 1.4, 1.4, 1.4, 1.4, 1.4, 1.4, 1.4, 1.4, 1.4 };
	G4double Absorption_QUARTZ[nEntries] = { 1e-05 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 0.09 * m, 0.008 * m, 1e-05 * m, 1e-05 * m};
	G4double Rayleigh_QUARTZ[nEntries] = { 7.*m, 7.*m, 7.*m, 7.*m, 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m , 7.*m  };

	QUARTZ_properties -> AddConstProperty("RESOLUTIONSCALE",    5.);
	QUARTZ_properties -> AddProperty("RINDEX", PhotonEnergy, RefractiveIndex_QUARTZ, nEntries);
	QUARTZ_properties -> AddProperty("ABSLENGTH", PhotonEnergy, Absorption_QUARTZ, nEntries);
	QUARTZ_properties -> AddProperty("RAYLEIGH", PhotonEnergy, Rayleigh_QUARTZ, nEntries);
	QUARTZ -> GetIonisation() -> SetBirksConstant(0.126*mm/MeV);

	QUARTZ -> SetMaterialPropertiesTable(QUARTZ_properties);

	//-----DIAMOND-----//
	G4double densityDIAMOND = 3.52 * g / cm3;

	DIAMOND_properties = new G4MaterialPropertiesTable();
	DIAMOND = new G4Material("WbLS", densityDIAMOND, 1, kStateSolid);

	DIAMOND->AddElement(C, 1);

	G4double RefractiveIndex_DIAMOND[nEntries] = { 2.417, 2.417, 2.417, 2.417, 02.417, 2.417, 2.417, 2.417, 2.417, 2.417, 2.417, 2.417, 2.417, 2.417, 2.417, 2.417, 2.417, 2.417, 2.417, 2.417, 2.417, 2.417 };
	G4double Absorption_DIAMOND[nEntries] = { 1 * m, 100.0 * m, 100.0 * m, 100.0 * m, 100.0 * m, 100.0 * m, 100.0 * m, 100.0 * m, 100.0 * m, 100.0 * m, 100.0 * m, 100.0 * m, 100.0 * m, 100.0 * m, 100.0 * m, 100.0 * m, 100.0 * m, 100.0 * m, 100 * m, 1e-06 * m, 1e-06 * m, 1e-06 * m };
	G4double Rayleigh_DIAMOND[nEntries] = { 7. * m, 7. * m, 7. * m, 7. * m, 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m , 7. * m };

	DIAMOND_properties->AddConstProperty("RESOLUTIONSCALE", 5.);
	DIAMOND_properties->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex_DIAMOND, nEntries);
	DIAMOND_properties->AddProperty("ABSLENGTH", PhotonEnergy, Absorption_DIAMOND, nEntries);
	DIAMOND_properties->AddProperty("RAYLEIGH", PhotonEnergy, Rayleigh_DIAMOND, nEntries);
	DIAMOND->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

	DIAMOND->SetMaterialPropertiesTable(DIAMOND_properties);

	//-----HARDPOLYMER-----//
	G4int polyeth;
	G4int nH_eth;
	G4int nC_eth;
	G4int nF_eth;
	polyeth = 1;
	nH_eth = 4*polyeth;
	nC_eth = 4*polyeth;
	nF_eth = 4*polyeth;
	G4double densityHARDPOLYMER = 1.7*g/cm3;

	HARDPOLYMER = new G4Material("Hard Polymer", densityHARDPOLYMER,3);
	HARDPOLYMER->AddElement(H,nH_eth);
	HARDPOLYMER->AddElement(C,nC_eth);
	HARDPOLYMER->AddElement(F,nF_eth);
	G4double Absorption_HARDPOLYMER[nEntries] = {1e-6 * m, 1e-6 * m,1e-6 * m, 1e-6 * m, 1e-6 * m, 1e-6 * m, 1e-6 * m, 1e-6 * m, 1e-6 * m, 1e-6 * m, 1e-6 * m, 1e-6 * m, 1e-6 * m, 1e-6 * m, 1e-6 * m, 1e-6 * m, 1e-6 * m, 1e-6 * m, 1e-6 * m, 1e-6 * m, 1e-6 * m, 1e-6 * m };
	G4double RefractiveIndex_HARDPOLYMER[nEntries] = { 1.37, 1.37, 1.37, 1.37, 1.37, 1.37, 1.37, 1.37, 1.37, 1.37, 1.37, 1.37, 1.37, 1.37, 1.37, 1.37, 1.37, 1.37, 1.37, 1.37, 1.37, 1.37 };

	HARDPOLYMER_properties = new G4MaterialPropertiesTable();

	HARDPOLYMER_properties -> AddProperty("RINDEX",PhotonEnergy,RefractiveIndex_HARDPOLYMER,nEntries);
	HARDPOLYMER_properties->AddProperty("ABSLENGTH", PhotonEnergy, Absorption_HARDPOLYMER, nEntries);
	HARDPOLYMER -> SetMaterialPropertiesTable(HARDPOLYMER_properties);

	//-----TEFZEL-----//
	polyeth = 1;
	nH_eth = 4*polyeth;
	nC_eth = 4*polyeth;
	nF_eth = 4*polyeth;
	G4double densityTEFZEL = 1.7*g/cm3;

	TEFZEL = new G4Material("Tefzel", densityTEFZEL,3);
	TEFZEL->AddElement(H,nH_eth);
	TEFZEL->AddElement(C,nC_eth);
	TEFZEL->AddElement(F,nF_eth);

	G4double RefractiveIndex_TEFZEL[nEntries] = { 1.4,1.4,1.4,1.4,1.4,1.4,1.4,1.4, 1.4,1.4,1.4,1.4,1.4,1.4, 1.4,1.4,1.4,1.4,1.4,1.4,1.4,1.4};
	G4double Absorption_TEFZEL[nEntries] = { 2373e-6 * m, 2373e-6 * m,2373e-6 * m, 2373e-6 * m, 2373e-6 * m, 2373e-6 * m, 2373e-6 * m, 2373e-6 * m, 2373e-6 * m, 2373e-6 * m, 2373e-6 * m, 2373e-6 * m, 2373e-6 * m, 2373e-6 * m, 2373e-6 * m, 2373e-6 * m, 2373e-6 * m, 2373e-6 * m, 2373e-6 * m, 2373e-6 * m, 2373e-6 * m, 2373e-6 * m };


	TEFZEL_properties = new G4MaterialPropertiesTable();

	TEFZEL_properties -> AddProperty("RINDEX",PhotonEnergy,RefractiveIndex_TEFZEL,nEntries);
	TEFZEL_properties->AddProperty("ABSLENGTH", PhotonEnergy, Absorption_TEFZEL, nEntries);
	TEFZEL->SetMaterialPropertiesTable(TEFZEL_properties);

	//-----MYLAR-----//
	MYLAR = nist->FindOrBuildMaterial("G4_MYLAR");
	MYLAR_properties = new G4MaterialPropertiesTable();
	G4double RefractiveIndex_MYLAR[nEntries] = { 1.655, 1.655, 1.655, 1.655, 1.655 , 1.655 , 1.655 , 1.655 , 1.655 , 1.655 , 1.655 , 1.655 , 1.655 , 1.655 , 1.655 , 1.655 , 1.655 , 1.655 , 1.655 , 1.655 , 1.655 , 1.655 };
	G4double Absorption_MYLAR[nEntries] = { 1e-2 * m,1e-2 * m,1e-2 * m, 1e-2 * m, 1e-2 * m, 1e-2 * m, 1e-2 * m, 1e-2 * m, 1e-2 * m, 1e-2 * m, 1e-2 * m, 1e-2 * m, 1e-2 * m, 1e-2 * m, 1e-2 * m, 1e-2 * m, 1e-2 * m, 1e-2 * m, 1e-2 * m, 1e-2 * m, 1e-2 * m, 1e-2 * m };

	MYLAR_properties->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex_MYLAR, nEntries);
	MYLAR_properties->AddProperty("ABSLENGTH", PhotonEnergy, Absorption_MYLAR, nEntries);

	MYLAR->SetMaterialPropertiesTable(MYLAR_properties);

	//-----Optical surfaces-----//
	mirrorOS = new G4OpticalSurface("MirrorOS");
	mirrorOS->SetModel(unified);
	mirrorOS->SetType(dielectric_metal);
	mirrorOS->SetFinish(polished);
	mirrorOS->SetPolish(polish_hand);
	mirrorOS->SetMaterialPropertiesTable(MYLAR_properties);

	mirrorOS_2 = new G4OpticalSurface("MirrorOS_2");
	mirrorOS_2->SetModel(unified);
	mirrorOS_2->SetType(dielectric_metal);
	mirrorOS_2->SetFinish(polished);
	mirrorOS_2->SetPolish(polish_trap);
	mirrorOS_2->SetMaterialPropertiesTable(QUARTZ_properties);

	mirrorOS_3 = new G4OpticalSurface("MirrorOS_3");
	mirrorOS_3->SetModel(unified);
	mirrorOS_3->SetType(dielectric_metal);
	mirrorOS_3->SetFinish(polished);
	mirrorOS_3->SetPolish(polish_coating);
	mirrorOS_3->SetMaterialPropertiesTable(MYLAR_properties);

	mirrorOS_4 = new G4OpticalSurface("MirrorOS_4");
	mirrorOS_4->SetModel(unified);
	mirrorOS_4->SetType(dielectric_metal);
	mirrorOS_4->SetFinish(polished);
	mirrorOS_4->SetPolish(polish_semiperm);
	mirrorOS_4->SetMaterialPropertiesTable(MYLAR_properties);


	quartzOS = new G4OpticalSurface("quartzOS");
	quartzOS->SetModel(unified);
	quartzOS->SetType(dielectric_dielectric);
	quartzOS->SetFinish(polished);
	quartzOS->SetPolish(polish_core);
	quartzOS->SetMaterialPropertiesTable(QUARTZ_properties);


	hardpolymerOS = new G4OpticalSurface("hardpolymerOS");
	hardpolymerOS->SetModel(unified);
	hardpolymerOS->SetType(dielectric_dielectric);
	hardpolymerOS->SetFinish(polished);
	hardpolymerOS->SetPolish(polish_hand);
	hardpolymerOS->SetMaterialPropertiesTable(HARDPOLYMER_properties);


	tefzelOS = new G4OpticalSurface("tefzelOS");
	tefzelOS->SetModel(unified);
	tefzelOS->SetType(dielectric_dielectric);
	tefzelOS->SetFinish(polished);
	tefzelOS->SetPolish(polish_hand);
	tefzelOS->SetMaterialPropertiesTable(TEFZEL_properties);


}

void placeFibres(G4double delta_x, G4double delta_z, G4double DetectorR,
				G4bool FILL, G4bool CLAD1, G4bool CLAD2, G4bool MirrorChannel, G4bool CheckOverlap, G4bool ConcentricPlacement,
				G4RotationMatrix * Rotation,
				G4double Nfiber,
				G4VPhysicalVolume* physFILL, G4VPhysicalVolume* physCLAD1, G4VPhysicalVolume* physCLAD2, G4VPhysicalVolume* physChannelMirror,
				G4LogicalVolume* logicFILL, G4LogicalVolume* logicCLAD1, G4LogicalVolume* logicCLAD2, G4LogicalVolume* logicChannelMirror, G4LogicalVolume* logicFiberMother)
{

	if (ConcentricPlacement) // we need to only place the outermost volume, since the others are daugters
	{
		G4double zplacement = delta_z;
		for (G4int i = 0; i < Nfiber; i++)
		{
			G4double xplacement = (i - (Nfiber / 2) + 0.5) * 2 * DetectorR + delta_x;
			if (CLAD2) { physCLAD2 = new G4PVPlacement(Rotation, G4ThreeVector(xplacement, 0, zplacement), logicCLAD2, "physCLAD2", logicFiberMother, false, i, CheckOverlap); }
			else if (CLAD1) { physCLAD1 = new G4PVPlacement(Rotation, G4ThreeVector(xplacement, 0, zplacement), logicCLAD1, "physCLAD1", logicFiberMother, false, i, CheckOverlap); }
			else if (FILL) { physFILL = new G4PVPlacement(Rotation, G4ThreeVector(xplacement, 0, zplacement), logicFILL, "physFILL", logicFiberMother, false, i, CheckOverlap); }
			else {}
		}
	}
	else // normal operation
	{
		G4double zplacement = delta_z;
		for (G4int i = 0; i < Nfiber; i++)
		{
			G4double xplacement = (i - (Nfiber / 2) + 0.5) * 2 * DetectorR + delta_x;
			if (FILL) { physFILL = new G4PVPlacement(Rotation, G4ThreeVector(xplacement, 0, zplacement), logicFILL, "physFILL", logicFiberMother, false, i, CheckOverlap); }
			if (CLAD1) { physCLAD1 = new G4PVPlacement(Rotation, G4ThreeVector(xplacement, 0, zplacement), logicCLAD1, "physCLAD1", logicFiberMother, false, i, CheckOverlap); }
			if (CLAD2) { physCLAD2 = new G4PVPlacement(Rotation, G4ThreeVector(xplacement, 0, zplacement), logicCLAD2, "physCLAD2", logicFiberMother, false, i, CheckOverlap); }

		}
	}

	if (MirrorChannel)
	{
		if (delta_z == 0)//only central plane
		{
			for (G4int i = 0; i < Nfiber; i++)
			{
				physChannelMirror = new G4PVPlacement(Rotation, G4ThreeVector((i - (Nfiber / 2) + 0.5) * 2 * DetectorR + delta_x, 0, delta_z), logicChannelMirror, "physChannelMirror", logicFiberMother, false, i, CheckOverlap);
			}
		}
	}
}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
	//-----REading the geometry we are building-----//

	static G4Mutex stuffMutex = G4MUTEX_INITIALIZER;
	G4AutoLock al(&stuffMutex);

	static std::ifstream geomFile("currentGeometry.csv");

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


	FiberZLocation = BaseZLocation * cm;

	//-----creating the world volume-----//

	solidWorld = new G4Box("solidWorld", worldsize, worldsize, worldsize);
	logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

	logicFiberMother = logicWorld;
	physFiberMother = physWorld;



	//---Variables for selecting material etc.----//
	//default values
	G4bool FILL = true;
	G4bool CLAD1 = true;
	G4bool CLAD2 = true;
	G4bool Mirrorcoat = false;
	G4bool BOX = false;
	G4bool BOXSides = false;
	G4bool BOXMirror = false;
	G4bool MirrorCap = true;
	G4bool Fibres = true;
	G4bool MirrorChannel = false;
	G4bool SquareArray = false;
	G4bool HexArray = true;
	G4bool HexSquareArray = false;
	G4bool Squarefiber = false;
	G4bool ConcentricPlacement = false;


	G4bool Surfaces = true;


	G4bool CheckOverlap = false;

	G4Material* FILL_mat = QUARTZ;
	G4Material* CLAD1_mat = QUARTZ;
	G4Material* CLAD2_mat = QUARTZ;
	G4Material* BOX_mat = WATER;
	G4Material* BOX_side_mat = QUARTZ;
	G4Material* Mirror_mat = MYLAR;

	G4OpticalSurface* FILLOS = quartzOS;
	G4OpticalSurface* CLAD1OS = quartzOS;
	G4OpticalSurface* CLAD2OS = quartzOS;

	//-----creating the rotation-----//
	G4RotationMatrix* Rotation = new G4RotationMatrix();
	Rotation->rotateX(90 * deg);
	Rotation->rotateY(0 * deg);
	Rotation->rotateZ(0 * deg);

	//global rotation of detector, now moved to mother-volume box
	//Rotation->rotateX(rot * deg);

	//Rotation for detector volumens
	G4RotationMatrix* RotationDetectors = new G4RotationMatrix();
	RotationDetectors->rotateX(rot * deg);

	//values for placing volumens correctly
	G4double DetectorR = R2 * mm * (2 * bundling + 1);
	G4double FiberSpacing = 2 * R2;
	G4double MirrorThickness = 0.1*mm;

	G4double BoxPadding = 2 * (R3 + R5); //to make room in box for front and back material

	if (2 * (R4 + R5) > BoxPadding)
	{
		BoxPadding = 2*(R4 + R5);
	}

	//-----Option 0, glass capillary with water inside-----//
	
	if (Geom_select == 0)
	{
		FILL = true;
		CLAD1 = true;
		CLAD2 = false;

		FILL_mat = WATER;
		CLAD1_mat = QUARTZ;

		FILLOS = quartzOS;
		CLAD1OS = quartzOS;

	}

	//-----Option 1, glass rods-----//

	if (Geom_select == 1)
	{
		FILL = true;
		CLAD1 = false;
		CLAD2 = false;


		FILL_mat = QUARTZ;

		FILLOS = quartzOS;
	}

	//-----Option 2, optical fibre-----//

	if (Geom_select == 2)
	{
		FILL = true;
		CLAD1 = true;
		CLAD2 = true;


		FILL_mat = QUARTZ;
		CLAD1_mat = HARDPOLYMER;
		CLAD2_mat = TEFZEL;

		FILLOS = quartzOS;
		CLAD1OS = hardpolymerOS;
		CLAD2OS = tefzelOS;
	}
	//-----Option 3, CF4 in HCF-----//

	if (Geom_select == 3)
	{
		FILL = true;
		CLAD1 = true;
		CLAD2 = true;
		Mirrorcoat = true;

		FILL_mat = CF4;
		CLAD1_mat = QUARTZ;
		CLAD2_mat = TEFZEL;

		FILLOS = quartzOS;
		CLAD1OS = quartzOS;
		CLAD2OS = tefzelOS;
	}

	//-----Option 4, glass capillary with EJ309 inside-----//

	if (Geom_select == 4)
	{
		FILL = true;
		CLAD1 = true;
		CLAD2 = false;
		Mirrorcoat = false;

		FILL_mat = EJ309;
		CLAD1_mat = QUARTZ;

		FILLOS = quartzOS;
		CLAD1OS = quartzOS;
	}

	//-----Option 5, H2O in HCF-----//

	if (Geom_select == 5)
	{
		FILL = true;
		CLAD1 = true;
		CLAD2 = true;
		Mirrorcoat = true;

		FILL_mat = WATER;
		CLAD1_mat = QUARTZ;
		CLAD2_mat = TEFZEL;

		FILLOS = quartzOS;
		CLAD1OS = quartzOS;
		CLAD2OS = tefzelOS;
	}

	//-----Option 6, EJ309 in HCF-----//

	if (Geom_select == 6)
	{
		FILL = true;
		CLAD1 = true;
		CLAD2 = true;
		Mirrorcoat = false;

		FILL_mat = EJ309;
		CLAD1_mat = QUARTZ;
		CLAD2_mat = TEFZEL;

		FILLOS = quartzOS;
		CLAD1OS = quartzOS;
		CLAD2OS = tefzelOS;
	}

	//-----Option 7, Rods in water-----//

	if (Geom_select == 7)
	{
		FILL = true;
		CLAD1 = false;
		CLAD2 = false;
		Mirrorcoat = false;

		FILL_mat = QUARTZ;

		FILLOS = quartzOS;

		BOX = true;
		BOXSides = true;
		MirrorCap = true;
		BOXMirror = true;

		BOX_mat = WATER;
		BOX_side_mat = QUARTZ;


	}
	//-----Option 8, Rods in WbLS-----//

	if (Geom_select == 8)
	{
		FILL = true;
		CLAD1 = false;
		CLAD2 = false;
		Mirrorcoat = false;

		FILL_mat = QUARTZ;

		FILLOS = quartzOS;

		BOX = true;
		BOXSides = true;
		MirrorCap = true;
		BOXMirror = true;

		BOX_mat = WbLS;
		BOX_side_mat = QUARTZ;
	}
	//-----Option 9, just WbLS-----//

	if (Geom_select == 9)
	{
		FILL = false;
		CLAD1 = false;
		CLAD2 = false;
		Mirrorcoat = false;

		FILL_mat = QUARTZ;

		FILLOS = quartzOS;

		BOX = true;
		BOXSides = true;
		MirrorCap = true;
		BOXMirror = true;

		BOX_mat = WbLS;
		BOX_side_mat = QUARTZ;
	}
	//-----Option 10, water with mirror 'channels'-----//

	if (Geom_select == 10)
	{
		FILL = false;
		CLAD1 = false;
		CLAD2 = false;
		Mirrorcoat = false;
		MirrorChannel = true;

		BOX = true;
		BOXSides = true;
		MirrorCap = true;
		BOXMirror = true;

		BOX_mat = WATER;
		BOX_side_mat = QUARTZ;
	}

	//-----Option 11, WbLS with mirror 'channels'-----//

	if (Geom_select == 11)
	{
		FILL = false;
		CLAD1 = false;
		CLAD2 = false;
		Mirrorcoat = false;
		MirrorChannel = true;

		BOX = true;
		BOXSides = true;
		MirrorCap = true;
		BOXMirror = true;

		BOX_mat = WbLS;
		BOX_side_mat = QUARTZ;
	}
	//-----Option 12, just water-----//

	if (Geom_select == 12)
	{
		FILL = false;
		CLAD1 = false;
		CLAD2 = false;
		Mirrorcoat = false;

		BOX = true;
		BOXSides = true;
		MirrorCap = true;
		BOXMirror = true;

		BOX_mat = WATER;
		BOX_side_mat = QUARTZ;
	}
	//-----Option 13, just EJ309-----//

	if (Geom_select == 13)
	{
		FILL = false;
		CLAD1 = false;
		CLAD2 = false;
		Mirrorcoat = false;

		BOX = true;
		BOXSides = true;
		MirrorCap = true;
		BOXMirror = true;

		BOX_mat = EJ309;
		BOX_side_mat = QUARTZ;
	}

	//-----Option 14, diamond rods-----// (just for fun)

	if (Geom_select == 14)
	{
		FILL = true;
		CLAD1 = false;
		CLAD2 = false;


		FILL_mat = DIAMOND;

		FILLOS = quartzOS;
	}

	//-----Option 15, diamond rods in square bundle-----// (just for fun)

	if (Geom_select == 15)
	{
		FILL = true;
		CLAD1 = false;
		CLAD2 = false;

		SquareArray = true;
		HexArray = false;

		FILL_mat = DIAMOND;

		FILLOS = quartzOS;
	}

	//-----Option 16, diamond rods in square hex bundle-----// (just for fun)

	if (Geom_select == 16)
	{
		FILL = true;
		CLAD1 = false;
		CLAD2 = false;

		SquareArray = false;
		HexArray = false;
		HexSquareArray = true;

		FILL_mat = DIAMOND;

		FILLOS = quartzOS;
	}

	//-----Option 17, square quartz fiber rods in square bundle-----// 

	if (Geom_select == 17)
	{
		FILL = true;
		CLAD1 = false;
		CLAD2 = false;

		SquareArray = true;
		HexArray = false;
		HexSquareArray = false;
		Squarefiber = true;
		ConcentricPlacement = true;

		FILL_mat = QUARTZ;

		FILLOS = quartzOS;


	}
	//-----Option 18, optical fiber in square bundle-----// 

	if (Geom_select == 18)
	{
		FILL = true;
		CLAD1 = true;
		CLAD2 = true;


		FILL_mat = QUARTZ;
		CLAD1_mat = HARDPOLYMER;
		CLAD2_mat = TEFZEL;

		FILLOS = quartzOS;
		CLAD1OS = hardpolymerOS;
		CLAD2OS = tefzelOS;

		SquareArray = true;
		HexArray = false;
		HexSquareArray = false;
		Squarefiber = false;
		ConcentricPlacement = true;
	}
	//-----Option 19, square quartz capillary with EJ309 in square bundle-----// 

	if (Geom_select == 19)
	{
		FILL = true;
		CLAD1 = false;
		CLAD2 = false;


		FILL_mat = EJ309;

		FILLOS = quartzOS;

		BOX = true;
		BOXSides = false;
		MirrorCap = true;
		BOXMirror = false;

		BOX_mat = QUARTZ;
		BOX_side_mat = QUARTZ;

		SquareArray = true;
		HexArray = false;
		HexSquareArray = false;
		Squarefiber = true;
		ConcentricPlacement = true;
	}
	//-----Option 20, square quartz capillary with EJ309 in square bundle with mirror on outside-----// 

	if (Geom_select == 20)
	{
		FILL = true;
		CLAD1 = false;
		CLAD2 = false;


		FILL_mat = EJ309;

		FILLOS = quartzOS;

		BOX = true;
		BOXSides = true;
		MirrorCap = true;
		BOXMirror = true;

		BOX_mat = QUARTZ;
		BOX_side_mat = QUARTZ;

		SquareArray = true;
		HexArray = false;
		HexSquareArray = false;
		Squarefiber = true;
		ConcentricPlacement = true;
	}

	if (Squarefiber) { ConcentricPlacement = true; } // in case I make a mistake earlier, since it is not currently possible to make boxes with more than one material without the concenctric scheme
	if (Squarefiber) { HexSquareArray = false; HexArray = false; SquareArray = true; } //to avoid overlap, since the efficient packing is only possible for round fibres

	//non physical volumes
	if (FILL)
	{
		if (Squarefiber) 
		{
			solidFILL_square = new G4Box("solidFILL", R0 * mm, R0 * mm, lenFiber * cm);
			logicFILL = new G4LogicalVolume(solidFILL_square, FILL_mat, "logicFILL");
		}
		else 
		{ 
			solidFILL = new G4Tubs("solidFILL", 0. * mm, R0 * mm, lenFiber * cm, 0, 360 * deg); 
			logicFILL = new G4LogicalVolume(solidFILL, FILL_mat, "logicFILL");
		}	
	}
	if (CLAD1)
	{
		if (Squarefiber)
		{
			solidCLAD1_square = new G4Box("solidCLAD1", R1 * mm, R1 * mm, lenFiber * cm);
			logicCLAD1 = new G4LogicalVolume(solidCLAD1_square, CLAD1_mat, "logicFILL");
		}
		else
		{
			G4double innerR = R0 * mm;
			if (ConcentricPlacement) { innerR = 0; }
			solidCLAD1 = new G4Tubs("solidCLAD1", innerR, R1 * mm, lenFiber * cm, 0, 360 * deg);
			logicCLAD1 = new G4LogicalVolume(solidCLAD1, CLAD1_mat, "logicCLAD1");
		}

	}
	if (CLAD2)
	{ 
		if (Squarefiber)
		{
			solidCLAD2_square = new G4Box("solidCLAD2", R2 * mm, R2 * mm, lenFiber * cm);
			logicCLAD2 = new G4LogicalVolume(solidCLAD2_square, CLAD2_mat, "logicFILL");
		}
		else
		{
			G4double innerR = R1 * mm;
			if (ConcentricPlacement) { innerR = 0; }
			solidCLAD2 = new G4Tubs("solidCLAD2", innerR, R2 * mm, lenFiber * cm, 0, 360 * deg);
			logicCLAD2 = new G4LogicalVolume(solidCLAD2, CLAD2_mat, "logicCLAD2");
		}
	}
	if (MirrorChannel)
	{
		solidChannelMirror = new G4Box("solidChannelMirror", R0 * mm, DetectorR+R3+R4, lenFiber * cm);
		logicChannelMirror = new G4LogicalVolume(solidChannelMirror, MYLAR, "logicChannelMirror");
	}

	//---Start by placing objects that we always need----//

	// the box containing everything, makes rotation much easier (can be a box of same material as world)
	solidBOX = new G4Box("solidBOX", DetectorR * (Nfiber) + 2 * R5 * mm + 2* MirrorThickness, lenFiber * cm + 2 * DetectorR + 2 * R5 * mm + 2 * MirrorThickness, DetectorR + BoxPadding * mm + 2 * MirrorThickness);
	logicBOX = new G4LogicalVolume(solidBOX, worldMat, "logicBOX");
	physBOX = new G4PVPlacement(RotationDetectors, G4ThreeVector(0., 0, FiberZLocation), logicBOX, "physBOX", logicWorld, false, 0, true);
	
	//now, depending on if we want a material around the fibres, we set the mother-volume:
	if (BOX)
	{
		//center
		solidBOXcenter = new G4Box("solidBOXcenter", DetectorR * (Nfiber), lenFiber * cm + 2 * DetectorR, DetectorR);
		logicBOXcenter = new G4LogicalVolume(solidBOXcenter, BOX_mat, "logicBOXcenter");
		physBOXcenter = new G4PVPlacement(0, G4ThreeVector(0., 0, 0), logicBOXcenter, "physBOXcenter", logicBOX, false, 0, true);
		//front
		solidBOXfront = new G4Box("solidBOXfront", DetectorR * (Nfiber), lenFiber * cm + 2 * DetectorR, R3*mm);
		logicBOXfront = new G4LogicalVolume(solidBOXfront, BOX_mat, "logicBOXfront");
		physBOXfront = new G4PVPlacement(0, G4ThreeVector(0., 0, ( - DetectorR - R3 * mm)), logicBOXfront, "physBOXfront", logicBOX, false, 0, true);
		//back
		solidBOXback = new G4Box("solidBOXback", DetectorR * (Nfiber), lenFiber * cm + 2 * DetectorR, R4 * mm);
		logicBOXback = new G4LogicalVolume(solidBOXback, BOX_mat, "logicBOXback");
		physBOXback = new G4PVPlacement(0, G4ThreeVector(0., 0, DetectorR + R4 * mm), logicBOXback, "physBOXback", logicBOX, false, 0, true);



		logicFiberMother = logicBOXcenter;
		physFiberMother = physBOXcenter;
		// the sides of the box 

	}
	else
	{
		logicFiberMother = logicBOX;
		physFiberMother = physBOX;
	}
	
	if (BOXSides)
	{
		//front
		solidBOXfrontside = new G4Box("solidBOXfrontside", DetectorR * (Nfiber)+ 2 * R5*mm, lenFiber * cm + 2 * DetectorR + 2 * R5 * mm, R5 * mm);
		logicBOXfrontside = new G4LogicalVolume(solidBOXfrontside, BOX_side_mat, "logicBOXfrontside");
		physBOXfrontside = new G4PVPlacement(0, G4ThreeVector(0., 0, -DetectorR - 2 * R3 * mm - R5 * mm), logicBOXfrontside, "physBOXfrontside", logicBOX, false, 0, true);
		//back
		solidBOXbackside = new G4Box("solidBOXbackside", DetectorR * (Nfiber) + 2 * R5 * mm, lenFiber * cm + 2 * DetectorR + 2*R5 * mm, R5 * mm);
		logicBOXbackside = new G4LogicalVolume(solidBOXbackside, BOX_side_mat, "logicBOXbackside");
		physBOXbackside = new G4PVPlacement(0, G4ThreeVector(0., 0, DetectorR + 2 * R4 * mm + R5 * mm), logicBOXbackside, "physBOXbackside", logicBOX, false, 0, true);
		//bottom
		solidBOXbottomside = new G4Box("solidBOXbottomside", DetectorR * (Nfiber) + 2 * R5 * mm, R5 * mm, (R3 * mm +R4 * mm +DetectorR) );
		logicBOXbottomside = new G4LogicalVolume(solidBOXbottomside, BOX_side_mat, "logicBOXbottomside");
		physBOXbottomside = new G4PVPlacement(0, G4ThreeVector(0., -lenFiber * cm - 2 * DetectorR - R5 * mm, (R4-R3)*mm), logicBOXbottomside, "physBOXbottomside", logicBOX, false, 0, true);
		//right
		solidBOXrightside = new G4Box("solidBOXrightside", R5 * mm, lenFiber * cm + 2 * DetectorR, (R3 * mm + R4 * mm + DetectorR));
		logicBOXrightside = new G4LogicalVolume(solidBOXrightside, BOX_side_mat, "logicBOXrightside");
		physBOXrightside = new G4PVPlacement(0, G4ThreeVector(-(DetectorR * (Nfiber) + 1 * R5 * mm), 0., (R4 - R3) * mm), logicBOXrightside, "physBOXrightside", logicBOX, false, 0, true);
		//left
		solidBOXleftside = new G4Box("solidBOXleftside", R5 * mm, lenFiber * cm + 2 * DetectorR, (R3 * mm + R4 * mm + DetectorR));
		logicBOXleftside = new G4LogicalVolume(solidBOXleftside, BOX_side_mat, "logicBOXleftside");
		physBOXleftside = new G4PVPlacement(0, G4ThreeVector((DetectorR * (Nfiber) + 1 * R5 * mm), 0., (R4 - R3) * mm), logicBOXleftside, "physBOXleftside", logicBOX, false, 0, true);
	}
	if (BOXMirror)
	{
		//front
		solidBOXfrontsideMirror = new G4Box("solidBOXfrontsideMirror", DetectorR * (Nfiber) + 2 * R5 * mm, lenFiber * cm + 2 * DetectorR + 2 * R5 * mm, MirrorThickness);
		logicBOXfrontsideMirror = new G4LogicalVolume(solidBOXfrontsideMirror, Mirror_mat, "logicBOXfrontsideMirror");
		physBOXfrontsideMirror = new G4PVPlacement(0, G4ThreeVector(0., 0, -DetectorR - 2 * R3 * mm - 2*R5 * mm - MirrorThickness), logicBOXfrontsideMirror, "physBOXfrontsideMirror", logicBOX, false, 0, true);
		//back
		solidBOXbacksideMirror = new G4Box("solidBOXbacksideMirror", DetectorR * (Nfiber) + 2 * R5 * mm, lenFiber * cm + 2 * DetectorR + 2 * R5 * mm, MirrorThickness);
		logicBOXbacksideMirror = new G4LogicalVolume(solidBOXbacksideMirror, Mirror_mat, "logicBOXbacksideMirror");
		physBOXbacksideMirror = new G4PVPlacement(0, G4ThreeVector(0., 0, DetectorR + 2 * R4 * mm + 2 * R5 * mm + MirrorThickness), logicBOXbacksideMirror, "physBOXbacksideMirror", logicBOX, false, 0, true);
		//bottom
		solidBOXbottomsideMirror = new G4Box("solidBOXbottomsideMirror", DetectorR * (Nfiber) + 2 * R5 * mm, MirrorThickness, (2 * R5 * mm + 2 * MirrorThickness + R3 * mm + R4 * mm + DetectorR));
		logicBOXbottomsideMirror = new G4LogicalVolume(solidBOXbottomsideMirror, Mirror_mat, "logicBOXbottomsideMirror");
		physBOXbottomsideMirror = new G4PVPlacement(0, G4ThreeVector(0., -lenFiber * cm - 2 * DetectorR - 2*R5 * mm - MirrorThickness, (R4 - R3) * mm), logicBOXbottomsideMirror, "physBOXbottomsideMirror", logicBOX, false, 0, true);
		//right
		solidBOXrightsideMirror = new G4Box("solidBOXrightsideMirror", MirrorThickness, lenFiber * cm + 2 * DetectorR + 2*MirrorThickness + 2* R5 * mm, (2*R5 * mm +2*MirrorThickness + R3 * mm + R4 * mm + DetectorR));
		logicBOXrightsideMirror = new G4LogicalVolume(solidBOXrightsideMirror, Mirror_mat, "logicBOXrightsideMirror");
		physBOXrightsideMirror = new G4PVPlacement(0, G4ThreeVector(-(DetectorR * (Nfiber) + 2 * R5 * mm + MirrorThickness), 0., (R4 - R3) * mm), logicBOXrightsideMirror, "physBOXrightsideMirror", logicBOX, false, 0, true);
		//left
		solidBOXleftsideMirror = new G4Box("solidBOXleftsideMirror", MirrorThickness, lenFiber * cm + 2 * DetectorR + 2 * MirrorThickness + 2 * R5 * mm, (2 * R5 * mm + 2 * MirrorThickness + R3 * mm + R4 * mm + DetectorR));
		logicBOXleftsideMirror = new G4LogicalVolume(solidBOXleftsideMirror, Mirror_mat, "logicBOXleftsideMirror");
		physBOXleftsideMirror = new G4PVPlacement(0, G4ThreeVector((DetectorR * (Nfiber) + 2 * R5 * mm + MirrorThickness), 0., (R4 - R3) * mm), logicBOXleftsideMirror, "physBOXleftsideMirror", logicBOX, false, 0, true);
		
		//making them mirrors:
		new G4LogicalSkinSurface("logicBOXfrontsideMirrorLS", logicBOXfrontsideMirror, mirrorOS_3);
		new G4LogicalSkinSurface("logicBOXbacksideMirrorLS", logicBOXbacksideMirror, mirrorOS_3);
		new G4LogicalSkinSurface("logicBOXbottomsideMirrorLS", logicBOXbottomsideMirror, mirrorOS_3);
		new G4LogicalSkinSurface("logicBOXrightsideMirrorLS", logicBOXrightsideMirror, mirrorOS_3);
		new G4LogicalSkinSurface("logicBOXleftsideMirrorLS", logicBOXleftsideMirror, mirrorOS_3);
	}


	//---constructing senstive volumes----//
	fiberDetector = new G4Box("fiberDetector", DetectorR, DetectorR, DetectorR);
	logicfiberDetector = new G4LogicalVolume(fiberDetector, worldMat, "logicfiberDetector");
	for (G4int i = 0; i < Nfiber; i++) {
		//physfiberDetector = new G4PVPlacement(RotationDetectors, G4ThreeVector((i - Nfiber / 2) * 2 * DetectorR, (lenFiber * cm + DetectorR) * cos(rot * deg), FiberZLocation - (lenFiber * cm + DetectorR) * sin(rot * deg)), logicfiberDetector, "physfiberDetector", logicFiberMother, false, i, true);
		physfiberDetector = new G4PVPlacement(0, G4ThreeVector((i - (Nfiber / 2)+0.5) * 2 * DetectorR, (lenFiber * cm + DetectorR), 0), logicfiberDetector, "physfiberDetector", logicFiberMother, false, i, true);
	}

	screenDetector = new G4Box("screenDetector", worldsize, worldsize, worldsize * 0.01);
	logicscreenDetector = new G4LogicalVolume(screenDetector, worldMat, "logicscreenDetector");
	physscreenDetector = new G4PVPlacement(0, G4ThreeVector(0, 0, worldsize * 0.99), logicscreenDetector, "physscreenDetector", logicWorld, false, 0, true);


	//---putting mirror----//
	if (MirrorCap)
	{
		solidMIRROR = new G4Box("solidMIRROR", (Nfiber) * DetectorR, 0.1 * mm, DetectorR);
		logicMIRROR = new G4LogicalVolume(solidMIRROR, MYLAR, "logicMIRROR");
		//physMIRROR = new G4PVPlacement(RotationDetectors, G4ThreeVector(0., -(lenFiber * cm + 0.1 * mm) * cos(rot * deg), FiberZLocation + (lenFiber * cm + 0.1 * mm) * sin(rot * deg)), logicMIRROR, "physMIRROR", logicFiberMother, false, 0, true);
		physMIRROR = new G4PVPlacement(0, G4ThreeVector(0., -(lenFiber * cm + 0.1 * mm), 0), logicMIRROR, "physMIRROR", logicFiberMother, false, 0, true);

	}
	

	new G4LogicalSkinSurface("MirrorLS", logicMIRROR, mirrorOS);


	//---putting the fibres, according to a few different cases---//

	if (ConcentricPlacement) // in this mode we just need to place the daughter volumes once
	{
		if (CLAD1 && FILL) { physFILL = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicFILL, "physFILL", logicCLAD1, false, 0, CheckOverlap); }
		else if (CLAD2 && FILL) { physFILL = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicFILL, "physFILL", logicCLAD2, false, 0, CheckOverlap); }

		if (CLAD2 && CLAD1) { physCLAD1 = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicCLAD1, "physCLAD1", logicCLAD2, false, 0, CheckOverlap); }		
		
	}


	G4double delta_x = 0;
	G4double delta_z = 0;

	if (SquareArray) //make square array of fibres, the simplest design
	{

		G4int NPerSide = 1 + 2 * bundling;

		for (G4int i = 0; i < NPerSide; i++) //loop over columns
		{
			for (G4int j = 0; j < NPerSide; j++) //loop over rows
			{
				delta_x = -bundling * 2 * R2 + i * 2 * R2;
				delta_z = -bundling * 2 * R2 + j * 2 * R2;

				//function that loops over the number of detectors and places a fibre in each according to supplied offset
				placeFibres(delta_x, delta_z, DetectorR,
					FILL, CLAD1, CLAD2, MirrorChannel, CheckOverlap, ConcentricPlacement,
					Rotation,
					Nfiber,
					physFILL, physCLAD1, physCLAD2, physChannelMirror,
					logicFILL, logicCLAD1, logicCLAD2, logicChannelMirror, logicFiberMother);

			}
		}

	}
	else if (HexSquareArray) // make square array packing as in a hex (good for round fibres) 
	{
		G4int NPerSide = 1 + 2 * bundling;

		//strategy will be to check if we are in an odd or an even row 

		for (G4int i = 0; i < NPerSide; i++) //loop over columns
		{
			for (G4int j = 0; j < NPerSide; j++) //loop over rows
			{
				G4bool flag = false; 
				if (j % 2 == 0) //even case, spacing in x same as square
				{
					delta_x = -bundling * 2 * R2 + i * 2 * R2;
					delta_z = -bundling * sqrt(3) * R2 + j * sqrt(3) * R2;

				}
				else // odd case, where we place one fiber fewer!
				{
					flag = true;
					delta_x = -bundling * 2 * R2 + R2 + i * 2 * R2;
					delta_z = -bundling * sqrt(3) * R2 + j * sqrt(3) * R2;

				}

				if ((flag==false) || (i < NPerSide-1))
				{
				//function that loops over the number of detectors and places a fibre in each according to supplied offset
				placeFibres(delta_x, delta_z, DetectorR,
					FILL, CLAD1, CLAD2, MirrorChannel, CheckOverlap, ConcentricPlacement,
					Rotation,
					Nfiber,
					physFILL, physCLAD1, physCLAD2, physChannelMirror,
					logicFILL, logicCLAD1, logicCLAD2, logicChannelMirror, logicFiberMother);
				}
			}
		}
	}
	else if (HexArray) // make hexagonal bundles of fibres (closest to each undle being round)
	{
		for (G4int j = 0; j < bundling +1  ; j++)//loop over layers
		{	
			G4int sides = 6;
			if (j == 0) //central fiber
			{
				sides = 1; // to avoid placing central fiber six times
			}

			for (G4int k = 0; k < sides; k++) //loop over six sides
			{

				G4int NPerSide = j;
				if (j == 0) //central fiber
				{
					NPerSide = 1; // so we place first fiber
				}
				for (G4int l = 0; l < NPerSide; l++) //loop over how many in each of the six sides
				{
					if (j == 0) //central fiber
						{ 
						delta_x = 0; //no displacement
						delta_z = 0;
						
					} else { // all other cases, displacement depends on the side we are building
						switch (k)
						{
						case 0:
							delta_x = -j * FiberSpacing + FiberSpacing * cos(60 * deg) * l;
							delta_z = FiberSpacing * sin(60 * deg) * l;
							break;
						case 1:
							delta_x = -FiberSpacing * cos(60 * deg) * j + FiberSpacing*l;
							delta_z = FiberSpacing * sin(60 * deg) * j;
							break;
						case 2:
							delta_x = FiberSpacing * cos(60 * deg) * j + FiberSpacing * cos(60 * deg) * l;
							delta_z = FiberSpacing * sin(60 * deg) * j - FiberSpacing * sin(60 * deg) * l;
							break;
						case 3:
							delta_x = j * FiberSpacing - FiberSpacing * cos(60 * deg) * l;
							delta_z = -FiberSpacing * sin(60 * deg) * l;
							break;
						case 4:
							delta_x = FiberSpacing * cos(60 * deg) * j - FiberSpacing * l;
							delta_z = -FiberSpacing * sin(60 * deg) * j;
							break;
						case 5:
							delta_x = -FiberSpacing * cos(60 * deg) * j - FiberSpacing * cos(60 * deg) * l;
							delta_z = -FiberSpacing * sin(60 * deg) * j + FiberSpacing * sin(60 * deg) * l;
							break;

						}
					}
					//function that loops over the number of detectors and places a fibre in each according to supplied offset
					placeFibres(delta_x, delta_z, DetectorR,
						FILL, CLAD1, CLAD2, MirrorChannel, CheckOverlap, ConcentricPlacement,
						Rotation,
						Nfiber,
						physFILL, physCLAD1, physCLAD2, physChannelMirror,
						logicFILL, logicCLAD1, logicCLAD2, logicChannelMirror, logicFiberMother);

				
				}
			}
		}

	}
	//-----Surfaces-----//
	if (Surfaces)
	{
		if (FILL == true && CLAD1 == false)
		{
			new G4LogicalBorderSurface("FILLIN", physFiberMother, physFILL, FILLOS);
			new G4LogicalBorderSurface("FILLOUT", physFILL, physFiberMother, FILLOS);
		}
		if (FILL == true && CLAD1 == true)
		{
			new G4LogicalBorderSurface("FILLIN", physCLAD1, physFILL, FILLOS);
			new G4LogicalBorderSurface("FILLOUT", physFILL, physCLAD1, FILLOS);

		}
		if (CLAD1 == true && CLAD2 == false)
		{
			new G4LogicalBorderSurface("CLAD1OUT", physCLAD1, physFiberMother, CLAD1OS);
			new G4LogicalBorderSurface("CLAD1IN", physFiberMother, physCLAD1, CLAD1OS);
		}
		if (CLAD1 == true && CLAD2 == true)
		{
			new G4LogicalBorderSurface("CLAD1OUT", physCLAD1, physCLAD2, CLAD1OS);
			new G4LogicalBorderSurface("CLAD1IN", physCLAD2, physCLAD1, CLAD1OS);
		}
		if (CLAD2)
		{
			new G4LogicalBorderSurface("CLAD2OUT", physCLAD2, physFiberMother, CLAD2OS);
			new G4LogicalBorderSurface("CLAD2IN", physFiberMother, physCLAD2, CLAD2OS);
		}
		if (Mirrorcoat)
		{
			new G4LogicalSkinSurface("MirrorCoat", logicCLAD1, mirrorOS_2);
		}
		if (MirrorChannel)
		{
			new G4LogicalSkinSurface("MirrorChannelOS", logicChannelMirror, mirrorOS_4);
		}
	}
	

G4RunManager::GetRunManager()->DefineWorldVolume(physWorld);

return physWorld;

}


void MyDetectorConstruction::ConstructSDandField()
{
  MySensitiveDetector *sensDet = new MySensitiveDetector("fiberDetector");
    
  logicfiberDetector->SetSensitiveDetector(sensDet);

  ScreenSensitiveDetector *sensDetScreen = new ScreenSensitiveDetector("screenDetector");
    
  logicscreenDetector->SetSensitiveDetector(sensDetScreen);
  
}


