#include <Arduino.h>

// librairie écran 
#include <LiquidCrystal.h>
#include <stdlib.h>

// librairie infrarouge
#include <IRremote.hpp>
#include <string.h>

// librairie carte sd 
#include <SPI.h>
#include <SD.h>

// pour l'enregistrement des temps en json
#include <ArduinoJson.h>

#include "mega_deps/Checkpoint/Checkpoint.h"
#include "mega_deps/Variables/Variables.h"
#include "mega_deps/Utils/Utils.h"
#include "mega_deps/Distance/Distance.h"
#include "mega_deps/Ecran/Ecran.h"
#include "mega_deps/InfraRouge/InfraRouge.h"
#include "mega_deps/SdCard/SdCard.h"
#include "mega_deps/Serie/Serie.h"
#include "mega_deps/Vrac/Vrac.h"

#include "mega_deps/Joueurs/Team.hpp"
#include "mega_deps/Joueurs/Player.hpp"


Team* t;


void setup() {
  // permet de reset l'esp, utile lors de l'utilisation de reset sur la grosse carte
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, LOW);

  delay(100);
  digitalWrite(resetPin, HIGH);

  delay(1000);

  // communication avec l'ordi
  Serial.begin(115200);
  lcd.begin(16, 2);

  // initialisation récepteur infrarouge
  IrReceiver.begin(11, ENABLE_LED_FEEDBACK);

  // initialisation pins in et out pour le capteur ultrasons
  pinMode(sendPinDist, OUTPUT);
  pinMode(recievePinDist, INPUT);

  initialisationCarteSd();


  // initialise en permettant de choisir des configs pré enregistrées ou de saisir les infos
  Team* team = configOuManuel();

  team->display_data();

  // create files to record times on the SD card and load some data if needed
  creationFichiersCourse(newSession);

  Serial.print("nb joueurs: "); Serial.println(team->get_nb_players());

  // pour enregistrer le touche pressé
  char touche[8] = "None";

  // permet de choisir qui va jouer et de lancer la partie
  // <- et -> pour changer de joueur, start pour lancer
  while(strcmp(touche, "start") != 0){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print((char)255);
    lcd.print(" ");
    lcd.print(team->getActivePlayer()->get_name());

    lcd.setCursor(0, 1);
    lcd.print("play:press start");

    const char* toucheTemp = waitAndGetTouche();
    // on assigne comme ça parce que sinon ca marche pas (vrmt bancale ce langage)
    strcpy(touche, toucheTemp);

    // fonction supprimée, voir quoi faire 
    // choixJoueurActif(touche);
    if(strcmp(touche, "droite") == 0){
    team->nextActivePlayer();
    } 
    if (strcmp(touche, "gauche") == 0){
      team->previousActivePlayer();
    }
  }

  t = team;
  
}

void loop() {
  Team* team = t;

  nbCheckpointsValides = 0;
  bool reglages = false;


  // si on est dans le mode relancer le jeu automatiquement, on a pas besoin d'attendre le passage de la voiture
  if(!autoPlaySolo){
    reglages = waitDebutChrono(team);
  }
  
  if(!reglages){
    // on joue un bruit au lancement si le mode est activé

    
    if(jouerSon){
      tone(buzzerPin, 400, 1000);
    }
    
    // gère le calcul du temps et tout ce qui se passe pendant que la voiture réalise le parcours
    reglages = calculTimeCircuit(team->getActivePlayer());
  }

  // cas où la course ne s'est pas arrêtée pour aller dans les paramètres
  if(!reglages){
    
    
    // on joue un autre bruit à la fin
    if(jouerSon){
      tone(buzzerPin, 1000, 1500);
    }

    idCourse += 1;
  }
  
  
  // signale la fin de la partie et permet de rallumer les leds de tout les checkpoints
  envoieMessage("!debut partie", 0);
  

  // si le joueur est en mode solo en continue, pas d'affichage ni de changement de joueur, on enregistre le temps s'il bat et on relance
  if(autoPlaySolo){
    if(!collision){
      team->getActivePlayer()->update_best(tempsTotal, idCourse);

    }
    
  } else{
    // en cas de collision on enregistre pas le temps 
    if(collision){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Collision avec");
      lcd.setCursor(0, 1);
      lcd.print("checkpt, dommage");

      delay(3000);
    } else { // enregistrement du temps et affichage 

      // cas où on la course ne s'est pas arrêtée pour aller dans les réglages
      if(!reglages){
        endCircuit(team, team->getActivePlayer());
        if(saveTimeOnSd){
          enregistrementTemps(team);
        }
      }
    }
    
    gereAffichageFin(team, team->getActivePlayer());

    // joueur suivant
    team->nextActivePlayer();
  }
}