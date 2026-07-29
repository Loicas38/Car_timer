#include <Arduino.h>
#include <LiquidCrystal.h>

// boutons télécommande

const int NB_TOUCHES = 26;
extern const int codes_lst[NB_TOUCHES];
extern const char touches_lst[NB_TOUCHES][8] ;


// *********************************************** PINS ***************************************************
// initialisation écran 
const int rs = 41, en = 43, d4 = 47, d5 = 49, d6 = 40, d7 = 45;
extern LiquidCrystal lcd;

// broche qui envoie et reçoit au module ultra sons
const int sendPinDist = 44;
const int recievePinDist = 46;

// broche où est conecté le pin reset de l'esp
const int resetPin = 38;

const int buzzerPin = 7;

// Chip Select pin for the SD card reader
const int chipSelectPin = 53;

// ***********************************************************************************************************

// nombre de mesures à effectuer par le capteur ultrrason pour définir la distance au début 
const int nbVals = 30;

const int nbMesureRapide = 7;
const int milieuMesureRapide = 3;

// distance de l'obstacle en face du capteur
extern float distanceNormal;
// distance en cm du déclenchement de début et fin chrono
extern int distanceDeclenchement;



// chrono du circuit, et temps minimum correspond au temps en dessous duquel le chrono n'est pas pris en compte
extern unsigned long tempsDebut, tempsTotal, bestTime;
extern unsigned long tempsMinimum;

const int nbMaxUtilisateurs = 10;
// liste des utilisateurs habituels
extern String utilisateurs[nbMaxUtilisateurs];
// nombre d'utilisateurs enregistrés
extern int nbUtilisateurs;


// port du buzzer

extern bool jouerSon;

// nombre total de checkpoints
// -1 sert à savoir si une valeur a été assignée plus tard dans le programme
extern int nbCheckpoints;
// nombre de checkpoints que la voiture a déjà passés
extern int nbCheckpointsValides;
// nombre max de checkpoint 
const int nbMaxCheckpoints = 10;
// ordre des checkpoints
extern int ordreCheckpoints[nbMaxCheckpoints];

// enregistre les checkpoints connectés mais non utilisés
extern int nbCheckpointsOff;
const int nbMaxCheckpointsOff = 10;
extern int checkpointsOff[nbMaxCheckpointsOff];

// permet de jouer en continu quand on est en solo, le chrono s'arrête et se redélenche tout seul
extern bool autoPlaySolo;

// enregistre si le temps s'est arrêté à cause d'une collision ou non
extern bool collision;

// enregistre si la carte sd fonctionne
extern bool carteSd;

// régit s'il faut ou non enregistrer les temps sur la carte SD
extern bool saveTimeOnSd;

// stocke le numéro de la course
extern int idCourse;

// chemin où se trouvent les fichiers des temps
extern String cheminTemps;


// record if the times which are going to be done will be consider to be in the same session than the times which have been done
// last time we played
extern bool newSession;