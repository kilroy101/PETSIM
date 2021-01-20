#ifndef PETActionInitialization_h
#define PETActionInitialization_h 1

#include "PETPrimaryGeneratorAction.hh"
#include "PETEventAction.hh"
#include "PETRunAction.hh"
#include "PETSteppingAction.hh"


#include "G4VUserActionInitialization.hh"


class PETDetectorConstruction;

class PETActionInitialization : public G4VUserActionInitialization {
public:
  PETActionInitialization(PETDetectorConstruction*);
  PETActionInitialization(PETDetectorConstruction*, int* val, int* val1, int* val2, int* val3, int* val4, double* val5, double* val6);

  virtual ~PETActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;
  int* fProdCount;
  int* fDetCountO;
  int* fDetCountI;
  int* fDetLoss;
  int* fKillCount;
  double* fPhotTimeO;
  double* fPhotTimeI;

private:
  PETDetectorConstruction* fDetector;
};

#endif
