#ifndef SERIE_H
#define SERIE_H

#include <Arduino.h>
#include "mega_deps/Joueurs/Player.hpp"

void gestionRequetes(String request, int id, Player* player);
String receptionSerie(char caractere);
String receptionCommande();
String receptionId();
int receptionQuantite();
void gereReceptionMessages(Player* player);

#endif