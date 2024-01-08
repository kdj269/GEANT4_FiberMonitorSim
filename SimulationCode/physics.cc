#include "physics.hh"
#include "FTFP_BERT.hh"


MyPhysicsList::MyPhysicsList()
{
  
	
  	RegisterPhysics (new G4EmStandardPhysics());
  	//RegisterPhysics( new G4StoppingPhysics() );
	RegisterPhysics( new G4OpticalPhysics() );

	

}

MyPhysicsList::~MyPhysicsList()
{}
