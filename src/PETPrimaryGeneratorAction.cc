//Kyle Klein
#include "PETPrimaryGeneratorAction.hh"

PETPrimaryGeneratorAction::PETPrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(), fParticleGun(0) {
  G4int n_particle = 1;

  fParticleGun = new G4ParticleGun(n_particle);
  G4ParticleTable * particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "gamma");
  fParticleGun->SetParticleDefinition(particle);
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1, 0, 0.0));
  fParticleGun->SetParticleEnergy(0.511 * MeV);
  //fParticleGun->SetParticleTime(100);

  tParticleGun = new G4ParticleGun(n_particle);
  tParticleGun->SetParticleDefinition(particle);
  tParticleGun->SetParticleMomentumDirection(G4ThreeVector(-1, 0, 0.0));
  tParticleGun->SetParticleEnergy(0.511 * MeV);
    //fParticleGun->SetParticleTime(100);

}

PETPrimaryGeneratorAction::~PETPrimaryGeneratorAction() {
  delete fParticleGun;
}

void PETPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {

  //Shoot straight into a crystal
  G4double x0 = 0 * mm;
  G4double y0 = 0 * mm;
  G4double z0 = 0 * mm;
  G4double xMomentumDirection = 1;
  G4double yMomentumDirection = 0;
  G4double zMomentumDirection = 0;
  fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xMomentumDirection, yMomentumDirection, zMomentumDirection)); 
  fParticleGun->GeneratePrimaryVertex(anEvent);
  
  ////Randomize over all solid angle:
  //G4double x0 = 0 * mm;
  //G4double y0 = 0 * mm;
  //G4double z0 = - 50 * mm;
  //G4double theta = CLHEP::twopi * G4UniformRand();
  //G4double phi = CLHEP::pi *acos(1-2* G4UniformRand());
  //G4double xMomentumDirection = sin(phi)*cos(theta);
  //G4double yMomentumDirection = sin(phi)*sin(theta);
  //G4double zMomentumDirection = cos(phi);
  //fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xMomentumDirection, yMomentumDirection, zMomentumDirection)); 
  //fParticleGun->GeneratePrimaryVertex(anEvent);
  //tParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
  //tParticleGun->SetParticleMomentumDirection(G4ThreeVector(-xMomentumDirection, -yMomentumDirection, -zMomentumDirection));
  //tParticleGun->SetParticleTime(1000*ns);
  //tParticleGun->GeneratePrimaryVertex(anEvent);
  
  ////Randomize over central 8x8 array, including their outer gaps:
  //G4double x0 = 0 * mm;
  //G4double y0 = 0.0 * mm;
  //G4double z0 = 0.0 * mm;
  //G4double theta = 0.0714*G4UniformRand();
  //G4double phi = 0.0714 * G4UniformRand();
  //G4double xMomentumDirection = 1;
  //G4double yMomentumDirection = tan(phi);
  //G4double zMomentumDirection = tan(theta);
  //fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xMomentumDirection, yMomentumDirection, zMomentumDirection)); 
  //fParticleGun->GeneratePrimaryVertex(anEvent);
  
  ////Randomize over central 4x4 crystals, not including their outer gaps:
  //G4double xMomentumDirection = 1;
  //G4double yMomentumDirection = ((G4UniformRand() *  (2 * 6.3 / 163.91)) - (6.3 / 163.91));
  //G4double zMomentumDirection = ((G4UniformRand() *  (2 * 6.3 / 163.91)) - (6.3 / 163.91));
  
  //fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xMomentumDirection, yMomentumDirection, zMomentumDirection)); 
  //fParticleGun->GeneratePrimaryVertex(anEvent);
  ////Include if you want back-to-back gammas
  //tParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
  //tParticleGun->SetParticleMomentumDirection(G4ThreeVector(-xMomentumDirection, -yMomentumDirection, -zMomentumDirection));
  //tParticleGun->GeneratePrimaryVertex(anEvent);
  
  ////Shoot back-to-back gammas radially from center with random 2*pi angle
  //G4double x0 = 0 * mm;
  //G4double y0 = 0 * mm;
  //G4double z0 = 1.6 * mm;
  //G4double theta = CLHEP::twopi*G4UniformRand();
  //G4double xMomentumDirection = cos(theta);
  //G4double yMomentumDirection = sin(theta);
  //G4double zMomentumDirection = 0;
  //fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xMomentumDirection, yMomentumDirection, zMomentumDirection)); 
  //fParticleGun->GeneratePrimaryVertex(anEvent);
  //tParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
  //tParticleGun->SetParticleMomentumDirection(G4ThreeVector(-xMomentumDirection, -yMomentumDirection, -zMomentumDirection));
  //tParticleGun->SetParticleTime(1000*ns);
  //tParticleGun->GeneratePrimaryVertex(anEvent);

}
