/// PETTrackingAction.hh
/// Author: Kyle KLein
/// Last Modified: 1-29-20
#ifndef PETTrackingAction_h
#define PETTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"

class PETTrackingAction : public G4UserTrackingAction{

public:
  PETTrackingAction();
  ~PETTrackingAction();

  void PreUserTrackingAction(const G4Track*);
  void PostUserTrackingAction(const G4Track*);
};
#endif
