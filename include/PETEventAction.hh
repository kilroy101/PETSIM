#ifndef PETEventAction_h
#define PETEventAction_h 1

#include "PETRunAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4EventManager.hh"



class PETRunAction;

class PETEventAction : public G4UserEventAction {
public:
  PETEventAction() = default;
  PETEventAction(PETRunAction *runAction);
  PETEventAction(PETRunAction *runAction, int* val, int* val1, int* val2, int* val3, int* val4, double* val5, double* val6);

  int* fProdCount;
  int* fDetCountO;
  int* fDetCountI;
  int* fDetLoss;
  int* fKillCount;
  double* fPhotTimeO;
  double* fPhotTimeI;

  virtual ~PETEventAction();

  virtual void BeginOfEventAction(const G4Event* event);
  virtual void EndOfEventAction(const G4Event* event);
private:
  PETRunAction *fRunAction;

  G4int fMPPCCollID;
};

class PETUserManager : public G4EventManager {
public:
G4UserSteppingAction * 	GetUserSteppingAction();
};

#endif
