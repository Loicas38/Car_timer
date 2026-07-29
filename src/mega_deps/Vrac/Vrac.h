#ifndef VRAC_H
#define VRAC_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "mega_deps/Joueurs/Team.hpp"

void printBest(int col, int ligne, Team* team);
void printTime(int col, int ligne, uint32_t temps);
bool waitDebutChrono(Team* team);
void displayPreviousTime(int col, int ligne, Player* player);
void checkCheckpoint(int checkpointRecu, Player* player);
bool calculTimeCircuit(Player* player);
void endCircuit(Team* team, Player* player);
void reglerTempsMini();
void replaceNewBestByOld(Team* team);
void changeEtatAutoplay();
void changeEtatSon();
void resetTimes(Team* team);
void gererMenus(int menu, Team* team);
void selectionJoueurs(Team* team);
Team* reglagesManuels();
void verifReglagesConfig(Team* team);
Team* afficheEtChargeConfigs();
Team* choixConfigOuReglagesManuels();
Team* configOuManuel();

#endif