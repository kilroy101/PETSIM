#include "PETActionInitialization.hh"
#include "PETDetectorConstruction.hh"

#include "PETPrimaryGeneratorAction.hh"
#include "PETRunAction.hh"
#include "PETStackingAction.hh"
#include "PETEventAction.hh"
#include "PETSteppingAction.hh"



PETActionInitialization::PETActionInitialization(PETDetectorConstruction* det)
  : G4VUserActionInitialization(),
    fDetector(det) {
}
PETActionInitialization::PETActionInitialization(PETDetectorConstruction* det, int* ProdCount, int* DetCountO, int* DetCountI, int* DetLoss, int* KillCount, double* PhotTimeO, double* PhotTimeI)
  : G4VUserActionInitialization(),
    fDetector(det),
    fProdCount(ProdCount),
    fDetCountO(DetCountO),
    fDetCountI(DetCountI),
    fDetLoss(DetLoss),
    fKillCount(KillCount),
    fPhotTimeO(PhotTimeO),
    fPhotTimeI(PhotTimeI){
}


PETActionInitialization::~PETActionInitialization() {
}

void PETActionInitialization::BuildForMaster() const{
  SetUserAction(new PETRunAction());
}

void PETActionInitialization::Build() const {
  SetUserAction(new PETPrimaryGeneratorAction());

  PETRunAction* runAction = new PETRunAction();
  SetUserAction(runAction);

  PETEventAction* eventAction = new PETEventAction(runAction, fProdCount, fDetCountO, fDetCountI, fDetLoss, fKillCount, fPhotTimeO, fPhotTimeI);
  SetUserAction(eventAction);

  SetUserAction(new PETStackingAction(runAction));
  PETSteppingAction* steppingAction = new PETSteppingAction(fDetector, fProdCount, fDetCountO, fDetCountI, fDetLoss, fKillCount, fPhotTimeO, fPhotTimeI);
  SetUserAction(steppingAction);
}
