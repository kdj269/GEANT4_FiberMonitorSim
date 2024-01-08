#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4SystemOfUnits.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4GenericMessenger.hh"
#include "G4AnalysisManager.hh"


#include "detector.hh"

#include "G4RunManager.hh"
#include "run.hh"

#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4GeometryManager.hh"
#include "G4VisManager.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  MyDetectorConstruction();
  ~MyDetectorConstruction();

  G4VPhysicalVolume *Construct();



//private:
	//-----Boxes,tubs and volumes-----//
	G4Box* solidFILL_square, * solidCLAD1_square, * solidCLAD2_square,*fiberDetector, *screenDetector, *solidWorld, *solidMIRROR, *solidBOX, * solidBOXcenter, * solidBOXfront, * solidBOXback, * solidBOXfrontside, * solidBOXbackside, * solidBOXbottomside, * solidBOXrightside, * solidBOXleftside, * solidBOXfrontsideMirror, * solidBOXbacksideMirror, * solidBOXbottomsideMirror, * solidBOXrightsideMirror, * solidBOXleftsideMirror, *solidChannelMirror;
	G4Tubs *solidFILL, *solidCLAD1, *solidCLAD2;
	G4LogicalVolume *logicfiberDetector, *logicscreenDetector, *logicWorld, *logicFILL, *logicCLAD1, *logicCLAD2, *logicMIRROR, * logicBOX, * logicFiberMother, * logicBOXcenter, * logicBOXfront, * logicBOXback, * logicBOXfrontside, * logicBOXbackside, * logicBOXbottomside, * logicBOXrightside, * logicBOXleftside, * logicBOXfrontsideMirror, * logicBOXbacksideMirror, * logicBOXbottomsideMirror, * logicBOXrightsideMirror, * logicBOXleftsideMirror, * logicChannelMirror;
	G4VPhysicalVolume *physfiberDetector, *physscreenDetector,*physWorld, *physFILL,*physCLAD1, *physCLAD2, *physMIRROR, * physBOX, * physFiberMother, * physBOXcenter, * physBOXfront, * physBOXback, * physBOXfrontside, * physBOXbackside, * physBOXbottomside, * physBOXrightside, * physBOXleftside, * physBOXfrontsideMirror, * physBOXbacksideMirror, * physBOXbottomsideMirror, * physBOXrightsideMirror, * physBOXleftsideMirror, * physChannelMirror;

	//-----Surfaces-----//

	G4OpticalSurface *mirrorOS;
	G4OpticalSurface *mirrorOS_2;
	G4OpticalSurface *mirrorOS_3;
	G4OpticalSurface* mirrorOS_4;
	G4OpticalSurface *quartzOS;
	G4OpticalSurface *hardpolymerOS;
	G4OpticalSurface *tefzelOS;
	
	//-----Tables-----//

	G4MaterialPropertiesTable* MYLAR_properties;
	G4MaterialPropertiesTable* QUARTZ_properties;
	G4MaterialPropertiesTable* WATER_properties;
	G4MaterialPropertiesTable* EJ309_properties;
	G4MaterialPropertiesTable* WORLD_properties;
	G4MaterialPropertiesTable* HARDPOLYMER_properties;
	G4MaterialPropertiesTable* TEFZEL_properties;
	G4MaterialPropertiesTable* CF4_properties;
	G4MaterialPropertiesTable* WbLS_properties;
	G4MaterialPropertiesTable* DIAMOND_properties;

	//-----Materials-----//
	G4Material *worldMat, *WATER, *QUARTZ, *MYLAR, *EJ309, *HARDPOLYMER, *TEFZEL, *CF4, *WbLS, * DIAMOND;

	//-----Non messenger variables for geometry-----//
	G4double polish_hand;
	G4double polish_core;
	G4double polish_trap;
	G4double polish_coating;
	G4double polish_semiperm;
	G4double worldsize;

	//-----Messenger variables for geometry-----//
	G4GenericMessenger *Messenger_geom;
	G4int Geom_select;
	G4double Nfiber;
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
	

	//-----Other variables for geometry-----//
	G4double FiberZLocation;
	



virtual void ConstructSDandField();

  void DefineMaterial();
};


#endif


