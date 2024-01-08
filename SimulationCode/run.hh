#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"

class MyRunAction : public G4UserRunAction
{

public:
  MyRunAction();
  ~MyRunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);
  G4AnalysisManager *man;
  G4int fileNum;

};

class FiberRun : public G4Run
{
public:
    FiberRun();
    ~FiberRun();

    G4int GetGeomSelect()
    {
        return Geom_select;
    };
    void SetGeomSelect(G4int n)
    {
        Geom_select = n;
    };
    G4int GetNfiber()
    {
        return Nfiber;
    };
    G4double GetBaseZLocation()
    {
        return BaseZLocation;
    };
    G4double GetlenFiber()
    {
        return lenFiber;
    };
    G4double GetR0()
    {
        return R0;
    };
    G4double GetR1()
    {
        return R1;
    };
    G4double GetR2()
    {
        return R2;
    };
    G4double Getrot()
    {
        return rot;
    };

    void LoadValues()
    {
        /*
        G4int thr = G4Threading::G4GetThreadId();
        std::stringstream thrID;
        thrID << thr;
        static std::ifstream geomFile("currentGeometry" + thrID.str() + ".csv");

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
    };


private:
    G4int Geom_select;
    G4int Nfiber;
    G4double BaseZLocation;
    G4double lenFiber;
    G4double R0;
    G4double R1;
    G4double R2;
    G4double rot;
};


#endif

