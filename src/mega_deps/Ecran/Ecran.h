#ifndef ECRAN_H
#define ECRAN_H

#include <Arduino.h>
#include "mega_deps/Joueurs/Team.hpp"
#include "mega_deps/Joueurs/Player.hpp"

String creationChaine(int nbMaxChar);
int choixMenu(String menus[], int longueur, String toucheEntree, String toucheEchap);
void displayBestPlayer();
bool afficheTemps(Team* team);
bool afficheCommandes(Team* team, Player* player);
void gereAffichageFin(Team* team, Player* player);

void affichageClignotant(String message1, int col1, String message2, int col2, int nbClignotements, int delai_display, int delai_entre_clignotements);
// surcharge qui fait appel à la fonction affichant les temps 
void affichageClignotant(String message1, int col1, uint32_t message2, int col2, int nbClignotements, int delai_display, int delai_entre_clignotements);


#endif