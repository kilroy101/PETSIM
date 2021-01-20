#include "PETEventAction.hh"
#include "PETRunAction.hh"
#include "PETTrackerHit.hh"


#include "G4Event.hh"
// #include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
// #include "PETSteppingAction.hh"
// #include "PETglobals.hh"
// int PETEventAction::gammaCounter = 0;
// int test = event->GetProdPhoton();
PETEventAction::PETEventAction(PETRunAction *runAction) : G4UserEventAction(), fRunAction(runAction){
  fMPPCCollID = 0;
}
PETEventAction::PETEventAction(PETRunAction *runAction,int* val, int* val1, int* val2, int* val3, int* val4, double* val5, double* val6)
: fRunAction(runAction), fProdCount(val), fDetCountO(val1), fDetCountI(val2), fDetLoss(val3), fKillCount(val4), fPhotTimeO(val5), fPhotTimeI(val6){
  fMPPCCollID = 0;
}

PETEventAction::~PETEventAction() {
}

void PETEventAction::BeginOfEventAction(const G4Event *event) {
}

void PETEventAction::EndOfEventAction(const G4Event *event) {

    if (fRunAction->GetRndmFreq() == 2) {
    std::ostringstream os;
    os<<"endOfEvent_"<<G4Threading::G4GetThreadId()<<".rndm";
    G4Random::saveEngineStatus(os.str().c_str());
  }

  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4String colName = "PhotonDetHitCollection";
  fMPPCCollID = SDman->GetCollectionID(colName);

  std::ofstream myfile501("PhotonData.txt", std::ios_base::app);
  myfile501 << *fProdCount << "," << *fDetCountO << "," << *fDetCountI << "," << *fDetLoss << "," << *fKillCount << "," << *fPhotTimeO << "," << *fPhotTimeI <<std::endl;
  myfile501.close();
  *fProdCount = 0;
  *fDetCountO = 0;
  *fDetCountI = 0;
  *fDetLoss = 0;
  *fKillCount = 0;
  *fPhotTimeO = 0;
  *fPhotTimeI = 0;

  G4HCofThisEvent* HCE = event->GetHCofThisEvent();
  PETTrackerHitsCollection* mppcHC = 0;

  G4int EventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();


  // Get the hit collections
  if (HCE) {

    if (fMPPCCollID>=0) mppcHC = (PETTrackerHitsCollection*)(HCE->GetHC(fMPPCCollID));
   }

  //  std::cout<<mppcHC->entries()<<"mmpcHC entries" << std::endl;

  // Get hit information about photons that reached the detector in this event
  if (mppcHC) {
    G4int n_hit = mppcHC->entries();
    auto HitVector = mppcHC->GetVector();
    G4int time_n = 0;
    for (unsigned short int ihit = 0; ihit < n_hit; ihit++) {
      if (ihit == 4){
      time_n = HitVector->at(ihit)->GetArrivalTime()/CLHEP::ns;

      }
      G4double energy = HitVector->at(ihit)->GetEnergy();
      G4ThreeVector photonArrive = HitVector->at(ihit)->GetArrivalPos();
      G4double time = HitVector->at(ihit)->GetArrivalTime()*ns;
      G4double wavelength = 1239.842 / (energy/eV);
      G4double xpos = photonArrive.x();
      G4double ypos = photonArrive.y();
      G4double zpos = photonArrive.z();
      //std::ofstream myfile2("Hits3x315.txt", std::ios_base::app);
      //myfile2 << EventID << ","<< wavelength << ","<<xpos<<","<<ypos<<","<<zpos<<","<<time<<std::endl;
      //myfile2.close();
      //fRunAction->FillEventHitTree(EventID, energy, wavelength, xpos, ypos, zpos, time);


    }
    // std::cout<< EventID <<"," << n_hit << "," << time_n << std::endl;

  }


}
// G4UserSteppingAction * PETUserManager::GetUserSteppingAction();
