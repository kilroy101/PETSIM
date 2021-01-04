#ifndef PETDetectorConstruction_h
#define PETDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4MaterialTable.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"

class PETDetectorMessenger;

class G4VPhysicalVolume;

class G4LogicalVolume;

class PETDetectorConstruction : public G4VUserDetectorConstruction {
public:
  PETDetectorConstruction();

  virtual ~PETDetectorConstruction();

  virtual G4VPhysicalVolume *Construct();

  virtual void BuildMaterial();

  virtual void ConstructSDandField();

  // set methods
  void SetOpeningAngle(G4double newValue);

  void SetInnerDiameter(G4double newValue);

  void SetOuterDiameter(G4double newValue);

  void SetHeight(G4double newValue);

private:
  G4double fOpeningAngle;
  G4double fInnerDiameter;
  G4double fOuterDiameter;
  G4double fHeight;

  G4double world_hx;
  G4double world_hy;
  G4double world_hz;
  G4Box *worldBox;
  G4LogicalVolume *worldLog;
  G4VPhysicalVolume *worldPhy;

  G4Box *waterBox;
  G4LogicalVolume *waterLog;
  G4VPhysicalVolume *waterPhy;
  
  G4UserLimits* fStepLimit;            // pointer to user step limits



  G4Box *sCrystal_Mod;
  G4Box *sDet_Mod;
  G4LogicalVolume *sCrystal_Log;
  G4LogicalVolume *detectorLog;
  G4LogicalVolume *wrapLog;
  G4VPhysicalVolume *sCrystal_Phy;
  G4VPhysicalVolume *detectorPhy;
  G4VPhysicalVolume *sTop_Wrap_Phy;

  G4Material *fAir;
  G4Material *fWater;
  G4Material *fNaI;
  G4Material *fLYSO;
  G4Material *fTeflon;
  G4Material *fBGO;
  G4Material *fLutetiumOxide;
  G4Material *fYttriumOxide;
  G4Material *fSiliconDioxide;
  G4Material *fGel;
  G4Material *fPolystyrene;
  G4Material *fBariumSulfate;
  G4Material *fTitaniumDioxide;
  G4Material *fGlue;
  G4Material *fResin;
  G4Material *fCircuitBoard;
  G4Material *fLead;
  G4MaterialPropertiesTable *fLYSO_MPT;
  G4MaterialPropertiesTable *fAir_MPT;
  G4MaterialPropertiesTable *fGel_MPT;
  G4MaterialPropertiesTable *fBGO_MPT;
  G4MaterialPropertiesTable *fGlue_MPT;
  G4MaterialPropertiesTable *fTeflon_MPT;
  G4MaterialPropertiesTable *fPoly_MPT;
  G4MaterialPropertiesTable *fBariumSulfate_MPT;
  G4MaterialPropertiesTable *fTitaniumDioxide_MPT;
  G4MaterialPropertiesTable* fPlasticSurface_MPT;
  G4MaterialPropertiesTable* fBaSO4Surface_MPT;
  G4MaterialPropertiesTable* fTiO2Surface_MPT;
  G4MaterialPropertiesTable* fCrystalSurface_MPT;
  G4MaterialPropertiesTable* fCircuitBoard_MPT;
  G4MaterialPropertiesTable* fLead_MPT;
  G4VProcess* fPhotoElectric;

  PETDetectorMessenger *fMessenger;
};

#endif
