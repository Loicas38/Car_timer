#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include <Arduino.h>
#include "mega_deps/Joueurs/Team.hpp"
#include "mega_deps/Joueurs/Player.hpp"

void choixNbCheckpoints();

void ajoutIdListeIds(int id);
void ajoutIdListeIdsOff(int id);
bool suppressionIdListeIds(int id);
void receptionIds();
void acquisitionCheckpoints();
void envoieMessage(String message, int id);
int choixCheckpoint();
void mesureDistanceCheckpoint();
int choixNewId(int currentId);
void changerIdCheckpoint();
void eteindreCheckpoint(int id);
void allumeCheckpoint(int id);
void OnOffCheckpoint();
void modeLedCheckpoints();
void reglagesCheckpoints(Player* player);

#endif