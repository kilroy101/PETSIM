//////////PETDetectorConstruction
// Rylee Ross, Kyle Klein, Ben Fox

#include "PETDetectorConstruction.hh"
#include "PETDetectorMessenger.hh"
#include "PETTrackerSD.hh"
#include "G4SDManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4GeometryManager.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4UserLimits.hh"

#include <iostream>
#include <fstream>
#include <vector>

PETDetectorConstruction::PETDetectorConstruction()
: G4VUserDetectorConstruction(),
world_hx(5.0 * m),
world_hy(5.0 * m),
world_hz(5.0 * m),
fMessenger(NULL), //initialized after PETDetectorMessenger
fLYSO_MPT(NULL),
fAir_MPT(NULL),
fGel_MPT(NULL),
fStepLimit(NULL),
fOpeningAngle(1.5),
fInnerDiameter(50*cm),
fOuterDiameter(60*cm),
fHeight(10*cm),
worldLog(NULL),
worldPhy(NULL),
sCrystal_Log(NULL) {
    fMessenger = new PETDetectorMessenger(this);
}

PETDetectorConstruction::~PETDetectorConstruction() {
	  delete fStepLimit;
}

void PETDetectorConstruction::BuildMaterial() {
    G4NistManager * nist = G4NistManager::Instance();

    G4Element *Hydrogen = nist->FindOrBuildElement("H");
    G4Element *Carbon = nist->FindOrBuildElement("C");
    G4Element *Chlorine = nist->FindOrBuildElement("Cl");
    G4Element *Oxygen = nist->FindOrBuildElement("O");
    G4Element *Lutetium = nist->FindOrBuildElement("Lu");
    G4Element *Yttrium = nist->FindOrBuildElement("Y");

    G4Element *fTeflon = nist->FindOrBuildElement("Teflon");

    G4Element *Na = nist->FindOrBuildElement("Na");
    G4Element *I = nist->FindOrBuildElement("I");

    G4Element *Bismuth = nist->FindOrBuildElement("Bi");
    G4Element *Germanium = nist->FindOrBuildElement("Ge");
    
    G4Element *Cerium = nist->FindOrBuildElement("Ce");
    
    G4Element *Silicon = nist->FindOrBuildElement("Si");
    G4Element *Lead = nist->FindOrBuildElement("Pb");


    // Air
    fAir = nist->FindOrBuildMaterial("G4_AIR");
    
    //Water
    fWater = nist->FindOrBuildMaterial("G4_WATER");
       
    

    //NaI
    fNaI = new G4Material("SodiumIodine", 3.67*g/cm3, 2);
    fNaI->AddElement(Na,1);
    fNaI->AddElement(I,1);

    // Lu2O3
    fLutetiumOxide = new G4Material("LutetiumOxide", 9.41 * g / cm3, 2);
    fLutetiumOxide->AddElement(Lutetium, 2);
    fLutetiumOxide->AddElement(Oxygen, 3);

    // SiO2
    fSiliconDioxide = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

	//The material used to simulate ESR film, a.k.a Vikuiti.
    fPolystyrene = nist->FindOrBuildMaterial("G4_POLYSTYRENE");

     //BaSO4
    fBariumSulfate = nist->FindOrBuildMaterial("G4_BARIUM_SULFATE");
    
    //TiO2
    fTitaniumDioxide = nist->FindOrBuildMaterial("G4_TITANIUM_DIOXIDE");

    // Y2O3
    fYttriumOxide = new G4Material("YttriumOxide", 5.01 * g / cm3, 2);
    fYttriumOxide->AddElement(Yttrium, 2);
    fYttriumOxide->AddElement(Oxygen, 3);

    // LYSO
    //fLYSO = new G4Material("LYSO", 7.1 * g / cm3, 3);
    //fLYSO->AddMaterial(fLutetiumOxide, 81 * perCent);
    //fLYSO->AddMaterial(fSiliconDioxide, 14 * perCent);
    //fLYSO->AddMaterial(fYttriumOxide, 5 * perCent);
    fLYSO = new G4Material("LYSO", 7.1 * g / cm3, 4);
    fLYSO->AddMaterial(fLutetiumOxide, 80.5* perCent);
    fLYSO->AddMaterial(fSiliconDioxide, 13.6 * perCent);
    fLYSO->AddMaterial(fYttriumOxide, 5.3 * perCent);
    fLYSO->AddElement(Cerium, 0.6 * perCent);

    fBGO = new G4Material("BGO", 7.13 * g/cm3, 3);
        fBGO->AddElement(Bismuth, 4);
        fBGO->AddElement(Germanium, 3);
        fBGO->AddElement(Oxygen, 12);

    fGel = new G4Material("Gel", 7.1 * g / cm3, 3);
    fGel->AddMaterial(fLutetiumOxide, 81 * perCent);
    fGel->AddMaterial(fSiliconDioxide, 14 * perCent);
    fGel->AddMaterial(fYttriumOxide, 5 * perCent);

    fGlue = new G4Material("Glue", 1.25 * g/cm3, 4);
    fGlue->AddElement(Carbon, 21);
    fGlue->AddElement(Hydrogen, 25);
    fGlue->AddElement(Chlorine, 1);
    fGlue->AddElement(Oxygen, 5);
    
    fCircuitBoard = new G4Material("CircuitBoard", 2.329 * g/cm3,1);
    fCircuitBoard->AddElement(Silicon,100);
    
    fLead = new G4Material("Lead", 11.4*g/cm3,1);
    fLead->AddElement(Lead,100);

    // Optical Properties of Materials ( nm/1242.38 )
    const G4int num = 120;

    G4double ene[num] = {3.51370304301*eV,3.46284728249*eV,3.4156577087*eV,3.36977603198*eV,3.23635193804*eV,3.22094763375*eV,3.20764136152*eV,3.19637713232*eV,3.18291759837*eV,3.17218298275*eV,3.16123184911*eV,3.1503731311*eV,3.1396057038*eV,3.12928927829*eV,3.12192642432*eV,3.11495009612*eV,3.10797996538*eV,3.10108780463*eV,3.09434823581*eV,3.0876570207*eV,3.0810055618*eV,3.07438540703*eV,3.06780442816*eV,3.06124350731*eV,3.05469721997*eV,3.04817354563*eV,3.04165911826*eV,3.03518039566*eV,3.02874761411*eV,3.02234465979*eV,3.01597393385*eV,3.00960145787*eV,3.00323000843*eV,2.99723759298*eV,2.99057154424*eV,2.984211427*eV,2.97752386539*eV,2.9682041556*eV,2.95859140503*eV,2.95063652972*eV,2.93947102052*eV,2.92513342795*eV,2.89963655932*eV,2.87253927733*eV,2.85352309132*eV,2.83628914618*eV,2.8222957219*eV,2.80993218523*eV,2.79766640437*eV,2.78549390188*eV,2.77344008928*eV,2.76149998282*eV,2.74965574426*eV,2.73791911725*eV,2.72625989445*eV,2.71468687979*eV,2.70321484598*eV,2.62319693516*eV,2.61249533729*eV,2.60188652089*eV,2.59136351654*eV,2.58094246644*eV,2.5706077365*eV,2.56035262437*eV,2.55018739616*eV,2.53884632056*eV,2.46805717364*eV,2.45631868318*eV,2.44469337969*eV,2.43204791693*eV,2.41841286976*eV,2.40494311619*eV,2.38943563283*eV,2.37197339274*eV,2.35478359973*eV,2.33681395534*eV,2.31707351876*eV,2.30377129545*eV,2.25609178885*eV,2.23488272047*eV,2.21410318259*eV,2.19371928099*eV,2.17372057346*eV,2.15408900424*eV,2.13481455788*eV,2.11588408116*eV,2.09729119825*eV,2.07902560898*eV,2.0610754277*eV,2.04343516532*eV,2.02609686477*eV,2.00905157727*eV,1.99229317822*eV,1.97581265517*eV,1.95960675492*eV,1.94367103348*eV,1.92800809024*eV,1.91260128415*eV,1.89744776279*eV,1.88252859884*eV,1.86783894415*eV,1.85337461802*eV,1.83912783245*eV,1.82509840454*eV,1.81128037215*eV,1.7976639421*eV,1.78425469025*eV,1.77643183324*eV,2.64723791997*eV,2.65431419592*eV,2.66836164719*eV,2.6790662172*eV,2.6863139081*eV,2.69716329233*eV,2.53364876322*eV,2.5209805194*eV,2.51147200842*eV,2.49905148118*eV,2.48976188644*eV,2.48661723357*eV};


    G4double BGOatt[num] = {1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm, 1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm, 1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm};

    G4double fast[num] = {0.013820781627794876, 0.013938358896580905, 0.014050591744058316, 0.01770083149773294, 0.1361291993771121, 0.17951253934849465, 0.22274311794414414, 0.2652312673463606, 0.3114997040874239, 0.35139224620750076, 0.40066157551326187, 0.4516999082721216, 0.5045072444840795, 0.5579031796294753, 0.6034774651162591, 0.6472816782656827, 0.688432386235458, 0.7345952706556798, 0.7938782640197157, 0.8552250947457001, 0.9177512611070833, 0.9805722613773165, 1.0445725972829487, 1.1076884314620314, 1.1693300960968656, 1.2303820929140001, 1.2893702523691863, 1.3492429135509216, 1.4111794120946055, 1.4734107445471392, 1.5362317448173723, 1.5958095720902579, 1.6524390602746462, 1.7103057819919416, 1.766140287506697, 1.8148092601521415, 1.8614449477107844, 1.910241473878124, 1.9568572980042371, 1.9966865978054973, 2.040506844218846, 2.084160879129502, 2.1189188575511695, 2.0948333321877324, 2.053620718004467, 2.012992427217593, 1.9736802001779261, 1.9334781269904011, 1.8919493012130524, 1.848651471982605, 1.807122646205256, 1.7669205730177313, 1.7258339981036572, 1.6856319249161322, 1.642334095685685, 1.597267263002139, 1.5526426811818679, 1.2800838749773036, 1.2372282966101307, 1.1952572199695073, 1.153286143328884, 1.1139685718679084, 1.0750932512702074, 1.035775679809232, 0.9977848609380802, 0.9548461768195611, 0.6689313945441024, 0.6314819655514974, 0.594386337249512, 0.556352584860516, 0.5169089741303492, 0.4798240346709801, 0.4399488619201559, 0.3975782897867255, 0.3587457245594927, 0.32193320213070775, 0.2849053292144217, 0.2621546621464601, 0.2066071763487929, 0.17922479084668286, 0.1590792376527035, 0.1416675988862397, 0.12715069304302862, 0.11392033516570743, 0.1019765252542757, 0.09051517083005267, 0.08017954587598375, 0.07064801340059512, 0.061116480925207384, 0.05222822243276415, 0.043983237923266305, 0.03605989040524049, 0.028779816870159625, 0.02166056183081544, 0.015667036261624467, 0.011442514145532101, 0.01156009141431813, 0.013285853640466527, 0.017584711798394714, 0.020757840486169243, 0.02296605819952635, 0.024531001929938068, 0.024648579198724097, 0.024766156467510125, 0.02456209674482368, 0.022428215073302393, 0.021580881367671, 0.023419362297779855, 1.3330963870244557, 1.3692671524065998, 1.4099353409976352, 1.4506171995680286, 1.486787964950173, 1.5274698235205666, 0.9082334285675229, 0.8675652399764877, 0.8223586182384865, 0.7862151928150589, 0.750058097412273, 0.727447951553593};

    G4double rLyso[num] = {1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82};

    // G4double rGel[num] = {1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82};

    G4double rBaSO4[num] = {1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636};

    G4double rPolystyrene[num] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};

    G4double rAir[num] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

     G4double rBGO[num] = {2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15};

    G4double rGel[num] = {1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55};

    G4double val = 1.67;

    //G4double rBGO[num] = {val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val};
    
    G4double rGlue[num] = {val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val};

    G4double abs[num] = {20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm,20*cm};


	//const G4int numBigEnergies = 11;
	//G4double bigEnergies[numBigEnergies] = {15*keV, 21*keV, 32*keV, 40*keV, 60*keV, 75*keV, 85*keV, 120*keV, 200*keV,300*keV, 500*keV};
	//G4double scintYields[numBigEnergies] = {0.6*33200, 0.66*33200, 0.75*33200, 0.79*33200, 0.87*33200, 0.82*33200, 0.84*33200, 0.9*33200, 0.92*33200, 0.97*33200, 1*33200};

    fLYSO_MPT = new G4MaterialPropertiesTable();
    fAir_MPT = new G4MaterialPropertiesTable();
    fGel_MPT = new G4MaterialPropertiesTable();
    fBGO_MPT = new G4MaterialPropertiesTable();
    fPolystyrene_MPT = new G4MaterialPropertiesTable();
    fBariumSulfate_MPT = new G4MaterialPropertiesTable();
    fTitaniumDioxide_MPT = new G4MaterialPropertiesTable();
    fGlue_MPT = new G4MaterialPropertiesTable();
    fCircuitBoard_MPT = new G4MaterialPropertiesTable();
    fLead_MPT = new G4MaterialPropertiesTable();

    fLYSO_MPT->AddProperty("FASTCOMPONENT", ene, fast, num)->SetSpline(true);
    fLYSO_MPT->AddProperty("RINDEX", ene, rLyso, num);
    fLYSO_MPT->AddProperty("ABSLENGTH", ene, abs, num);
    fLYSO_MPT->AddConstProperty("SCINTILLATIONYIELD", 33200 / MeV);
    //fLYSO_MPT->AddProperty("ELECTRONSCINTILLATIONYIELD", bigEnergies,scintYields,numBigEnergies);
    fLYSO_MPT->AddConstProperty("RESOLUTIONSCALE", 5.555);
    fLYSO_MPT->AddConstProperty("FASTTIMECONSTANT", 43 * ns);
    
    
    //fLYSO->GetIonisation()->SetBirksConstant(0.111* CLHEP::mm/CLHEP ::MeV);
    fAir_MPT->AddProperty("RINDEX", ene, rAir, num);

    fGel_MPT->AddProperty("RINDEX", ene, rGel, num);
    
    fGlue_MPT->AddProperty("RINDEX", ene, rGlue, num);

    // fBGO_MPT->AddProperty("FASTCOMPONENT", ene, fast, num)->SetSpline(true);
    fBGO_MPT->AddProperty("RINDEX", ene,rBGO,num);
    // fBGO_MPT->AddProperty("ABSLENGTH", ene, BGOatt, num);
    // fBGO_MPT->AddConstProperty("SCINTILLATIONYIELD", 9000 / MeV);
    // fBGO_MPT->AddConstProperty("RESOLUTIONSCALE", 1);
    // fBGO_MPT->AddConstProperty("FASTTIMECONSTANT", 300 * ns);


    fLYSO->SetMaterialPropertiesTable(fLYSO_MPT);
    fAir->SetMaterialPropertiesTable(fAir_MPT);
    fGel->SetMaterialPropertiesTable(fGel_MPT);
    fBGO->SetMaterialPropertiesTable(fBGO_MPT);
    fPolystyrene->SetMaterialPropertiesTable(fPolystyrene_MPT);
    fBariumSulfate->SetMaterialPropertiesTable(fBariumSulfate_MPT);
    fTitaniumDioxide->SetMaterialPropertiesTable(fTitaniumDioxide_MPT);
    fGlue->SetMaterialPropertiesTable(fGlue_MPT);
    fCircuitBoard->SetMaterialPropertiesTable(fCircuitBoard_MPT);
    fLead->SetMaterialPropertiesTable(fLead_MPT);
}

G4VPhysicalVolume * PETDetectorConstruction::Construct() {
    BuildMaterial();

    //Set this boolean to true if you would like Geant4 to report overlaps. Set to false if not. If trying to check overlaps,
    //beware that ESR separators naturally have many overlap, so remove them before trying to check overlaps of other items.
    G4bool checkOverlaps = false; 
    // Clear old geometry if any
    if (worldPhy) {
        G4GeometryManager::GetInstance()->OpenGeometry();
        G4PhysicalVolumeStore::GetInstance()->Clean();
        G4LogicalVolumeStore::GetInstance()->Clean();
        G4SolidStore::GetInstance()->Clean();
    }
    
    //Defining dimensions of the pieces that make up a crystal/detector block
    //The size of each crystal
    G4double cryst_dx = (3.005)*mm, cryst_dy = (3.005)*mm, cryst_dz = 30*mm;
    //The size of each SiPM pixel
    G4double det_dx = (1-0.0)*mm, det_dy = (1-0.0)*mm, det_dz = 1*mm;
    //The thickness of the separator between each crystal
    G4double sep_thickness = 0.195*mm;
    //The gap between SiPM pixels. Should be exactly 0.2mm.
    G4double gap_c = cryst_dx + sep_thickness - det_dx;
    //Thickness of the layer of glue between crystal and SiPM
    G4double glue_dz = 0.1*mm;
    //The thickness of a circuit board that, if desired, could be placed in front or behind the SiPMs.
    G4double front_board_dz = 2*mm;
    //The number of crystals in each row of a block and the number of separators in each such row
    //(one separator between each crystal and one separator on each end of the row.)
    G4int nb_crys = 8, nb_separators = nb_crys+1;
    //The total width and height of each block. Should be 25.795 mm.
    G4double block_dx = nb_crys*cryst_dx+nb_separators*sep_thickness;
    G4double block_dy = nb_crys*cryst_dx+nb_separators*sep_thickness;
    //Defines the thickness of each block.
    G4double block_dz = cryst_dz+2*det_dz+2*glue_dz;

    //Defining parameters for how these blocks make up the segments of the scanner.
    //Spacing between adjacent blocks. Note that block_dx and block_dy are both 25.795mm - just 0.005mm less than the 25.8mm in Marek's drawings.
    //The extra 0.005mm in blockSpacing makes up for this difference (in Marek's drawings, block spacing is 0.6mm)
    G4double blockSpacing = 0.605 * mm;
    //The number of rings in the barrel segment.
    G4int nb_rings = 8;
    //The number of blocks making up each such ring of the barrel segment.
    G4int nb_blocks =44;
    //The circumferential angle occupied by each block in the barrel.
    G4double dPhi = twopi/nb_blocks;
    //The inner radius of the barrel. Should calculate to 184.56mm, yielding an inner diameter of 369.12mm - very small difference from Marek's
    //drawings of 369.2mm
    G4double ring_R1 = 0.5*(block_dx+blockSpacing)/std::tan(0.5*dPhi);
    //The outer radius of the barrel.
    G4double ring_R2 = ring_R1+cryst_dz+2*det_dz+2*glue_dz;
    //Defining the number of rows of crystal blocks in the corona section.
    const G4int numCoronaRows = 14;
    //For each row in the corona section, defining how many blocks there are.
    G4int numBlocksInCoronaRow[numCoronaRows] = {6, 10, 12, 12, 14, 14, 14, 14, 14, 14, 12, 12, 10, 6};
    //Defining the number of rows of crystal blocks in the chin section.
    const G4int numChinRows = 7;
    //For each row in the corona section, defining how many blocks there are.
    G4int numBlocksInChinRow[numChinRows] = {6, 10, 12, 12, 14, 14, 14};


    worldBox = new G4Box("WorldBox",1*m,1*m,1*m);
    worldLog = new G4LogicalVolume(worldBox, fAir, "WorldLog");
    worldPhy = new G4PVPlacement(0, G4ThreeVector(), worldLog, "WorldPhy", 0, false, 0, checkOverlaps);

    G4Tubs* solidIRing = new G4Tubs("RingI", 0, ring_R2+5*mm , 15*block_dy + 5*mm, 0.,twopi);
    G4LogicalVolume* logicIRing = new G4LogicalVolume(solidIRing, fAir,"RingI");
      

    G4Box* solidCryst = new G4Box("crystal", cryst_dx/2, cryst_dy/2, (cryst_dz)/2);
    G4LogicalVolume* logicCryst = new G4LogicalVolume(solidCryst,fLYSO,"CrystalLV");

    G4Box* solidDet_outer = new G4Box("detector",  det_dx/2, det_dy/2, det_dz/2);
    G4LogicalVolume* logicDet_outer = new G4LogicalVolume(solidDet_outer,fAir,"detector");

    G4Box* solidDet_side = new G4Box("detector", det_dz/2, cryst_dy/2, cryst_dz/2);
    G4LogicalVolume* logicDet_side =  new G4LogicalVolume(solidDet_side, fAir, "detector");

    G4Box* solidDet_top = new G4Box("detector", det_dx/2, det_dz/2, cryst_dz/2);
    G4LogicalVolume* logicDet_top =  new G4LogicalVolume(solidDet_top, fAir, "detector");

    G4Box* solidPolystyreneSide = new G4Box("PolystyreneSide",(sep_thickness - 0.001*mm)/2,block_dy/2,block_dz/2);
    G4Box* solidPolystyreneTop = new G4Box("PolystyreneTop",block_dx/2,(sep_thickness-0.001*mm)/2,block_dz/2);
    G4LogicalVolume* logicPolystyreneTop = new G4LogicalVolume(solidPolystyreneTop, fPolystyrene, "PolystyreneTopLV");
    G4LogicalVolume* logicPolystyreneSide = new G4LogicalVolume(solidPolystyreneSide,fPolystyrene,"PolystyreneSideLV");
    G4Box* solidPolystyreneBlock = new G4Box("PolystyreneBlock", block_dx/2, block_dy/2, (sep_thickness - 0.001*mm)/2);
    G4LogicalVolume* logicPolystyreneBlock = new G4LogicalVolume(solidPolystyreneBlock, fPolystyrene, "PolystyreneBlockLV");

    G4Box* solidBaSO4Side = new G4Box("BaSO4Side",sep_thickness/2,block_dy/2,block_dz/2);
    G4Box* solidBaSO4Top = new G4Box("BaSO4Top",block_dx/2,sep_thickness/2,block_dz/2);
    G4LogicalVolume* logicBaSO4Top = new G4LogicalVolume(solidBaSO4Top, fBariumSulfate, "BaSO4TopLV");
    G4LogicalVolume* logicBaSO4Side = new G4LogicalVolume(solidBaSO4Side,fBariumSulfate,"BaSO4SideLV");
    G4Box* solidBaSO4Block = new G4Box("BaSO4Block", block_dx/2, block_dy/2, sep_thickness/2);
    G4LogicalVolume* logicBaSO4Block = new G4LogicalVolume(solidBaSO4Block, fBariumSulfate, "BaSO4BlockLV");
    
	G4Box* solidTiO2Side = new G4Box("TiO2Side",sep_thickness/2,block_dy/2,block_dz/2);
    G4Box* solidTiO2Top = new G4Box("TiO2Top",block_dx/2,sep_thickness/2,block_dz/2);
    G4LogicalVolume* logicTiO2Top = new G4LogicalVolume(solidTiO2Top, fTitaniumDioxide, "TiO2TopLV");
    G4LogicalVolume* logicTiO2Side = new G4LogicalVolume(solidTiO2Side,fTitaniumDioxide,"TiO2SideLV");
    G4Box* solidTiO2Block = new G4Box("TiO2Block", block_dx/2, block_dy/2, sep_thickness/2);
    G4LogicalVolume* logicTiO2Block = new G4LogicalVolume(solidTiO2Block, fTitaniumDioxide, "TiO2BlockLV");

    G4Box* solidGlue = new G4Box("Glue", det_dx/2, det_dy/2, glue_dz/2);
    G4LogicalVolume* logicGlue =  new G4LogicalVolume(solidGlue, fGlue, "Glue");
    
    G4Box* solidFrontBoard = new G4Box("CircuitBoard", cryst_dx/2, cryst_dy/2, front_board_dz/2);
    G4LogicalVolume* logicFrontBoard =  new G4LogicalVolume(solidFrontBoard, fCircuitBoard, "CircuitBoard");


	const G4int n = 6;
	G4double PhotonEnergy[n] = {3.105*eV,2.95714*eV,2.855*eV,2.7*eV,2.5875*eV,2.388*eV};
	G4double specularlobe[n] = {1.0,1.0,1.0,1.0,1.0,1.0};
	G4double specularspike[n] = {0.0,0.0,0.0,0.0,0.0,0.0};
	G4double backscatter[n] = {0.0,0.0,0.0,0.0,0.0,0.0};
    //Defining the surface properties of crystals. Crystals may be ground or polished, so choose the correct sigma alpha.
    G4double sigma_alphaGround = 0.209439; //12 deg., for ground crystal
	G4double sigma_alphaPolished = 0.0226893; //1.3 deg., for polished crystal
	//G4double sigma_alphaTest = 0.209439 * radian;
	
	//G4double crystalTransmission[n] = {0.1, 0.1, 0.1, 0.1, 0.1, 0.1};
	G4OpticalSurface* opCrystalSurface = new G4OpticalSurface("CrystalSurface");
	fCrystalSurface_MPT = new G4MaterialPropertiesTable();
	fCrystalSurface_MPT->AddProperty("SPECULARLOBECONSTANT",PhotonEnergy,specularlobe,n);
	fCrystalSurface_MPT->AddProperty("SPECULARSPIKECONSTANT",PhotonEnergy,specularspike,n);
	fCrystalSurface_MPT->AddProperty("BACKSCATTERCONSTANT",PhotonEnergy,backscatter,n);
	//fCrystalSurface_MPT->AddProperty("TRANSMITTANCE", PhotonEnergy, crystalTransmission, n);
	opCrystalSurface->SetFinish(ground);
	opCrystalSurface->SetModel(unified);
	opCrystalSurface->SetSigmaAlpha(sigma_alphaPolished);
	opCrystalSurface->SetMaterialPropertiesTable(fCrystalSurface_MPT);
	//ONLY set crystal skin surface if using Vikuiti/air coupled separator, to set sigmaalpha at crystal-air interface.
	//G4LogicalSkinSurface* crystalSurface = new G4LogicalSkinSurface("Crystal Surface", logicCryst, opCrystalSurface);

	
	//Defining surface properties of detectors
    G4OpticalSurface* opDetectorSurface = new G4OpticalSurface("DetectorSurface");
      opDetectorSurface->SetFinish(ground);  
    const G4int effNum = 64;
    G4double ephoton_pmt[effNum] = {4.39937628481263* eV, 4.26793273171382* eV, 4.25415407177944* eV, 4.2526101654458* eV, 4.25002775896025* eV, 4.23740768267528* eV, 4.23740768267528* eV, 4.22619410336501* eV, 4.21111365866894* eV, 4.19468373277856* eV, 4.17562764465* eV, 4.16485707439122* eV, 4.13287618164404* eV, 4.11182706958868* eV, 4.06068564113166* eV, 3.98990143108327* eV, 3.91255690517576* eV, 3.82905214632875* eV, 3.73170831479369* eV, 3.63100753721902* eV, 3.54335760187188* eV, 3.39028291586928* eV, 3.313575740457* eV, 3.26400828279202* eV, 3.2073708649341* eV, 3.13717401566535* eV, 3.06569708729327* eV, 2.99026243581681* eV, 2.92677376304993* eV, 2.81453688978407* eV, 2.71059029663409* eV, 2.61404815600077* eV, 2.51541948296409* eV, 2.47766764433903* eV, 2.43449980055764* eV, 2.38261019441473* eV, 2.31642767655566* eV, 2.28160772652005* eV, 2.24045139601668* eV, 2.20375721015054* eV, 2.16824560915714* eV, 2.13386033457783* eV, 2.09415257211902* eV, 2.06881296944741* eV, 2.03552435424357* eV, 1.99418369175086* eV, 1.9602891257364* eV, 1.92752749589036* eV, 1.89584292993575* eV, 1.8621095224178* eV, 1.82796546757295* eV, 1.78873181823744* eV, 1.75185829972651* eV, 1.71101762636774* eV, 1.67203778810094* eV, 1.63479444340984* eV, 1.60473676359121* eV, 1.56380972550422* eV, 1.53439796153312* eV, 1.50385115860456* eV, 1.47365578936701* eV, 1.44464911982904* eV, 1.416762310119* eV, 1.39353045806523};
    G4double reflectivity_pmt[effNum] = {0.02696598, 0.05893489, 0.07987711, 0.11722792, 0.1014316, 0.12730853, 0.14009744, 0.15859531, 0.18073614, 0.2021246, 0.22482906, 0.24095285, 0.26211995, 0.28273989, 0.30246012, 0.32418864, 0.34063229, 0.35529757, 0.36923624, 0.38282257, 0.39408207, 0.40300594, 0.41655242, 0.4362877, 0.45028576, 0.4658789, 0.48091868, 0.49178681, 0.4997318, 0.50860914, 0.50921274, 0.49967056, 0.47483176, 0.45819545, 0.44008796, 0.42196203, 0.39788722, 0.38184955, 0.36710202, 0.35260655, 0.33865954, 0.32471254, 0.3101095, 0.29717251, 0.28064034, 0.26568978, 0.25062753, 0.23575333, 0.22153729, 0.20680864, 0.19418899, 0.17932864, 0.16402218, 0.14939387, 0.13560323, 0.1227101, 0.11222387, 0.10103327, 0.08902995, 0.07927751, 0.06943588, 0.06025241, 0.05136811, 0.0440509};
    G4MaterialPropertiesTable *detectorMPT = new G4MaterialPropertiesTable();
    //Currently using reflectivity to account for detector efficiency, because as of yet I can't make efficiency work. Photons that are "reflected" off the detector are really just counted as detector hits, and they're killed when they reflect. All other photons hitting the 
    //detector are "absorbed" in Geant4, which for us means that they are lost due to detector efficiency.
    detectorMPT->AddProperty("REFLECTIVITY", ephoton_pmt, reflectivity_pmt, effNum);
    //detectorMPT->AddProperty("EFFICIENCY",   ephoton_pmt, efficiency_pmt,   effNum);
    opDetectorSurface->SetMaterialPropertiesTable(detectorMPT);
    G4LogicalSkinSurface* DetectorSideSurface = new G4LogicalSkinSurface("Detector Surface", logicDet_outer, opDetectorSurface);


	//Defining surface properites for Vikuiti, BaSO4, and TiO2. First, tables that will be used.
	const G4int num = 120;
    G4double ene[num] = {3.51370304301*eV,3.46284728249*eV,3.4156577087*eV,3.36977603198*eV,3.23635193804*eV,3.22094763375*eV,3.20764136152*eV,3.19637713232*eV,3.18291759837*eV,3.17218298275*eV,3.16123184911*eV,3.1503731311*eV,3.1396057038*eV,3.12928927829*eV,3.12192642432*eV,3.11495009612*eV,3.10797996538*eV,3.10108780463*eV,3.09434823581*eV,3.0876570207*eV,3.0810055618*eV,3.07438540703*eV,3.06780442816*eV,3.06124350731*eV,3.05469721997*eV,3.04817354563*eV,3.04165911826*eV,3.03518039566*eV,3.02874761411*eV,3.02234465979*eV,3.01597393385*eV,3.00960145787*eV,3.00323000843*eV,2.99723759298*eV,2.99057154424*eV,2.984211427*eV,2.97752386539*eV,2.9682041556*eV,2.95859140503*eV,2.95063652972*eV,2.93947102052*eV,2.92513342795*eV,2.89963655932*eV,2.87253927733*eV,2.85352309132*eV,2.83628914618*eV,2.8222957219*eV,2.80993218523*eV,2.79766640437*eV,2.78549390188*eV,2.77344008928*eV,2.76149998282*eV,2.74965574426*eV,2.73791911725*eV,2.72625989445*eV,2.71468687979*eV,2.70321484598*eV,2.62319693516*eV,2.61249533729*eV,2.60188652089*eV,2.59136351654*eV,2.58094246644*eV,2.5706077365*eV,2.56035262437*eV,2.55018739616*eV,2.53884632056*eV,2.46805717364*eV,2.45631868318*eV,2.44469337969*eV,2.43204791693*eV,2.41841286976*eV,2.40494311619*eV,2.38943563283*eV,2.37197339274*eV,2.35478359973*eV,2.33681395534*eV,2.31707351876*eV,2.30377129545*eV,2.25609178885*eV,2.23488272047*eV,2.21410318259*eV,2.19371928099*eV,2.17372057346*eV,2.15408900424*eV,2.13481455788*eV,2.11588408116*eV,2.09729119825*eV,2.07902560898*eV,2.0610754277*eV,2.04343516532*eV,2.02609686477*eV,2.00905157727*eV,1.99229317822*eV,1.97581265517*eV,1.95960675492*eV,1.94367103348*eV,1.92800809024*eV,1.91260128415*eV,1.89744776279*eV,1.88252859884*eV,1.86783894415*eV,1.85337461802*eV,1.83912783245*eV,1.82509840454*eV,1.81128037215*eV,1.7976639421*eV,1.78425469025*eV,1.77643183324*eV,2.64723791997*eV,2.65431419592*eV,2.66836164719*eV,2.6790662172*eV,2.6863139081*eV,2.69716329233*eV,2.53364876322*eV,2.5209805194*eV,2.51147200842*eV,2.49905148118*eV,2.48976188644*eV,2.48661723357*eV};
	G4double reflecVikuiti[num] = {0.9167, 0.9167, 0.9167, 0.9378, 0.9412, 0.9412, 0.9467, 0.9467, 0.9467, 0.9467, 0.9467, 0.9467, 0.9467, 0.9507, 0.9507, 0.9507, 0.9507, 0.9507, 0.9507, 0.9507, 0.9507, 0.9507, 0.9507, 0.9524, 0.9524, 0.9524, 0.9524, 0.9524, 0.9524, 0.9524, 0.9524, 0.9524, 0.9524, 0.9524, 0.9524, 0.9548, 0.9548, 0.9548, 0.9548, 0.9548, 0.9548, 0.9548, 0.9581, 0.9581, 0.9581, 0.9605, 0.9605, 0.9605, 0.9605, 0.9605, 0.9605, 0.9605, 0.9605, 0.9605, 0.9605, 0.9628, 0.9628, 0.964, 0.964, 0.9647, 0.9647, 0.9647, 0.9647, 0.9647, 0.9659, 0.9659, 0.9661, 0.9661, 0.9665, 0.9665, 0.9665, 0.9676, 0.9676, 0.9676, 0.9676, 0.9676, 0.9674, 0.9674, 0.9677, 0.9677, 0.9678, 0.9683, 0.9683, 0.9684, 0.9684, 0.9683, 0.9683, 0.9686, 0.9686, 0.9683, 0.9683, 0.9681, 0.9681, 0.969, 0.969, 0.9695, 0.9695, 0.9693, 0.9693, 0.969, 0.969, 0.9692, 0.9692, 0.9685, 0.9685, 0.9683, 0.9683, 0.9682, 0.964, 0.964, 0.9628, 0.9628, 0.9628, 0.9628, 0.9659, 0.9659, 0.9659, 0.9661, 0.9661, 0.9661};
	//G4double reflecVikuiti[num] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	//G4double reflecTiO2[num] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	G4double rBaSO4[num] = {1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636};
    G4double reflecBaSO4[num] = {0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955};
    G4double rTiO2[num] = {1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61, 1.61};
    G4double reflecTiO2[num] = {0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955, 0.955};
	G4double transmittanceTiO2[num] = {0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045};

	//Ascribing Vikuiti properties
	fPlasticSurface_MPT = new G4MaterialPropertiesTable();
	//Defining probabilities that the Vikuity surface undergoes certain reflection processes. Since using unified model, should be 100% specular lobe, with sigma_alpha specified.
	fPlasticSurface_MPT->AddProperty("SPECULARLOBECONSTANT",PhotonEnergy,specularlobe,n);
	fPlasticSurface_MPT->AddProperty("SPECULARSPIKECONSTANT",PhotonEnergy,specularspike,n);
	fPlasticSurface_MPT->AddProperty("BACKSCATTERCONSTANT",PhotonEnergy,backscatter,n);
	fPlasticSurface_MPT->AddProperty("REFLECTIVITY", ene, reflecVikuiti, num);
    G4OpticalSurface* opPlasticSurface = new G4OpticalSurface("PlasticSurface");
      opPlasticSurface->SetModel(unified);
      opPlasticSurface->SetType(dielectric_metal);
      opPlasticSurface->SetFinish(ground);
      //opPlasticSurface->SetSigmaAlpha(sigma_alphaPolished);
      opPlasticSurface->SetMaterialPropertiesTable(fPlasticSurface_MPT);
	G4LogicalSkinSurface* PlasticSurface_T = new G4LogicalSkinSurface("Plastic Surface", logicPolystyreneTop, opPlasticSurface);
    G4LogicalSkinSurface* PlasticSurface_S = new G4LogicalSkinSurface("Plastic Surface", logicPolystyreneSide, opPlasticSurface);
    
    //Ascribing BaSO4 properties
    fBaSO4Surface_MPT = new G4MaterialPropertiesTable();
	//Defining probabilities that the surface undergoes certain reflection processes. Since using unified model, should be 100% specular lobe, with sigma_alpha specified.
	fBaSO4Surface_MPT->AddProperty("SPECULARLOBECONSTANT",PhotonEnergy,specularlobe,n);
	fBaSO4Surface_MPT->AddProperty("SPECULARSPIKECONSTANT",PhotonEnergy,specularspike,n);
	fBaSO4Surface_MPT->AddProperty("BACKSCATTERCONSTANT",PhotonEnergy,backscatter,n);
	fBaSO4Surface_MPT->AddProperty("RINDEX", ene, rBaSO4, num);
	fBaSO4Surface_MPT->AddProperty("REFLECTIVITY", ene, reflecBaSO4, num);
    G4OpticalSurface* opBaSO4Surface = new G4OpticalSurface("BaSO4Surface");
      opBaSO4Surface->SetModel(unified);
      opBaSO4Surface->SetType(dielectric_dielectric);
      opBaSO4Surface->SetFinish(groundbackpainted);
      opBaSO4Surface->SetSigmaAlpha(sigma_alphaGround);
      opBaSO4Surface->SetMaterialPropertiesTable(fBaSO4Surface_MPT);
    G4LogicalSkinSurface* BaSO4Surface_T = new G4LogicalSkinSurface("BaSO4 Surface", logicBaSO4Top, opBaSO4Surface);
    G4LogicalSkinSurface* BaSO4Surface_S = new G4LogicalSkinSurface("BaSO4 Surface", logicBaSO4Side, opBaSO4Surface);
    //G4LogicalBorderSurface* BaSO4Surface_T  = new G4LogicalBorderSurface("BaSO4 Top Surface", logicCryst, logicBaSO4Top, opBaSO4Surface);
    //G4LogicalBorderSurface* BaSO4Surface_S  = new G4LogicalBorderSurface("BaSO4 Side Surface", logicCryst, logicBaSO4Side, pBaSO4Surface);
    
    //Ascribing TiO2 properties
    fTiO2Surface_MPT = new G4MaterialPropertiesTable();
	//Defining probabilities that the surface undergoes certain reflection processes. Since using unified model, should be 100% specular lobe, with sigma_alpha specified.
	fTiO2Surface_MPT->AddProperty("SPECULARLOBECONSTANT",PhotonEnergy,specularlobe,n);
	fTiO2Surface_MPT->AddProperty("SPECULARSPIKECONSTANT",PhotonEnergy,specularspike,n);
	fTiO2Surface_MPT->AddProperty("BACKSCATTERCONSTANT",PhotonEnergy,backscatter,n);
	fTiO2Surface_MPT->AddProperty("RINDEX", ene, rTiO2, num);
	fTiO2Surface_MPT->AddProperty("REFLECTIVITY", ene, reflecTiO2, num);
	//fTiO2Surface_MPT->AddProperty("TRANSMITTANCE", ene, transmittanceTiO2, num);
    G4OpticalSurface* opTiO2Surface = new G4OpticalSurface("TiO2Surface");
	  opTiO2Surface->SetModel(unified);
      opTiO2Surface->SetType(dielectric_dielectric);
      opTiO2Surface->SetFinish(groundbackpainted);
      opTiO2Surface->SetSigmaAlpha(sigma_alphaGround);
      opTiO2Surface->SetMaterialPropertiesTable(fTiO2Surface_MPT);
    G4LogicalSkinSurface* TiO2Surface_T = new G4LogicalSkinSurface("TiO2 Surface", logicTiO2Top, opTiO2Surface);
    G4LogicalSkinSurface* TiO2Surface_S = new G4LogicalSkinSurface("TiO2 Surface", logicTiO2Side, opTiO2Surface);
    G4LogicalSkinSurface* TiO2Surface_B = new G4LogicalSkinSurface("TiO2 Surface", logicTiO2Block, opTiO2Surface);


//Adding the corona
//Documentation for how CopyNumber is defined for each crystal and detector:
 //CopyNumber has either 6 or 7 digits (7 only if in the corona).
//The first digit runs from 0 to 9. If this digit is from 0 to 7, it tells the ring in the barrel along which the crystal/detector is located.
//If the first digit is an 8, then the crystal/detector is in the chin section. If it is a 9, then it is in the corona (and there will be 7 digits)
//For those in the barrel, the second and third digits run from 00 to 43, and they tell the module along the ring. For detectors, the fourth digit is either a 0 or a 1. If a 0, it is a front detector; if a 1, it is a back detector.
//For those in the chin section, the second and third digits run from 00 to 35, and they tell the module of the section, where from the axial view looking down through the corona where the chin module is to the right of the page, 00 is the top left module.
//and 35 is the bottom right module (sequence left to right and then top to bottom).
//For those in the corona, the second, third, and fourth digits run from 00 to 143 and similarly tell the module of the corona, where from the same view as for the chin 00 is the top left and 143 the bottom right.
//The 3rd to last digit is 1 for back detectors and 0 for crystals and front detectors.
//The last 2 digits run from 00 to 63, and they tell the location of the crystal/detector within the 8x8 module, running from left to right and top to bottom.

//Defining the axial position of the bottom of the corona.
G4double coronaZ = (210.6/2)*mm;
G4RotationMatrix rotm_corona  = G4RotationMatrix();
for (G4int xmoduleIndex = 0; xmoduleIndex < numCoronaRows; xmoduleIndex++){
	G4double xmodule = (xmoduleIndex - numCoronaRows/2) * (block_dx + blockSpacing);
	for (G4int ymoduleIndex = 0; ymoduleIndex < numBlocksInCoronaRow[xmoduleIndex]; ymoduleIndex++){
		G4double ymodule = (ymoduleIndex - numBlocksInCoronaRow[xmoduleIndex]/2) * (block_dx + blockSpacing);
		for (G4int xcrystIndex = 0; xcrystIndex < nb_crys; xcrystIndex++){
			for (G4int ycrystIndex = 0; ycrystIndex < nb_crys; ycrystIndex++){
							  G4int CoronaCrysCopyNumber = 9000000 + (xmoduleIndex + 12*ymoduleIndex)*1000 + 0*100 + (xcrystIndex + nb_crys * ycrystIndex);
						      G4ThreeVector position_corona_cryst = G4ThreeVector(xmodule + (0.5 + xcrystIndex)*(det_dx+gap_c),
						                                                     ymodule + (0.5 + ycrystIndex)*(det_dy+gap_c),
						                                                     coronaZ + det_dz + glue_dz + 0.5*cryst_dz); 
						      G4Transform3D transform_corona_cryst = G4Transform3D(rotm_corona,position_corona_cryst);
						      new G4PVPlacement(transform_corona_cryst,             //rotation,position
						                          logicCryst,            //its logical volume
						                          "crystal",             //its name
						                          logicIRing,             //its mother  volume
						                          false,                 //no boolean operation
						                          CoronaCrysCopyNumber,                 //copy number
						                          checkOverlaps);       // checking overlaps
						            
						      G4int CoronaOuterDetCopyNumber = 9000000 + (xmoduleIndex + 12*ymoduleIndex)*1000 + 1*100 + (xcrystIndex + nb_crys * ycrystIndex);                   
						      G4ThreeVector position_corona_det_outer = G4ThreeVector(xmodule + (0.5 + xcrystIndex)*(det_dx+gap_c),
																			ymodule + (0.5 + ycrystIndex)*(det_dy+gap_c),
																			coronaZ + det_dz + glue_dz + cryst_dz + glue_dz + 0.5*det_dz);
							  G4Transform3D transform_corona_det_outer = G4Transform3D(rotm_corona,position_corona_det_outer);
						      new G4PVPlacement(transform_corona_det_outer,             //rotation,position
						                          logicDet_outer,            //its logical volume
						                          "detector",             //its name
						                          logicIRing,             //its mother  volume
						                          false,                 //no boolean operation
						                          CoronaOuterDetCopyNumber,                 //copy number
						                          checkOverlaps);       // checking overlaps
						                          
							  G4int CoronaInnerDetCopyNumber = 9000000 + (xmoduleIndex + 12*ymoduleIndex)*1000 + 0*100 + (xcrystIndex + nb_crys * ycrystIndex);                   
						      G4ThreeVector position_corona_det_inner = G4ThreeVector(xmodule + (0.5 + xcrystIndex)*(det_dx+gap_c),
																			ymodule + (0.5 + ycrystIndex)*(det_dy+gap_c),
																			coronaZ + 0.5*det_dz);
							  G4Transform3D transform_corona_det_inner = G4Transform3D(rotm_corona,position_corona_det_inner);
						      new G4PVPlacement(transform_corona_det_inner,             //rotation,position
						                          logicDet_outer,            //its logical volume
						                          "detector",             //its name
						                          logicIRing,             //its mother  volume
						                          false,                 //no boolean operation
						                          CoronaInnerDetCopyNumber,                 //copy number
						                          checkOverlaps);       // checking overlaps
						                          
							  G4int CoronaOuterGlueCopyNumber = 1;                   
						      G4ThreeVector position_corona_glue_outer = G4ThreeVector(xmodule + (0.5 + xcrystIndex)*(det_dx+gap_c),
																			ymodule + (0.5 + ycrystIndex)*(det_dy+gap_c),
																			coronaZ + det_dz + glue_dz + cryst_dz + 0.5*glue_dz);
							  G4Transform3D transform_corona_glue_outer = G4Transform3D(rotm_corona,position_corona_glue_outer);
						      new G4PVPlacement(transform_corona_glue_outer,             //rotation,position
						                          logicGlue,            //its logical volume
						                          "glue",             //its name
						                          logicIRing,             //its mother  volume
						                          false,                 //no boolean operation
						                          CoronaOuterGlueCopyNumber,                 //copy number
						                          checkOverlaps);       // checking overlaps
						                          
							  G4int CoronaInnerGlueCopyNumber = 1;                   
						      G4ThreeVector position_corona_glue_inner = G4ThreeVector(xmodule + (0.5 + xcrystIndex)*(det_dx+gap_c),
																			ymodule + (0.5 + ycrystIndex)*(det_dy+gap_c),
																			coronaZ + det_dz + 0.5*glue_dz);
							  G4Transform3D transform_corona_glue_inner = G4Transform3D(rotm_corona,position_corona_glue_inner);
						      new G4PVPlacement(transform_corona_glue_inner,             //rotation,position
						                          logicGlue,            //its logical volume
						                          "glue",             //its name
						                          logicIRing,             //its mother  volume
						                          false,                 //no boolean operation
						                          CoronaInnerGlueCopyNumber,                 //copy number
						                          checkOverlaps);       // checking overlaps										                        						                          
		
		}}
		
		for (G4int  xGelIndex = 0; xGelIndex < nb_separators ; xGelIndex++){
				      for (G4int yGelIndex = 0; yGelIndex < nb_separators ; yGelIndex++){
				
				        G4ThreeVector Corona_side_separator_position = G4ThreeVector(xmodule + (xGelIndex)*(det_dx+gap_c),
				                                                     ymodule + 0.5*block_dx,
				                                                     coronaZ + det_dz + glue_dz + 0.5*cryst_dz);
				
				        G4ThreeVector Corona_top_separator_position = G4ThreeVector(xmodule + 0.5*block_dx,
				                                                     ymodule + (yGelIndex)*(det_dy+gap_c),
				                                                     coronaZ + det_dz + glue_dz + 0.5*cryst_dz);
				
				
				        G4Transform3D Corona_side_separator_transform = G4Transform3D(rotm_corona,Corona_side_separator_position);
				        G4Transform3D Corona_top_separator_transform = G4Transform3D(rotm_corona,Corona_top_separator_position);
				
				
				  new G4PVPlacement(Corona_side_separator_transform,
				                    logicPolystyreneSide,
				                    "separatorSide",
				                    logicIRing,false,1,checkOverlaps);
				
				
				new G4PVPlacement(Corona_top_separator_transform,
				                  logicPolystyreneTop,"separatorTop",
				                  logicIRing,
				                  false,1,checkOverlaps);
				
				
				    }}
}}


//Adding the chin
//Defining the axial position of the top of the chin module
G4double chinZ = -(210.6/2)*mm - 164.4*mm;
G4RotationMatrix rotm_chin  = G4RotationMatrix();
for (G4int xmoduleIndex = 0; xmoduleIndex < numChinRows; xmoduleIndex++){
	G4double xmodule = (xmoduleIndex - numChinRows) * (block_dx + blockSpacing);
	for (G4int ymoduleIndex = 0; ymoduleIndex < numBlocksInChinRow[xmoduleIndex]; ymoduleIndex++){
		G4double ymodule = (ymoduleIndex - numBlocksInChinRow[xmoduleIndex]/2) * (block_dx + blockSpacing);
		for (G4int xcrystIndex = 0; xcrystIndex < nb_crys; xcrystIndex++){
			for (G4int ycrystIndex = 0; ycrystIndex < nb_crys; ycrystIndex++){
							  G4int chinCrysCopyNumber = 800000 + (xmoduleIndex + 6*ymoduleIndex)*1000 + 0*100 + (xcrystIndex + nb_crys * ycrystIndex);
						      G4ThreeVector position_chin_cryst = G4ThreeVector(xmodule + (xcrystIndex)*(det_dx+gap_c),
						                                                     ymodule + (0.5 + ycrystIndex)*(det_dy+gap_c),
						                                                     chinZ - det_dz - glue_dz - 0.5*cryst_dz); 
						      G4Transform3D transform_chin_cryst = G4Transform3D(rotm_chin,position_chin_cryst);
						      new G4PVPlacement(transform_chin_cryst,             //rotation,position
						                          logicCryst,            //its logical volume
						                          "crystal",             //its name
						                          logicIRing,             //its mother  volume
						                          false,                 //no boolean operation
						                          chinCrysCopyNumber,                 //copy number
						                          checkOverlaps);       // checking overlaps
						            
						      G4int chinOuterDetCopyNumber = 800000 + (xmoduleIndex + 6*ymoduleIndex)*1000 + 1*100 + (xcrystIndex + nb_crys * ycrystIndex);                   
						      G4ThreeVector position_chin_det_outer = G4ThreeVector(xmodule + (xcrystIndex)*(det_dx+gap_c),
																			ymodule + (0.5 + ycrystIndex)*(det_dy+gap_c),
																			chinZ - det_dz - glue_dz - cryst_dz - glue_dz - 0.5*det_dz);
							  G4Transform3D transform_chin_det_outer = G4Transform3D(rotm_chin,position_chin_det_outer);
						      new G4PVPlacement(transform_chin_det_outer,             //rotation,position
						                          logicDet_outer,            //its logical volume
						                          "detector",             //its name
						                          logicIRing,             //its mother  volume
						                          false,                 //no boolean operation
						                          chinOuterDetCopyNumber,                 //copy number
						                          checkOverlaps);       // checking overlaps
						                          
							  G4int chinInnerDetCopyNumber = 800000 + (xmoduleIndex + 6*ymoduleIndex)*1000 + 0*100 + (xcrystIndex + nb_crys * ycrystIndex);                   
						      G4ThreeVector position_chin_det_inner = G4ThreeVector(xmodule + (xcrystIndex)*(det_dx+gap_c),
																			ymodule + (0.5 + ycrystIndex)*(det_dy+gap_c),
																			chinZ - 0.5*det_dz);
							  G4Transform3D transform_chin_det_inner = G4Transform3D(rotm_chin,position_chin_det_inner);
						      new G4PVPlacement(transform_chin_det_inner,             //rotation,position
						                          logicDet_outer,            //its logical volume
						                          "detector",             //its name
						                          logicIRing,             //its mother  volume
						                          false,                 //no boolean operation
						                          chinInnerDetCopyNumber,                 //copy number
						                          checkOverlaps);       // checking overlaps
						                          
							  G4int chinOuterGlueCopyNumber = 1;                   
						      G4ThreeVector position_chin_glue_outer = G4ThreeVector(xmodule + (xcrystIndex)*(det_dx+gap_c),
																			ymodule + (0.5 + ycrystIndex)*(det_dy+gap_c),
																			chinZ - det_dz - glue_dz - cryst_dz - 0.5*glue_dz);
							  G4Transform3D transform_chin_glue_outer = G4Transform3D(rotm_chin,position_chin_glue_outer);
						      new G4PVPlacement(transform_chin_glue_outer,             //rotation,position
						                          logicGlue,            //its logical volume
						                          "glue",             //its name
						                          logicIRing,             //its mother  volume
						                          false,                 //no boolean operation
						                          chinOuterGlueCopyNumber,                 //copy number
						                          checkOverlaps);       // checking overlaps
						                          
							  G4int chinInnerGlueCopyNumber = 1;                   
						      G4ThreeVector position_chin_glue_inner = G4ThreeVector(xmodule + ( xcrystIndex)*(det_dx+gap_c),
																			ymodule + (0.5 + ycrystIndex)*(det_dy+gap_c),
																			chinZ - det_dz - 0.5*glue_dz);
							  G4Transform3D transform_chin_glue_inner = G4Transform3D(rotm_chin,position_chin_glue_inner);
						      new G4PVPlacement(transform_chin_glue_inner,             //rotation,position
						                          logicGlue,            //its logical volume
						                          "glue",             //its name
						                          logicIRing,             //its mother  volume
						                          false,                 //no boolean operation
						                          chinInnerGlueCopyNumber,                 //copy number
						                          checkOverlaps);       // checking overlaps										                        						                          
		
		}}
		
		for (G4int  xGelIndex = 0; xGelIndex < nb_separators ; xGelIndex++){
				      for (G4int yGelIndex = 0; yGelIndex < nb_separators ; yGelIndex++){
				
				        G4ThreeVector chin_side_separator_position = G4ThreeVector(xmodule + (xGelIndex - 0.5)*(det_dx+gap_c),
				                                                     ymodule + 0.5*block_dx,
				                                                     chinZ - det_dz - glue_dz - 0.5*cryst_dz);
				
				        G4ThreeVector chin_top_separator_position = G4ThreeVector(xmodule + 0.5*block_dx - 0.5*(det_dx+gap_c),
				                                                     ymodule + (yGelIndex)*(det_dy+gap_c),
				                                                     chinZ - det_dz - glue_dz - 0.5*cryst_dz);
				
				
				        G4Transform3D chin_side_separator_transform = G4Transform3D(rotm_chin,chin_side_separator_position);
				        G4Transform3D chin_top_separator_transform = G4Transform3D(rotm_chin,chin_top_separator_position);
				
				
				  new G4PVPlacement(chin_side_separator_transform,
				                    logicPolystyreneSide,
				                    "separatorSide",
				                    logicIRing,false,1,checkOverlaps);
				
				
				new G4PVPlacement(chin_top_separator_transform,
				                  logicPolystyreneTop,"separatorTop",
				                  logicIRing,
				                  false,1,checkOverlaps);
				
				
				    }}
}}

//Adding the main barrel
  for (G4int ringIndex = 0; ringIndex < nb_rings; ringIndex++){
	      //Defining the axial position of the axial center of the blocks in the given ring.
		  G4double ringZ = (ringIndex - nb_rings/2 + 0.5) * (block_dx+blockSpacing);
		  for (G4int blockIndex = 0; blockIndex < nb_blocks; blockIndex ++){
			      //Defining the angle that controls where along the circumference of the barrel a block will be placed, using the dPhi defined earlier
			      //that gives the angle occupied by each block.
				  G4double phi = blockIndex * dPhi;
				  G4double Bx = (std::cos(phi))*(ring_R1+0.5*cryst_dz);
				  G4double By = (std::sin(phi))*(ring_R1+0.5*cryst_dz);
				  G4double ODx = (std::cos(phi))*(ring_R1+cryst_dz+glue_dz+0.5*det_dz);
				  G4double ODxGlue = (std::cos(phi))*(ring_R1+cryst_dz+0.5*glue_dz);
				  G4double ODy = (std::sin(phi))*(ring_R1+cryst_dz+glue_dz+0.5*det_dz);
				  G4double ODyGlue = (std::sin(phi))*(ring_R1+cryst_dz+0.5*glue_dz);
				  G4double IDx = (std::cos(phi))*(ring_R1-glue_dz-0.5*det_dz);
				  G4double IDxGlue = (std::cos(phi))*(ring_R1-0.5*glue_dz);
				  G4double IDxFrontBoard = (std::cos(phi))*(ring_R1-glue_dz - det_dz - 0.5*front_board_dz);
				  G4double IDy = (std::sin(phi))*(ring_R1-glue_dz-0.5*det_dz);
				  G4double IDyGlue = (std::sin(phi))*(ring_R1-0.5*glue_dz);
				  G4double IDyFrontBoard = (std::sin(phi))*(ring_R1-glue_dz - det_dz - 0.5*front_board_dz);
				
				  G4RotationMatrix rotm_crys  = G4RotationMatrix();
				  rotm_crys.rotateY(90*deg);
				  rotm_crys.rotateZ(phi);
				
				  G4RotationMatrix rotm_out  = G4RotationMatrix();
				  rotm_out.rotateY(90*deg);
				  rotm_out.rotateZ(phi);
				
				  G4RotationMatrix rotm_in  = G4RotationMatrix();
				  rotm_in.rotateY(90*deg);
				  rotm_in.rotateZ(phi);
				
				  //for (G4int icrys = 3; icrys < 4 ; icrys++){
				    //for (G4int jcrys = 3; jcrys < 4 ; jcrys++){
				for (G4int icrys = 0; icrys < nb_crys ; icrys++){
				    for (G4int jcrys = 0; jcrys < nb_crys ; jcrys++){
				      G4int CrysCopyNumber = ringIndex * 100000 + blockIndex*1000 +(jcrys + nb_crys*icrys ) ;
				      G4ThreeVector position_cryst = G4ThreeVector(Bx+(nb_crys/2 - 0.5 - icrys)*(det_dx+gap_c)*-sin(phi),//168.16*mm,// - (3.5-icrys)*det_dx*sin(phi),// +0.5*det_dz,
				                                                     By+(nb_crys/2 - 0.5-icrys)*(det_dx+gap_c)*cos(phi),
				                                                     ringZ + (nb_crys/2 - 0.5-jcrys)*(det_dx+gap_c)); 
				      G4Transform3D transform_cryst = G4Transform3D(rotm_crys,position_cryst);
				      new G4PVPlacement(transform_cryst,             //rotation,position
				                          logicCryst,            //its logical volume
				                          "crystal",             //its name
				                          logicIRing,             //its mother  volume
				                          false,                 //no boolean operation
				                          CrysCopyNumber,                 //copy number
				                          checkOverlaps);       // checking overlaps
				                          
				
				}}
				
				    for (G4int  iSeparator = 0; iSeparator < nb_separators ; iSeparator++){
				      for (G4int jSeparator = 0; jSeparator < nb_separators ; jSeparator++){
				
				        G4ThreeVector side_separator_position = G4ThreeVector(Bx,//+0.5*det_dz,//- (3.5-icrys)*det_dx*sin(phi),
				                                                     By,
				                                                     ringZ + (nb_crys/2 - jSeparator)*(det_dx+gap_c));
				
				        G4ThreeVector top_separator_position = G4ThreeVector(Bx+((nb_crys/2 - iSeparator)*(det_dx+gap_c))*-sin(phi),
				                                                     By+((nb_crys/2 - iSeparator)*(det_dx+gap_c))*cos(phi),
				                                                     ringZ + 0);
				
				
				        G4Transform3D side_separator_transform = G4Transform3D(rotm_crys,side_separator_position);
				        G4Transform3D top_separator_transform = G4Transform3D(rotm_crys,top_separator_position);
				
				
				  new G4PVPlacement(side_separator_transform,
				                    logicPolystyreneSide,
				                    "separatorSide",
				                    logicIRing,false,iSeparator,checkOverlaps);
				
				
				new G4PVPlacement(top_separator_transform,
				                  logicPolystyreneTop,"separatorTop",
				                  logicIRing,
				                  false,jSeparator,checkOverlaps);
				        
				
				    }}//end of separators
				
				    //for (G4int  icrys = 3; icrys < 4; icrys++){
				      //for (G4int jcrys = 3; jcrys < 4 ; jcrys++){
				    for (G4int  icrys = 0; icrys < nb_crys; icrys++){
				      for (G4int jcrys = 0; jcrys < nb_crys ; jcrys++){
				
				  //G4int OutdetCopyNumber = jcrys + nb_crys*icrys+64;
				  G4int OutdetCopyNumber = ringIndex * 100000 + blockIndex*1000 + 100 + (jcrys + nb_crys*icrys);
				  G4ThreeVector position_out_det = G4ThreeVector(ODx+(nb_crys/2 - 0.5-icrys)*(det_dx+gap_c)*-sin(phi), //- 0.03*cryst_dz,//172.41*mm,//+det_dz- (3.5-icrys)*det_dx*sin(phi),
				                                                  ODy+(nb_crys/2 - 0.5-icrys)*(det_dx+gap_c)*cos(phi),
				                                                  ringZ + (nb_crys/2 - 0.5-jcrys)*(det_dx+gap_c)); 
				  G4Transform3D transform_out_det = G4Transform3D(rotm_out,position_out_det);
				
				  G4ThreeVector position_out_glue = G4ThreeVector(ODxGlue+(nb_crys/2 - 0.5-icrys)*(det_dx+gap_c)*-sin(phi),
				                                                  ODyGlue+(nb_crys/2 - 0.5-icrys)*(det_dx+gap_c)*cos(phi),
				                                                  ringZ + (nb_crys/2 - 0.5-jcrys)*(det_dx+gap_c));
				  G4Transform3D transform_out_glue = G4Transform3D(rotm_out,position_out_glue);
			
				
					new G4PVPlacement(transform_out_det,             //rotation,position
				    		           logicDet_outer,            //its logical volume
						               "detector",             //its name
						               logicIRing,             //its mother  volume
						               false,                 //no boolean operation
						               OutdetCopyNumber,                 //copy number
				  		             checkOverlaps);       // checking overlaps

				  new G4PVPlacement(transform_out_glue,             //rotation,position
				                    logicGlue,            //its logical volume
				                  	"Glue",             //its name
				                  	logicIRing,             //its mother  volume
				                  	false,                 //no boolean operation
				                  	0,                 //copy number
				                    checkOverlaps);       // checking overlaps
				
				  //Detectors on the front face. Only for double-readout geometry.
				  //G4int IndetCopyNumber = jcrys + nb_crys*icrys;
				  G4int IndetCopyNumber =ringIndex * 100000 + blockIndex*1000 + ( jcrys + nb_crys*icrys);
				  G4ThreeVector position_in_det = G4ThreeVector(IDx+(nb_crys/2 - 0.5-icrys)*(det_dx+gap_c)*-sin(phi), //- 0.01*cryst_dz,//+0.4*cos(phi)*mm - (3.5-icrys)*det_dx*sin(phi),
				            IDy+(nb_crys/2 - 0.5-icrys)*(det_dx+gap_c)*cos(phi),
				            ringZ + (nb_crys/2 - 0.5-jcrys)*(det_dx+gap_c));
				
				  G4Transform3D transform_in_det = G4Transform3D(rotm_in,position_in_det);
				  
				  G4ThreeVector position_in_glue = G4ThreeVector(IDxGlue+(nb_crys/2 - 0.5-icrys)*(det_dx+gap_c)*-sin(phi),
				                                                  IDyGlue+(nb_crys/2 - 0.5-icrys)*(det_dx+gap_c)*cos(phi),
				                                                  ringZ + (nb_crys/2 - 0.5-jcrys)*(det_dx+gap_c));
				  G4Transform3D transform_in_glue = G4Transform3D(rotm_out,position_in_glue);
				  
				  G4ThreeVector position_front_board = G4ThreeVector(IDxFrontBoard+(nb_crys/2 - 0.5-icrys)*(det_dx+gap_c)*-sin(phi),
				                                                  IDyFrontBoard+(nb_crys/2 - 0.5-icrys)*(det_dx+gap_c)*cos(phi),
				                                                  ringZ + (nb_crys/2 - 0.5-jcrys)*(det_dx+gap_c));
				  G4Transform3D transform_front_board = G4Transform3D(rotm_out,position_front_board);
									
									  new G4PVPlacement(transform_in_det,             //rotation,position
									                    logicDet_outer,            //its logical volume
									                    "detector",             //its name
									                    logicIRing,             //its mother  volume
									                    false,                 //no boolean operation
									                    IndetCopyNumber,                 //copy number
									                    checkOverlaps);       // checking overlaps
				
				                    new G4PVPlacement(transform_in_glue,             //rotation,position
				                                      logicGlue,            //its logical volume
				                                    	"Glue",             //its name
				                                    	logicIRing,             //its mother  volume
				                                    	false,                 //no boolean operation
				                                    	0,                 //copy number
				                                      checkOverlaps);       // checking overlaps
				                                   
									//new G4PVPlacement(transform_front_board,             //rotation,position
				                                      //logicFrontBoard,            //its logical volume
				                                      //"FrontBoard",             //its name
				                                      //logicIRing,             //its mother  volume
				                                      //false,                 //no boolean operation
				                                      //0,                 //copy number
				                                     //checkOverlaps);       // checking overlaps				                                   
				  }} // end of cryst
				  
			
				  ////Vikuity on front crystal face - only include if doing a single-readout geometry
				  //G4ThreeVector position_seperator_block = G4ThreeVector((ring_R1 - 0.0425*mm)*cos(phi), (ring_R1 - 0.0425*mm)*sin(phi), ringZ);
				  //G4Transform3D seperator_block_transform = G4Transform3D(rotm_crys,position_seperator_block);
				  //new G4PVPlacement(seperator_block_transform,             //rotation,position
					                 //logicPolystyreneBlock,            //its logical volume
					                 //"seperatorBlock",             //its name
					                 //logicIRing,             //its mother  volume
					                 //false,                 //no boolean operation
					                 //0,                 //copy number
					                 //checkOverlaps);       // checking overlaps
			                 
		 }//End of ring
	 }//End of all rings

  for (G4int imod_radial = 0; imod_radial < 1 ; imod_radial++){
    G4double mod_phi = (imod_radial-1)*dPhi;
    for (G4int imod_vertical = 0; imod_vertical < 1 ; imod_vertical++){
    G4double mod_pi = (imod_vertical-1)*dPhi;
    G4RotationMatrix rotm  = G4RotationMatrix();
    // rotm.rotateZ(mod_phi);//mod_phi
	// rotm.rotateY(-1*mod_pi);

	//placing logicIRing, the cylindrical mother volume for the scanner.
    G4ThreeVector position_mod = G4ThreeVector(0,0,0);//std::cos(mod_phi), std::sin(mod_phi)-block_dy/2, imod_vertical-block_dx/2
    G4Transform3D transform_mod = G4Transform3D(rotm,position_mod);
    new G4PVPlacement(transform_mod,                     //no rotation
                    logicIRing,             //its logical volume
                    "IRing",                //its name
                    worldLog,			//its mother volume
                    false,
                    imod_vertical,
                    checkOverlaps);        //no boolean operation
    }}//end of mods

    G4VisAttributes* crystVisAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));//white
  //  G4VisAttributes* SiPMVisAttributes = new G4VisAttributes(G4Colour(0.0,1.0,0.0));//green
    G4VisAttributes* DetVisAttributes = new G4VisAttributes(G4Colour(0.0,0.0,1.0));//blue
    G4VisAttributes* PolystyreneVisAttributes = new G4VisAttributes(G4Colour(1.0,0.0,0.0));//red
    G4VisAttributes* InnerDetVisAttributes = new G4VisAttributes(G4Colour(1.0,1.0,0.0));//yellow
    G4VisAttributes* GlueVisAttributes = new G4VisAttributes(G4Colour(1.0,0.0,1.0));//magenta

    G4VisAttributes* LogVisAttributes = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
    logicIRing->SetVisAttributes(G4VisAttributes::Invisible);//LogVisAttributes
    logicPolystyreneSide->SetVisAttributes(PolystyreneVisAttributes);
    logicPolystyreneTop->SetVisAttributes(PolystyreneVisAttributes);
    logicPolystyreneBlock->SetVisAttributes(PolystyreneVisAttributes);
    logicBaSO4Side->SetVisAttributes(PolystyreneVisAttributes);
    logicBaSO4Top->SetVisAttributes(PolystyreneVisAttributes);

      logicCryst->SetVisAttributes(crystVisAttributes);
      logicDet_outer->SetVisAttributes(DetVisAttributes);
      logicDet_side->SetVisAttributes(DetVisAttributes);
      logicDet_top->SetVisAttributes(DetVisAttributes);
      logicGlue->SetVisAttributes(GlueVisAttributes);

        return worldPhy;
      }

        void PETDetectorConstruction::SetOpeningAngle(G4double newValue) {
          this->fOpeningAngle = newValue;
          G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
        }

        void PETDetectorConstruction::SetInnerDiameter(G4double newValue) {
          this->fInnerDiameter = newValue;
          G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
        }

        void PETDetectorConstruction::SetOuterDiameter(G4double newValue) {
          this->fOuterDiameter = newValue;
          G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
        }

        void PETDetectorConstruction::SetHeight(G4double newValue) {
          this->fHeight = newValue;
          G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
        }


        void PETDetectorConstruction::ConstructSDandField() {
        // Sensitive detectors

        PETTrackerSD* aTrackerSD = new PETTrackerSD("TrackerChamberSD");
        G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);

        // Setting aTrackerSD to all logical volumes with the same name of "Chamber_LV".
        SetSensitiveDetector("detector", aTrackerSD, true);
        }
