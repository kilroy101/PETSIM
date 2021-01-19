//
// Created by Dung Phan on 3/19/18.
//

#ifndef PETSIM_PETSTEPPINGACTION_HH
#define PETSIM_PETSTEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "PETRunAction.hh"
#include "G4LogicalVolume.hh"

//#include "TTree.h"
//#include "TBranch.h"
  G4int GetGammaCount();
  // G4int gammaCounter = 0;


class G4Track;
class G4StepPoint;
class G4OpBoundaryProcess;


class PETDetectorConstruction;

class PETSteppingAction : public G4UserSteppingAction {
public:
  PETSteppingAction(PETDetectorConstruction*);
  PETSteppingAction(PETDetectorConstruction*, int* val, int* val1, int* val2, int* val3, int* val4, double* val5, double* val6);
  virtual ~PETSteppingAction();
  int* fProdCount;
  int* fDetCountO;
  int* fDetCountI;
  int* fDetLoss;
  int* fKillCount;
  double* fPhotTimeO;
  double* fPhotTimeI;

  virtual void UserSteppingAction(const G4Step*);
  virtual void PrepareNewEvent();
  static void EndofRun();



  G4int DetectedCerenCounter;









private:
  G4OpBoundaryProcess* fOpProcess;
  PETDetectorConstruction* fDetector;
  PETRunAction *fRunAction;
  G4LogicalVolume *sCrystal_Log;




};


#endif //PETSIM_PETSTEPPINGACTION_HH
