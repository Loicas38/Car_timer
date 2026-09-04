// boutons télécommande

#include "Variables.h"
#include <LiquidCrystal.h>

const int codes_lst[NB_TOUCHES] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
const char touches_lst[NB_TOUCHES][8] = {"A", "haut", "B", "gauche", "ok", "droite", "start", "bas", "retour"};


// *********************************************** PINS ***************************************************
//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; 

// distance de l'obstacle en face du capteur
float distanceNormal = -1;
// distance en cm du déclenchement de début et fin chrono
int distanceDeclenchement = 10;

// chrono du circuit, et temps minimum correspond au temps en dessous duquel le chrono n'est pas pris en compte
unsigned long tempsDebut=0, tempsTotal=0, bestTime=0;
unsigned long tempsMinimum = 0;


// port du buzzer

bool jouerSon = false;

// permet de jouer en continu quand on est en solo, le chrono s'arrête et se redélenche tout seul
bool autoPlaySolo = false;

// enregistre si le temps s'est arrêté à cause d'une collision ou non
bool collision = false;

// enregistre si la carte sd fonctionne
bool carteSd;

// régit s'il faut ou non enregistrer les temps sur la carte SD
bool saveTimeOnSd = false;

// stocke le numéro de la course
int idCourse = 0;

// chemin où se trouvent les fichiers des temps
String cheminTemps;


// record if the times which are going to be done will be consider to be in the same session than the times which have been done
// last time we played
bool newSession = false;