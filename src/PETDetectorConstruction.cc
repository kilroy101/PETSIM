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
}

void PETDetectorConstruction::BuildMaterial() {
    G4NistManager * nist = G4NistManager::Instance();

    G4Element *Hydrogen = nist->FindOrBuildElement("H");
    G4Element *Carbon = nist->FindOrBuildElement("C");
    G4Element *Chlorine = nist->FindOrBuildElement("Cl");
    G4Element *Oxigen = nist->FindOrBuildElement("O");
    G4Element *Lutetium = nist->FindOrBuildElement("Lu");
    G4Element *Yttrium = nist->FindOrBuildElement("Y");

    G4Element *fTeflon = nist->FindOrBuildElement("Teflon");

    G4Element *Na = nist->FindOrBuildElement("Na");
    G4Element *I = nist->FindOrBuildElement("I");

    G4Element *Bismuth = nist->FindOrBuildElement("Bi");
    G4Element *Germanium = nist->FindOrBuildElement("Ge");


    // Air
    fAir = nist->FindOrBuildMaterial("G4_AIR");

    //NaI
    fNaI = new G4Material("SodiumIodine", 3.67*g/cm3, 2);
    fNaI->AddElement(Na,1);
    fNaI->AddElement(I,1);

    // Lu2O3
    fLutetiumOxide = new G4Material("LutetiumOxide", 9.41 * g / cm3, 2);
    fLutetiumOxide->AddElement(Lutetium, 2);
    fLutetiumOxide->AddElement(Oxigen, 3);

    // SiO2
    fSiliconDioxide = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

    fPolystyrene = nist->FindOrBuildMaterial("G4_POLYSTYRENE");

     //BaSO4
    fBariumSulfate = nist->FindOrBuildMaterial("G4_BARIUM_SULFATE");

    // Y2O3
    fYttriumOxide = new G4Material("YttriumOxide", 5.01 * g / cm3, 2);
    fYttriumOxide->AddElement(Yttrium, 2);
    fYttriumOxide->AddElement(Oxigen, 3);

    // LYSO
    fLYSO = new G4Material("LYSO", 7.1 * g / cm3, 3);
    fLYSO->AddMaterial(fLutetiumOxide, 81 * perCent);
    fLYSO->AddMaterial(fSiliconDioxide, 14 * perCent);
    fLYSO->AddMaterial(fYttriumOxide, 5 * perCent);

    fBGO = new G4Material("BGO", 7.13 * g/cm3, 3);
        fBGO->AddElement(Bismuth, 4);
        fBGO->AddElement(Germanium, 3);
        fBGO->AddElement(Oxigen, 12);

    fGel = new G4Material("Gel", 7.1 * g / cm3, 3);
    fGel->AddMaterial(fLutetiumOxide, 81 * perCent);
    fGel->AddMaterial(fSiliconDioxide, 14 * perCent);
    fGel->AddMaterial(fYttriumOxide, 5 * perCent);

    fResin = new G4Material("Resin", 1.25 * g/cm3, 4);
    fResin->AddElement(Carbon, 21);
    fResin->AddElement(Hydrogen, 25);
    fResin->AddElement(Chlorine, 1);
    fResin->AddElement(Oxigen, 5);

    fGlue = new G4Material("Glue", 1.25 * g/cm3, 4);
    fGlue->AddElement(Carbon, 21);
    fGlue->AddElement(Hydrogen, 25);
    fGlue->AddElement(Chlorine, 1);
    fGlue->AddElement(Oxigen, 5);

    // Optical Properties of Materials ( nm/1242.38 )
    const G4int num = 120;

    G4double ene[num] = {3.51370304301*eV,3.46284728249*eV,3.4156577087*eV,3.36977603198*eV,3.23635193804*eV,3.22094763375*eV,3.20764136152*eV,3.19637713232*eV,3.18291759837*eV,3.17218298275*eV,3.16123184911*eV,3.1503731311*eV,3.1396057038*eV,3.12928927829*eV,3.12192642432*eV,3.11495009612*eV,3.10797996538*eV,3.10108780463*eV,3.09434823581*eV,3.0876570207*eV,3.0810055618*eV,3.07438540703*eV,3.06780442816*eV,3.06124350731*eV,3.05469721997*eV,3.04817354563*eV,3.04165911826*eV,3.03518039566*eV,3.02874761411*eV,3.02234465979*eV,3.01597393385*eV,3.00960145787*eV,3.00323000843*eV,2.99723759298*eV,2.99057154424*eV,2.984211427*eV,2.97752386539*eV,2.9682041556*eV,2.95859140503*eV,2.95063652972*eV,2.93947102052*eV,2.92513342795*eV,2.89963655932*eV,2.87253927733*eV,2.85352309132*eV,2.83628914618*eV,2.8222957219*eV,2.80993218523*eV,2.79766640437*eV,2.78549390188*eV,2.77344008928*eV,2.76149998282*eV,2.74965574426*eV,2.73791911725*eV,2.72625989445*eV,2.71468687979*eV,2.70321484598*eV,2.62319693516*eV,2.61249533729*eV,2.60188652089*eV,2.59136351654*eV,2.58094246644*eV,2.5706077365*eV,2.56035262437*eV,2.55018739616*eV,2.53884632056*eV,2.46805717364*eV,2.45631868318*eV,2.44469337969*eV,2.43204791693*eV,2.41841286976*eV,2.40494311619*eV,2.38943563283*eV,2.37197339274*eV,2.35478359973*eV,2.33681395534*eV,2.31707351876*eV,2.30377129545*eV,2.25609178885*eV,2.23488272047*eV,2.21410318259*eV,2.19371928099*eV,2.17372057346*eV,2.15408900424*eV,2.13481455788*eV,2.11588408116*eV,2.09729119825*eV,2.07902560898*eV,2.0610754277*eV,2.04343516532*eV,2.02609686477*eV,2.00905157727*eV,1.99229317822*eV,1.97581265517*eV,1.95960675492*eV,1.94367103348*eV,1.92800809024*eV,1.91260128415*eV,1.89744776279*eV,1.88252859884*eV,1.86783894415*eV,1.85337461802*eV,1.83912783245*eV,1.82509840454*eV,1.81128037215*eV,1.7976639421*eV,1.78425469025*eV,1.77643183324*eV,2.64723791997*eV,2.65431419592*eV,2.66836164719*eV,2.6790662172*eV,2.6863139081*eV,2.69716329233*eV,2.53364876322*eV,2.5209805194*eV,2.51147200842*eV,2.49905148118*eV,2.48976188644*eV,2.48661723357*eV};


    G4double BGOatt[num] = {1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm, 1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm, 1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm,1.0*cm};

    G4double fast[num] = {0.013820781627794876, 0.013938358896580905, 0.014050591744058316, 0.01770083149773294, 0.1361291993771121, 0.17951253934849465, 0.22274311794414414, 0.2652312673463606, 0.3114997040874239, 0.35139224620750076, 0.40066157551326187, 0.4516999082721216, 0.5045072444840795, 0.5579031796294753, 0.6034774651162591, 0.6472816782656827, 0.688432386235458, 0.7345952706556798, 0.7938782640197157, 0.8552250947457001, 0.9177512611070833, 0.9805722613773165, 1.0445725972829487, 1.1076884314620314, 1.1693300960968656, 1.2303820929140001, 1.2893702523691863, 1.3492429135509216, 1.4111794120946055, 1.4734107445471392, 1.5362317448173723, 1.5958095720902579, 1.6524390602746462, 1.7103057819919416, 1.766140287506697, 1.8148092601521415, 1.8614449477107844, 1.910241473878124, 1.9568572980042371, 1.9966865978054973, 2.040506844218846, 2.084160879129502, 2.1189188575511695, 2.0948333321877324, 2.053620718004467, 2.012992427217593, 1.9736802001779261, 1.9334781269904011, 1.8919493012130524, 1.848651471982605, 1.807122646205256, 1.7669205730177313, 1.7258339981036572, 1.6856319249161322, 1.642334095685685, 1.597267263002139, 1.5526426811818679, 1.2800838749773036, 1.2372282966101307, 1.1952572199695073, 1.153286143328884, 1.1139685718679084, 1.0750932512702074, 1.035775679809232, 0.9977848609380802, 0.9548461768195611, 0.6689313945441024, 0.6314819655514974, 0.594386337249512, 0.556352584860516, 0.5169089741303492, 0.4798240346709801, 0.4399488619201559, 0.3975782897867255, 0.3587457245594927, 0.32193320213070775, 0.2849053292144217, 0.2621546621464601, 0.2066071763487929, 0.17922479084668286, 0.1590792376527035, 0.1416675988862397, 0.12715069304302862, 0.11392033516570743, 0.1019765252542757, 0.09051517083005267, 0.08017954587598375, 0.07064801340059512, 0.061116480925207384, 0.05222822243276415, 0.043983237923266305, 0.03605989040524049, 0.028779816870159625, 0.02166056183081544, 0.015667036261624467, 0.011442514145532101, 0.01156009141431813, 0.013285853640466527, 0.017584711798394714, 0.020757840486169243, 0.02296605819952635, 0.024531001929938068, 0.024648579198724097, 0.024766156467510125, 0.02456209674482368, 0.022428215073302393, 0.021580881367671, 0.023419362297779855, 1.3330963870244557, 1.3692671524065998, 1.4099353409976352, 1.4506171995680286, 1.486787964950173, 1.5274698235205666, 0.9082334285675229, 0.8675652399764877, 0.8223586182384865, 0.7862151928150589, 0.750058097412273, 0.727447951553593};

    G4double rLyso[num] = {1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82};

    // G4double rGel[num] = {1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82};

    //G4double rGel[num] = {1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9, 1.9};

    //G4double rGel[num] = {1.7, 1.7, 1.7, 1.7, 1.7,1.7, 1.7, 1.7, 1.7, 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7, 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 , 1.7,1.7, 1.7, 1.7, 1.7 ,1.7};


    //G4double rGel[num] = {1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636};

	  G4double rBaSO4[num] = {1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636, 1.636};

    G4double rPoly[num] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};

    G4double rAir[num] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    G4double Reflec[num] = {0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98};

    // G4double rBGO[num] = {2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15, 2.15};

    G4double rGel[num] = {1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55, 1.55};

    G4double val = 1.67;

    G4double rBGO[num] = {val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val};

    G4double abs[num] = {1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm};


    fLYSO_MPT = new G4MaterialPropertiesTable();
    fAir_MPT = new G4MaterialPropertiesTable();
    fGel_MPT = new G4MaterialPropertiesTable();
    fBGO_MPT = new G4MaterialPropertiesTable();
    fPoly_MPT = new G4MaterialPropertiesTable();
    fBariumSulfate_MPT = new G4MaterialPropertiesTable();



    fLYSO_MPT->AddProperty("FASTCOMPONENT", ene, fast, num)->SetSpline(true);
    fLYSO_MPT->AddProperty("RINDEX", ene, rLyso, num);
    fLYSO_MPT->AddProperty("ABSLENGTH", ene, abs, num);
    fLYSO_MPT->AddConstProperty("SCINTILLATIONYIELD", 30000 / MeV);
    fLYSO_MPT->AddConstProperty("RESOLUTIONSCALE", 1);
    fLYSO_MPT->AddConstProperty("FASTTIMECONSTANT", 43 * ns);

    fAir_MPT->AddProperty("RINDEX", ene, rAir, num);

    fGel_MPT->AddProperty("RINDEX", ene, rGel, num);

    // fBGO_MPT->AddProperty("FASTCOMPONENT", ene, fast, num)->SetSpline(true);
    fBGO_MPT->AddProperty("RINDEX", ene,rBGO,num);
    // fBGO_MPT->AddProperty("ABSLENGTH", ene, BGOatt, num);
    // fBGO_MPT->AddConstProperty("SCINTILLATIONYIELD", 9000 / MeV);
    // fBGO_MPT->AddConstProperty("RESOLUTIONSCALE", 1);
    // fBGO_MPT->AddConstProperty("FASTTIMECONSTANT", 300 * ns);

    //fPoly_MPT->AddProperty("RINDEX", ene, rPoly, num);
    fPoly_MPT->AddProperty("REFLECTIVITY", ene, Reflec, num);

    fBariumSulfate_MPT->AddProperty("RINDEX", ene, rBaSO4, num);


    fLYSO->SetMaterialPropertiesTable(fLYSO_MPT);
    fAir->SetMaterialPropertiesTable(fAir_MPT);
    fGel->SetMaterialPropertiesTable(fGel_MPT);
    //fGel1->SetMaterialPropertiesTable(fGel1_MPT);
    fBGO->SetMaterialPropertiesTable(fBGO_MPT);
    fPolystyrene->SetMaterialPropertiesTable(fPoly_MPT);
    fBariumSulfate->SetMaterialPropertiesTable(fBariumSulfate_MPT);
    //fResin->SetMaterialPropertiesTable(fResin_MPT);
    // fGlue->SetMaterialPropertiesTable(fGlue_MPT);


}

G4VPhysicalVolume * PETDetectorConstruction::Construct() {
    BuildMaterial();

    // Overlapping check
    G4bool checkOverlaps =false; //true;

    // Clear old geometry if any
    if (worldPhy) {
        G4GeometryManager::GetInstance()->OpenGeometry();
        G4PhysicalVolumeStore::GetInstance()->Clean();
        G4LogicalVolumeStore::GetInstance()->Clean();
        G4SolidStore::GetInstance()->Clean();
    }

    G4double cryst_dx = 3.0*mm, cryst_dy = 3.0*mm, cryst_dz = 15*mm;
    G4double det_dx = 3*mm, det_dy = 3*mm, det_dz = 1*mm;
    G4double gap_c = 0.2*mm;
    G4double Glue_dz = 0.1*mm;

    G4int nb_block =40;
    G4int nb_crys = 8, nb_gels = nb_crys+1;
    G4double block_dx = nb_crys*cryst_dx+nb_gels*gap_c, block_dy = nb_crys*cryst_dx+nb_crys*gap_c, block_dz = cryst_dz+det_dz+Glue_dz+0.150*mm;
    //G4int nb_rings = 8;
    //
    G4double dPhi = twopi/nb_block, half_dPhi=0.5*dPhi;
    G4double cosdPhi = std::cos(half_dPhi);
    G4double tandPhi = std::tan(half_dPhi);
    //
    G4double ring_R1 = 0.5*block_dx/tandPhi;
    G4double ring_R2 = (ring_R1+cryst_dz+2*det_dz)/cosdPhi;//unknown
    //
    G4double detector_dZ = cryst_dx;//unknown



    worldBox = new G4Box("WorldBox",1*m,1*m,1*m);
    worldLog = new G4LogicalVolume(worldBox, fAir, "WorldLog");
    worldPhy = new G4PVPlacement(0, G4ThreeVector(), worldLog, "WorldPhy", 0, false, 0, checkOverlaps);

    G4Tubs* solidIRing = new G4Tubs("RingI", ring_R1-5*mm, ring_R2+5*mm , 0.5*block_dy+5*mm, -half_dPhi, dPhi);//0.,twopi);//
      G4LogicalVolume* logicIRing = new G4LogicalVolume(solidIRing, fAir,"RingI");

    G4Box* solidCryst = new G4Box("crystal", cryst_dx/2, cryst_dy/2, (cryst_dz+0.01*mm)/2);
    G4LogicalVolume* logicCryst = new G4LogicalVolume(solidCryst,fLYSO,"CrystalLV");

    G4Box* solidDet_outer = new G4Box("detector",  det_dx/2, det_dy/2, det_dz/2);
    G4LogicalVolume* logicDet_outer = new G4LogicalVolume(solidDet_outer,fAir,"detector");

    G4Box* solidDet_side = new G4Box("detector", det_dz/2, cryst_dy/2, cryst_dz/2);
    G4LogicalVolume* logicDet_side =  new G4LogicalVolume(solidDet_side, fAir, "detector");

    G4Box* solidDet_top = new G4Box("detector", det_dx/2, det_dz/2, cryst_dz/2);
    G4LogicalVolume* logicDet_top =  new G4LogicalVolume(solidDet_top, fAir, "detector");

    G4Box* solidDet_Inside_side = new G4Box('detector',0.001*mm/2,cryst_dy/2,cryst_dz/2);
    G4Box* solidDet_Inside_top = new G4Box('detector',cryst_dx/2,0.001*mm/2,cryst_dz/2);
    G4LogicalVolume* logicDet_inside_top = new G4LogicalVolume(solidDet_Inside_top, fGel, "detector");
    G4LogicalVolume* logicDet_inside_side = new G4LogicalVolume(solidDet_Inside_side,fGel,"detector");

    G4Box* solidPolySide = new G4Box('PolySide',(0.007*mm)/2,(block_dy-0.2*mm)/2,block_dz/2);
    G4Box* solidPolyTop = new G4Box('PolyTop',(block_dx-0.2*mm)/2,0.007*mm/2,block_dz/2);
    G4LogicalVolume* logicPolyTop = new G4LogicalVolume(solidPolyTop, fPolystyrene, "PolyTopLV");
    G4LogicalVolume* logicPolySide = new G4LogicalVolume(solidPolySide,fPolystyrene,"PolySideLV");

    G4Box* solidBaSO4Side = new G4Box('BaSO4Side',(gap_c)/2,block_dy/2,block_dz/2);
    G4Box* solidBaSO4Top = new G4Box('BaSO4Top',block_dx/2,(gap_c)/2,block_dz/2);
    G4LogicalVolume* logicBaSO4Top = new G4LogicalVolume(solidBaSO4Top, fBariumSulfate, "BaSO4TopLV");
    G4LogicalVolume* logicBaSO4Side = new G4LogicalVolume(solidBaSO4Side,fBariumSulfate,"BaSO4SideLV");

    G4Box* solidResin = new G4Box("Resin", cryst_dx/2, cryst_dy/2, 0.150*mm/2);
    G4LogicalVolume* logicResin =  new G4LogicalVolume(solidResin, fGel, "Resin");

    G4Box* solidGlue = new G4Box("Glue", cryst_dx/2, cryst_dy/2, Glue_dz/2);
    G4LogicalVolume* logicGlue =  new G4LogicalVolume(solidGlue, fBGO, "Glue");


    G4OpticalSurface* opDetectorSurface = new G4OpticalSurface("DetectorSurface");
      opDetectorSurface->SetFinish(ground);
    G4OpticalSurface* opPlasticSurface = new G4OpticalSurface("PlasticSurface");
      opPlasticSurface->SetFinish(ground);
    G4OpticalSurface* opCrystalSurface = new G4OpticalSurface("CystalSurface");
      opCrystalSurface->SetFinish(polished);
    G4OpticalSurface* opBaSO4Surface = new G4OpticalSurface("BaSO4Surface");
      opBaSO4Surface->SetType(dielectric_dielectric);
      opBaSO4Surface->SetFinish(polishedfrontpainted);

    G4LogicalSkinSurface* CrystalSurface = new G4LogicalSkinSurface("Crystal Surface", logicCryst, opCrystalSurface);
    G4LogicalSkinSurface* BaSO4Surface_T = new G4LogicalSkinSurface("BaSO4 Surface", logicBaSO4Top, opBaSO4Surface);
    G4LogicalSkinSurface* BaSO4Surface_S = new G4LogicalSkinSurface("BaSO4 Surface", logicBaSO4Side, opBaSO4Surface);
    //Skins and surface code available on github

    G4double ephoton_pmt[4]      = {0.0001*eV, 1*eV, 10*eV, 100*eV};
    G4double reflectivity_pmt[4] = {0,0,0,0};
    G4double efficiency_pmt[4]   = {1.0,1.0,1.0,1.0};

    G4MaterialPropertiesTable *detectorMPT = new G4MaterialPropertiesTable();
    detectorMPT->AddProperty("REFLECTIVITY", ephoton_pmt, reflectivity_pmt, 4);
    detectorMPT->AddProperty("EFFICIENCY",   ephoton_pmt, efficiency_pmt,   4);
    opDetectorSurface->SetMaterialPropertiesTable(detectorMPT);




  G4double phi = 0;
  G4int Bx = (std::cos(phi))*(ring_R1+0.5*cryst_dz);
  G4int By = (std::sin(phi))*(ring_R1+0.5*cryst_dz);// its 0
  G4int ODx = (std::cos(phi))*(ring_R1+cryst_dz+0.5*det_dz);
  G4int ODy = (std::sin(phi))*(ring_R1+cryst_dz+0.5*det_dz);// its 0
  G4int IDx = (std::cos(phi))*(ring_R1+0*det_dz);
  G4int IDy = (std::sin(phi))*(ring_R1+0*det_dz);// its 0

  G4RotationMatrix rotm_crys  = G4RotationMatrix();
  rotm_crys.rotateY(90*deg);
  rotm_crys.rotateZ(phi);

  G4RotationMatrix rotm_out  = G4RotationMatrix();
  rotm_out.rotateY(90*deg);
  rotm_out.rotateZ(0.934*phi);

  G4RotationMatrix rotm_in  = G4RotationMatrix();
  rotm_in.rotateY(90*deg);
  rotm_in.rotateZ(phi);

  for (G4int icrys = 0; icrys < nb_crys ; icrys++){
    for (G4int jcrys = 0; jcrys < nb_crys ; jcrys++){
      G4int CrysCopyNumber = (jcrys + nb_crys*icrys ) ;
      G4ThreeVector position_cryst = G4ThreeVector(Bx,//168.16*mm,// - (3.5-icrys)*det_dx*sin(phi),// +0.5*det_dz,
                                                     By+(3.5-icrys)*1.07*det_dx*cos(phi),
                                                     (3.5-jcrys)*1.07*det_dx); //with rotation, 1.07 to account for 0.2 space between detectors
      G4Transform3D transform_cryst = G4Transform3D(rotm_crys,position_cryst);
      new G4PVPlacement(transform_cryst,             //rotation,position
                          logicCryst,            //its logical volume
                          "crystal",             //its name
                          logicIRing,             //its mother  volume
                          false,                 //no boolean operation
                          CrysCopyNumber,                 //copy number
                          checkOverlaps);       // checking overlaps




    for (G4int  igel = 0; igel < nb_gels-1 ; igel++){
      for (G4int jgel = 0; jgel < nb_gels-1 ; jgel++){

        G4ThreeVector Side_gel_position = G4ThreeVector(Bx,//+0.5*det_dz,//- (3.5-icrys)*det_dx*sin(phi),
                                                     0,
                                                     (4-jgel)*1.07*det_dx+0.0965*mm);//Shift it over from between two cystals to 0.001 mm of air gap, take out for BaSO4

        G4ThreeVector Top_gel_position = G4ThreeVector(Bx,
                                                     (4-igel)*1.07*det_dx*cos(phi)+0.00965*mm,
                                                     0);


        G4Transform3D Side_gel_transform = G4Transform3D(rotm_crys,Side_gel_position);
        G4Transform3D Top_gel_transform = G4Transform3D(rotm_crys,Top_gel_position);


  new G4PVPlacement(Side_gel_transform,
                    logicPolySide,
                    "gelSide",
                    logicIRing,false,igel,checkOverlaps);


new G4PVPlacement(Top_gel_transform,
                  logicPolyTop,"gelTop",
                  logicIRing,
                  false,jgel,checkOverlaps);


    }}//end of gels

    for (G4int  igel = 1; igel < nb_gels ; igel++){
      for (G4int jgel = 1; jgel < nb_gels ; jgel++){

        G4ThreeVector Side_gel_position = G4ThreeVector(Bx,//+0.5*det_dz,//- (3.5-icrys)*det_dx*sin(phi),
                                                     0,
                                                     (4-jgel)*1.07*det_dx-0.0965*mm);

        G4ThreeVector Top_gel_position = G4ThreeVector(Bx,
                                                     (4-igel)*1.07*det_dx*cos(phi)-0.0965*mm,
                                                     0);


        G4Transform3D Side_gel_transform = G4Transform3D(rotm_crys,Side_gel_position);
        G4Transform3D Top_gel_transform = G4Transform3D(rotm_crys,Top_gel_position);


  new G4PVPlacement(Side_gel_transform,
                    logicPolySide,
                    "gelSide",
                    logicIRing,false,igel,checkOverlaps);


new G4PVPlacement(Top_gel_transform,
                  logicPolyTop,"gelTop",
                  logicIRing,
                  false,jgel,checkOverlaps);


    }}

  G4int OutdetCopyNumber = jcrys + nb_crys*icrys;
  G4ThreeVector position_out_det = G4ThreeVector(ODx+0.150*mm+Glue_dz, //- 0.03*cryst_dz,//172.41*mm,//+det_dz- (3.5-icrys)*det_dx*sin(phi),
                                                  ODy+(3.5-icrys)*1.07*det_dx*cos(phi),
                                                  (3.5-jcrys)*1.07*det_dx); //with rotation, 1.07 to account for 0.2 space between detectors
  G4Transform3D transform_out_det = G4Transform3D(rotm_out,position_out_det);

  G4ThreeVector position_out_resin = G4ThreeVector(ring_R1+cryst_dz+Glue_dz+0.075*mm -0.41*mm,//no idea why the 0.41 is nesseary but confirmed with visalizer
                                                  ODy+(3.5-icrys)*1.07*det_dx*cos(phi),
                                                  (3.5-jcrys)*1.07*det_dx);
  G4Transform3D transform_out_resin = G4Transform3D(rotm_out,position_out_resin);

  G4ThreeVector position_out_glue = G4ThreeVector(ring_R1+cryst_dz+Glue_dz/2 -0.41*mm,//no idea why the 0.41 is nesseary but confirmed with visalizer
                                                  ODy+(3.5-icrys)*1.07*det_dx*cos(phi),
                                                  (3.5-jcrys)*1.07*det_dx);
  G4Transform3D transform_out_glue = G4Transform3D(rotm_out,position_out_glue);

	new G4PVPlacement(transform_out_det,             //rotation,position
    		           logicDet_outer,            //its logical volume
		               "detector",             //its name
		               logicIRing,             //its mother  volume
		               false,                 //no boolean operation
		               OutdetCopyNumber,                 //copy number
  		             checkOverlaps);       // checking overlaps
  new G4PVPlacement(transform_out_resin,             //rotation,position
                    logicResin,            //its logical volume
                 		"Resin",             //its name
                 		logicIRing,             //its mother  volume
                 		false,                 //no boolean operation
                 		0,                 //copy number
                    checkOverlaps);       // checking overlaps
  new G4PVPlacement(transform_out_glue,             //rotation,position
                    logicGlue,            //its logical volume
                  	"Glue",             //its name
                  	logicIRing,             //its mother  volume
                  	false,                 //no boolean operation
                  	0,                 //copy number
                    checkOverlaps);       // checking overlaps


  G4int IndetCopyNumber = jcrys + nb_crys*icrys;
  G4ThreeVector position_in_det = G4ThreeVector(IDx+1*det_dz, //- 0.01*cryst_dz,//+0.4*cos(phi)*mm - (3.5-icrys)*det_dx*sin(phi),
            IDy+(3.5-icrys)*1.07*det_dx*cos(phi)+0.4*sin(phi),
            (3.5-jcrys)*1.07*det_dx);
            //with rotation, 1.07 to account for 0.2 space between detectors
  G4Transform3D transform_in_det = G4Transform3D(rotm_in,position_in_det);

/*  new G4PVPlacement(transform_in_det,             //rotation,position
                    logicDet_inner,            //its logical volume
                    "detector",             //its name
                    logicIRing,             //its mother  volume
                    false,                 //no boolean operation
                    IndetCopyNumber,                 //copy number
                    checkOverlaps);       // checking overlaps
*/

  }} // end of cryst

  for (G4int imod_radial = 0; imod_radial < 1 ; imod_radial++){
    G4double mod_phi = (imod_radial-1)*dPhi;
    for (G4int imod_vertical = 0; imod_vertical < 1 ; imod_vertical++){
    G4RotationMatrix rotm  = G4RotationMatrix();
    rotm.rotateZ(0);//mod_phi

    G4ThreeVector position_mod = G4ThreeVector(0,0,0);//std::cos(mod_phi),std::sin(mod_phi), (25.78*imod_vertical)
    G4Transform3D transform_mod = G4Transform3D(rotm,position_mod);
    new G4PVPlacement(transform_mod,                     //no rotation
                    logicIRing,             //its logical volume
                    "IRing",                //its name
                    worldLog,
                    false,
                    imod_vertical,
                    checkOverlaps);        //no boolean operation
    }}//end of mods

    G4VisAttributes* crystVisAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));//white
  //  G4VisAttributes* SiPMVisAttributes = new G4VisAttributes(G4Colour(0.0,1.0,0.0));//green
    G4VisAttributes* DetVisAttributes = new G4VisAttributes(G4Colour(0.0,0.0,1.0));//blue
    G4VisAttributes* PolyVisAttributes = new G4VisAttributes(G4Colour(1.0,0.0,0.0));//red
    G4VisAttributes* InnerDetVisAttributes = new G4VisAttributes(G4Colour(1.0,1.0,0.0));//yellow
    G4VisAttributes* ResinVisAttributes = new G4VisAttributes(G4Colour(0.0,1.0,0.0));//green
    G4VisAttributes* GlueVisAttributes = new G4VisAttributes(G4Colour(1.0,0.0,1.0));//magenta


    G4VisAttributes* LogVisAttributes = new G4VisAttributes(G4Colour(0.0,1.0,1.0));

    logicIRing->SetVisAttributes(G4VisAttributes::Invisible);//LogVisAttributes
    logicPolySide->SetVisAttributes(PolyVisAttributes);
    logicPolyTop->SetVisAttributes(PolyVisAttributes);

      logicCryst->SetVisAttributes(crystVisAttributes);
      logicDet_outer->SetVisAttributes(DetVisAttributes);
      logicDet_side->SetVisAttributes(DetVisAttributes);
      logicDet_top->SetVisAttributes(DetVisAttributes);
      logicDet_inside_side->SetVisAttributes(InnerDetVisAttributes);
      logicDet_inside_top->SetVisAttributes(InnerDetVisAttributes);
      logicResin->SetVisAttributes(ResinVisAttributes);
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
