#ifndef TEMPS_H
#define TEMPS_H

#include "mega_deps/Joueurs/Team.hpp"
#include "mega_deps/Joueurs/Player.hpp"

bool afficheTemps(Team* team);
void displayBestPlayer(Team* team);
void printBest(int col, int ligne, Team* team);
void printTime(int col, int ligne, uint32_t temps);

#endif