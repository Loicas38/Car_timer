// ************************************************  about time calcul ******************************************************************
#include "Vrac.h"
#include "Variables.h"
#include "Utils.h"
#include "Distance.h"
#define USE_IRREMOTE_HPP_AS_PLAIN_INCLUDE
#include <IRremote.hpp>
#include "InfraRouge.h"
#include "Serie.h"
#include "Checkpoint.h"
#include "Ecran.h"
#include "SdCard.h"
#include "Team.hpp"
#include "Player.hpp"


// affiche le meilleur temps sur l'écran
void printBest(int col, int ligne, Team* team){
  printTime(col, ligne, team->get_best_time());
}

// affiche le temps donné ou souhaité sur l'écran 
void printTime(int col, int ligne, uint32_t temps){
  lcd.setCursor(col, ligne);

  if (temps >= 60000){
    lcd.print(calculMinutes(temps));
    lcd.print("m");
  }

  lcd.print(calculSecondes(temps));
  lcd.print("s");
  lcd.print(calculMilli(temps));
  lcd.print("ms");
}


// ************************************************** about chrono  *****************************************************************************


// attend pour lancer le chrono
// retourne true si l'utilisateur veut aller dans les paramètres
bool waitDebutChrono(Team* team){

  // suppression des anciens trucs reçus
  IrReceiver.decodedIRData.command = 0;
  IrReceiver.resume();

  float distance = mesureDistancePrecise();
  while( (distanceNormal - distance) < distanceDeclenchement){

    lcd.clear();
    lcd.setCursor(0, 0);

    if(bestTime != 0){
      lcd.print("Best ");
      printBest(6, 0, team);
    } else {
      // affichage du nom du joueur qui va jouer
      lcd.setCursor(0, 0);
      lcd.print((char)255);
      lcd.print(" ");
      lcd.print(team->getActivePlayer()->get_name());
    }

    lcd.setCursor(0, 1);
    lcd.print("  Wait for car");


    distance = mesureDistancePrecise();

    // pour détecter une touche cliquée et relancer l'attente
    if (IrReceiver.decode()) {
      IrReceiver.resume();
      const char* touchePresse = getNomTouche(IrReceiver.decodedIRData.command);

      if(strcmp(touchePresse, "reglage") == 0){
        return true;
      }

      // permet de changer le joueur
      team->nextActivePlayer();

      IrReceiver.decodedIRData.command = 0;

    }

    delay(10);
  }

  IrReceiver.resume();

  return false;
}

// affiche le temps précédent du gars passé en paramètre où demandé
void displayPreviousTime(int col, int ligne, Player* player){
  printTime(col, ligne, player->get_last_time());
}

// vérifie que le checkpoint pass en paramètre est bien le suivant
void checkCheckpoint(int checkpointRecu, Player* player){

  // Serial.println(message);
  // Serial.println(checkpointRecu);
  
  // on vérifie que le checkpoint qui vient d'être placé est bien le suivant
  if(ordreCheckpoints[nbCheckpointsValides] == checkpointRecu){
    player->add_new_time_checkpoint(tempsTotal, nbCheckpointsValides);

    if(jouerSon){
      tone(buzzerPin, 100, 500);
    }

    nbCheckpointsValides++;

    // lance les mesures sur le prochain checkpoint s'il existe
    if(nbCheckpointsValides < nbCheckpoints){
      envoieMessage("!debut mesure", ordreCheckpoints[nbCheckpointsValides]);
    }

    // envoi au serveur de dire à ce checkpoint d'arrêter d'effectuer des mesures
    envoieMessage("!fin mesure", checkpointRecu);

    if(nbCheckpointsValides > nbCheckpoints){
      nbCheckpointsValides = nbCheckpoints;
    }
    
    if(!autoPlaySolo){
      lcd.setCursor(10, 0);
      lcd.print("     ");
      lcd.setCursor(10, 0);
    } else {
      lcd.setCursor(0, 0);
      lcd.print("     ");
      lcd.setCursor(0, 0);
    }
    
    lcd.print(nbCheckpointsValides);
    lcd.print(" / ");
    lcd.print(nbCheckpoints);
  }
}


// calcul le temps du circuit et gère les affichages pendant ce temps
bool calculTimeCircuit(Player* player) {

  if(nbCheckpoints > 0){
    Serial.println("!debut mesure");
    String chaine = "#";
    chaine += String(ordreCheckpoints[0]);
    Serial.println(chaine);
  }


  // enregistre à quel moment a commencé la course
  tempsDebut = millis();

  // permet de "faire le ménage" des messages déjà arrivés 
  // permet de ne pas avoir des checkpoints checker avant même le début de la course
  if (Serial.available()) {
      String message = Serial.readStringUntil('\n');
  }

  lcd.clear();

  // affiche le nombre de chackpoints validés (0 vu que c'est le début)
  // et on affiche le nombre total de checkpoints 
  // on affiche un texte avant que si on est pas en mode solo
  // on garde de la place pour afficher le temps précédent en mode solo
  lcd.setCursor(0, 0);
  if(!autoPlaySolo){
    lcd.print("passages: ");
  }

  lcd.print(nbCheckpointsValides);
  lcd.print(" / ");
  lcd.print(nbCheckpoints);

  lcd.setCursor(0, 1);
  lcd.print("Tps ");


  // affiche le temps précédent si on est en mode autoPlay
  if(autoPlaySolo){
    displayPreviousTime(6, 0, player);
  }

  // sans le delay, la touche A pressée pour relancer le jeu est ncore considérée pressée au début de la course
  delay(500);
  // permet que la dernière touche pressée ne soit pas le A
  IrReceiver.decodedIRData.command = 0;
  

  float distance = mesureDistancePrecise();

  // sans cette ligne, si quelque chose est détecté avant l'entrée dans la boucle while, le chrono s'arrête immédiatement sans tenir compte du temps minimum
  // puisque tempsTotal est encore au temps du joueur précédent 
  tempsTotal = millis() - tempsDebut;

  // pour détecter si une touche est pressée
  // on remet à 0 sinon la touche cliquée précédemment sera prise en compte
  IrReceiver.decodedIRData.command = 0;
  // on relance l'attente d'une commande infrarouge
  IrReceiver.resume();

  // enlève les messages en attente pour que des checkpoints ne soient pas déjà validés au début
  while(Serial.available()) {
    Serial.readStringUntil('\n');
  }

  collision = false;
  while (((distanceNormal - distance) < distanceDeclenchement || nbCheckpointsValides < nbCheckpoints) || tempsTotal < tempsMinimum){

    if(collision){
      // on joue un son plus long que les autres
      if(jouerSon){
        tone(buzzerPin, 100, 3000);
    }
      break;
    }

    // on ne mesure la distance que si tout les checkpoints ont été validés. 
    // inutile sinon, et on utilise de la batterie pour rien
    if(nbCheckpointsValides == nbCheckpoints){
      distance = mesureDistancePrecise();
    }
    
    tempsTotal = millis() - tempsDebut;

    // affiche le temps en cours
    lcd.setCursor(4, 1);
    lcd.print("            ");

    // on affiche le temps en cours
    printTime(4, 1, tempsTotal);

    
    // si la touche A est cliqué on active ou désactive le mode auto play et on arrête le chrono en cours
    if (IrReceiver.decode()) {
      Serial.println(IrReceiver.decodedIRData.command);
      IrReceiver.resume();
      if(strcmp(getNomTouche(IrReceiver.decodedIRData.command), "reglage") == 0){
        tempsTotal = 0;
        IrReceiver.resume();

        Serial.println(IrReceiver.decodedIRData.command);
        return true;
      }

    }

    
    // permet de recevoir les checkpoints passés
    if (Serial.available()) {
      gereReceptionMessages(player);

      lcd.setCursor(14, 0);
      lcd.print(nbCheckpoints);
    }

    delay(50);
  }

  IrReceiver.resume();
  return false;
}

// gère l'enregistrement des temps et l'affichage du temps qui vient d'être fait
void endCircuit(Team* team, Player* player){
  // on enregistre ce temps comme le temps précédent du joueur


  if((tempsTotal < player->getBestTime() || player->get_nb_times_saved() == 0) && tempsTotal != 0){

    // on ajoute le temps ici et pas avant la condition pour pouvoir savoir s'il s'agit d'un record ou non
    player->add_new_time(tempsTotal);

    // stocke si le record est global ou personel, cad si c'est le meilleur temps de la piste ou le meilleur temps de l'utilisateur
    bool globalRecord = false;
    if(team->get_best_time() == tempsTotal){
      globalRecord = true;
    }

    // affichage clignotant pour dire que c'est un record, avec le temps réalisé
    if (globalRecord){
      affichageClignotant(" New gl record !", 0, tempsTotal, 4, 5, 500, 300);
    } else {
      affichageClignotant(" New ps record !", 0, tempsTotal, 4, 5, 500, 300);
    }

  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("yr best ");

    // affiche le meilleur temps
    printTime(8, 0, player->getBestTime());

    // affihe le temps du joueur
    lcd.setCursor(0, 1);
    lcd.print("you: ");
    
    printTime(5, 1, tempsTotal);
    
    // laisse le temps de lire le message et permet de quitter si flemme d'attendre
    waitAndGetTouche(5000);
  }
}

// permet de régler le temps minimum en dessous duquel le chrono ne s'arrête pas 
void reglerTempsMini(){

  char touchePressee[8] = "";

  while(strcmp(touchePressee, "Ok") != 0){
    lcd.clear();

    printTime(0, 0, tempsMinimum);

    const char* toucheTemp = waitAndGetTouche();
    strcpy(touchePressee, toucheTemp);

    if(strcmp(touchePressee, "haut") == 0){
      tempsMinimum = tempsMinimum + 1000;
    }

    if(strcmp(touchePressee, "bas") == 0){
      tempsMinimum = tempsMinimum - 1000;

      if(tempsMinimum < 0){
        tempsMinimum = 0;
      }
    }
  }
  
}




// ***********************************************************  fin du jeu  ***********************************************


// replace the new best time f the user by his old one 
// useful in case of an error in the time calcul
void replaceNewBestByOld(Team* team){

  Player* player = team->getActivePlayer();

  // we display what is going to be done
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("old = now, 1 = Y");
  lcd.setCursor(0, 1);
  lcd.print("for ");
  lcd.print(player->get_name());

  delay(200);
  // the player has the choice to replace the best or not 
  const char* action = waitAndGetTouche();

  // the change is made if it's needed, and the operation which has been done is written
  if(strcmp(action, "1") == 0){
    player->delete_best_time();
    lcd.clear();
    lcd.print("changement ok");
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("annulation");
  }

  delay(2000);
  
}

// allume ou éteint le mode autoplay en fonction de s'il était allumé ou éteint
void changeEtatAutoplay(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("le mode auto est");
  lcd.setCursor(0, 1);
  lcd.print("maintenant ");

  if(autoPlaySolo){
    lcd.print("off");
  } else {
    lcd.print("on");
  }

  autoPlaySolo = !autoPlaySolo;

  // laisse le temps de lire le message
  delay(3000);
}

// le passe de on à off et inversement
void changeEtatSon(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("le son est ");
  lcd.setCursor(0, 1);
  lcd.print("maintenant ");

  if(jouerSon){
    lcd.print("off");
  } else {
    lcd.print("on");
  }
  
  delay(3000);
  jouerSon = !jouerSon;
}

void resetTimes(Team* team){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Reset all times?");
  lcd.setCursor(0, 1);
  lcd.print("1 -> O, all -> N");

  delay(1000);
  const char* action = waitAndGetTouche();
  Serial.println(action);
  if (strcmp(action, "1") == 0){
    team->reset_times();
    lcd.clear();
  }
}

// réalise l'appel à la fonction associée au menu choisie
void gererMenus(int menu, Team* team){
  switch(menu){
    case 1:
    // active ou désactive le mode jouer en continu
      changeEtatAutoplay();
      break;

    case 2:
    // regle le temps minimum avant lequel le chrono ne peut pas s'arrêter
      reglerTempsMini();
      break;

    case 3:
    // allume le son s'il est éteint et inversement
      changeEtatSon();
      break;

    case 4:
    // remplace le meilleur score actuel d'un joueur par son ancien meilleur score
      replaceNewBestByOld(team);
      break;

    case 5:
    // supprime tout les records 
    // n'affecte pas les temps présents sur la carte SD
      resetTimes(team);
      break;

    case 6:
    // permet d'effectuer plusieurs réglages à distance sur les checkpoints'
      reglagesCheckpoints(team->getActivePlayer());
      break;

    case 7:
    // effectue à nouveau la mesure de la distance de départ, utilisée pour détecter le passage de la voiture
      mesureDistanceDepart();
      break;

    case 8:
    // règle la distance de déclenchement. Si la distance du mur en face est de 30 et la distance mini de x, lorsque 
    // la distance mesurée est inférieure à 30 - x on considère que la voiture est passée
      reglerDistanceMini();
      break;

    case 9:
    // permet d'enregistrer une configuration sur la carte SD pour ne pas avoir besoin d'effectuer tout les réglages à chaque nouvelle session
      enregistrementConfig(team);
      break;

    case 10:
    // permet de resélectionner les joueurs et gère les fichiers de course
      selectionJoueurs(team);
      creationFichiersCourse(false);
      break;

    case 11:
    // crée de nouveaux fichiers de temps pour créer une nouvelle session
      creationFichiersCourse(true);
      idCourse = 0;
      break;

    case 12:
    // relance une nouvelle session
      setup();
  }
}




// ***************************************************************  initialisation  ****************************************************************


void selectionJoueurs(Team* team){
  lectureUtilisateursCarteSd();
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A = joue ok= fin");
  lcd.setCursor(0, 1);
  lcd.print("fleche = suivant");

  // attend que l'utilisateur clique sur une touche avant de passer à la sélection des joueurs
  waitAndGetTouche();

  // contiendra la touche pressée
  char touchePressee[8] = "rien";

  // nombre d'invités, pour savoir le numéro à mettre après le texte invite
  int nbInvites = 0;

  // stocke si on est en train de demander si un invité va jouer
  bool invite = false;

  // stocke le nom du joueur affiché
  String joueur;

  // permet de savoir où on en en est dans la sélction
  // si vaut x, on affiche le nom d'utilisateur x
  // si x ne correspond pas à un utilisateur enregistré, on affiche invite et le bon numéro
  int indiceSelection = 0;

  bool joue = false;

  while(((strcmp(touchePressee, "ok") != 0) && (team->is_full())) || (team->get_nb_players() < 1)){
    lcd.clear();
    lcd.setCursor(0, 0);

    if(indiceSelection == -1){
      joueur = "add user";
      invite = false;
    }
    // on choisit soit un joueur enregistré soit un invité en fonction de là ou on en est
    else if((indiceSelection < (sizeof(utilisateurs) / sizeof(utilisateurs[0]))) && utilisateurs[indiceSelection] != ""){
      joueur = utilisateurs[indiceSelection];
      invite = false;
    } else {
      // on crée la chaine invite avec un numéro
      joueur = "invite ";
      joueur += String(nbInvites);
      invite = true;
    }

    lcd.print(joueur);

    // pas besoin d'afficher joue ou joue pas si on est en mode entrer un nouvel utilisateur
    if(indiceSelection != -1){
      // on regarde si le joueur est déja considéré comme jouant
      joue = team->player_in_team(joueur);

      // on affiche si le joueur actuelle est déjà considéré comme jouant ou non
      lcd.setCursor(0, 1);
      if(joue){
        lcd.print("joue");
      } else {
        lcd.print("ne joue pas");
      }
    }

    const char* toucheTemp = waitAndGetTouche();

    Serial.print("touche pressee : "); Serial.println(toucheTemp);

    strcpy(touchePressee, toucheTemp) ;

    // on passe le joueur en mode joueur ou non joueur
    if(strcmp(touchePressee, "Ok") == 0){
      if(indiceSelection == -1){
        if(nbUtilisateurs < nbMaxUtilisateurs){
          String newUser = creationChaine(10);

          // on ajoute qui si on a une vraie chaine de caractères
          if(newUser != ""){
            utilisateurs[nbUtilisateurs] = newUser;
            nbUtilisateurs ++;

            // on enregistre le nouvel utilisateur
            ajoutDonneesFichier(newUser, "USERS.txt");

            // on ajoute le nouveau joueur à ceux qui jouent
            if(!team->is_full()){
              team->add_player(newUser);
            }
          }
        }
      }
      // cas où le joueur sélectionné n'est pas encore dans la liste des jouants
      else if(joue == false){
        // on l'ajoute s'il n'y a pas trop de joueurs
        if(!team->is_full()){

          // Serial.println("va jouer");
          // joueurs[indiceJoueur] = (char*)malloc(11 * sizeof(char));
          team->add_player(joueur);
        }
      } else { // cas où le joueur joue déjà et qu'on l'enlève
        // Serial.println("ne va plus jouer");
        team->delete_player(joueur);
      }
    }

    // défilement à droite
    if(strcmp(touchePressee, "droite") == 0){
      // Serial.println("suivant");

      indiceSelection ++;

      // incrémente le numéro de l'invite lorsqu'on est en mode invité, c'est à dire si on est en train de choisir des invités
      if(invite){
        nbInvites++;

        // pour ne pas aller trop loin
        if(nbInvites > 9){
          indiceSelection = -1;
          nbInvites = 0;
        }
      }
    }

    // défilement à gauche
    if(strcmp(touchePressee, "gauche") == 0){
      // Serial.println("precedent");
      indiceSelection --;

      if(indiceSelection < -1){
        indiceSelection = 10 + nbUtilisateurs;

        nbInvites = 9;
      }

      if(invite){
        nbInvites--;

        if(nbInvites < 0){
          nbInvites = 0;
        }
      }
    }

    delay(200);

  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("fin selection");
}


Team* reglagesManuels(){
  Serial.println("reglages manuels");

  // permet de choisir le nombre de checkpoints
  choixNbCheckpoints();


  if(nbCheckpoints > 0){
    acquisitionCheckpoints();
  }


  lcd.clear();
  lcd.setCursor(0, 0);

  // affichages 
  lcd.print("Dist : ");
  lcd.setCursor(0, 1);
  lcd.print("A -> joueurs");

  delay(100);
  waitClickTouche("Ok", true); 

  Team* team = new Team();

  // sélection de qui jouera
  selectionJoueurs(team);


  lcd.clear();
  lcd.setCursor(0, 0);
  // affichages 
  lcd.print("Dist : ");
  lcd.setCursor(0, 1);
  lcd.print("A -> mesure distance");

  waitClickTouche("Ok", true);

  mesureDistanceDepart();

  return team;
}

// permet de vérifier que la config chargée a bien effectué tout les réglages nécessaires
void verifReglagesConfig(Team* team){
  Serial.println("verif reglages config");

  if(nbCheckpoints == -1){
    // permet de choisir le nombre de checkpoints
    choixNbCheckpoints();
  }

  if(nbCheckpoints > 0){
    acquisitionCheckpoints();
  }

  // test sans la conditions, verra si ça marche 
  if(team->get_nb_players() == 0){
    lcd.clear();
    lcd.setCursor(0, 0);

    // affichages 
    lcd.print("Dist : ");
    lcd.setCursor(0, 1);
    lcd.print("Ok -> joueurs");

    delay(200);
    waitClickTouche("Ok", true); 

    // sélection de qui jouera
    selectionJoueurs(team);
  }

  if(distanceNormal == -1){
    lcd.clear();
    lcd.setCursor(0, 0);
    // affichages 
    lcd.print("Dist : ");
    lcd.setCursor(0, 1);
    lcd.print("A -> mesure distance");

    waitClickTouche("Ok", true);

    mesureDistanceDepart();
  }
}

// liste les configs dispos et permet d'en choisir une
Team* afficheEtChargeConfigs(){
  if(!carteSd){
    return NULL;
  }

  String configurations[5];

  listerFichiers(SD.open("/CONFIG/"), configurations);

  int nbConfigs = 0;

  for(int i = 0; i < 5; i++){
    if(configurations[i] != ""){
      nbConfigs++;
    }
  }

  String menus[nbConfigs];

  for(int i = 0; i < nbConfigs; i++){
    int longueur = configurations[i].length();
    menus[i] = configurations[i].substring(0, longueur - 4);
  }

  int commande = choixMenu(menus, nbConfigs, "ok", "");

  Team* team = ouvertureConfig(configurations[commande]);
  // on utilise configurations[commande] pour avoir l'extension du fichier
  if(team != NULL){
    verifReglagesConfig(team);
  } else {
    team = reglagesManuels();
  }

  return team;
  
}


// permet à l'utilisateur de choisir une config enregistrée ou de saisir les paramètres manuellement
Team* choixConfigOuReglagesManuels(){
  lcd.clear();
  lcd.setCursor(0, 0);

  Serial.println("choix de la config ou du mode manuel");

  String options[2] = {"autres configs", "reglages manuels"};

  int menu = choixMenu(options, 2, "ok", "");

  if(menu == 0){
    return afficheEtChargeConfigs();

  } else {
    return reglagesManuels();
  }

}

// gère le choix des paramètres via une configuration ou un choix manuel
Team* configOuManuel(){
  lcd.clear();
  lcd.setCursor(0, 0);
  
  lcd.print("config Default 5");

  lcd.setCursor(0, 1);
  lcd.print("manuel -> reglage");

  int debut = millis();

  // suppression des anciens trucs reçus
  IrReceiver.decodedIRData.command = 0;
  IrReceiver.resume();

  Serial.println("choix config ou manuel");

  Team* team;

  // durée qui est attendue avant d'utiliser la config par défaut 
  int duree_attente = 5;
  while(true){

    // pour détecter une touche cliquée et relancer l'attente
    if (IrReceiver.decode()) {
      IrReceiver.resume();
      
      if(strcmp(getNomTouche(IrReceiver.decodedIRData.command), "reglage") == 0){
        Serial.println("touche pressée");

        team = choixConfigOuReglagesManuels();
        break;
      }
    }

    lcd.setCursor(15, 0);
    lcd.print(duree_attente - (millis() - debut) / 1000);

    if((duree_attente - (millis() - debut) / 1000) < 1){
      Serial.println("ouverture config default");

      team = ouvertureConfig("DEFAULT.txt");

      if(team != NULL){
        verifReglagesConfig(team);
      } else {
        team = reglagesManuels();
      }
      break;
    }
  }

  IrReceiver.resume();

  return team;
}


/*
void choixJoueurActif(char* touchePressee, Team* team){
  if(strcmp(touchePressee, "droite") == 0){
    team->nextActivePlayer();
    if (joueurActif >= nbJoueurs){
      joueurActif = 0;
    }
  } 
  if (strcmp(touchePressee, "gauche") == 0){
    joueurActif --;
    if(joueurActif < 0){
      joueurActif = nbJoueurs - 1;
    }
  }
}*/