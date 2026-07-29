// ************************************************************** checkpoints ************************************************************************
#include "Checkpoint.h"
#include <LiquidCrystal.h>
#define USE_IRREMOTE_HPP_AS_PLAIN_INCLUDE
#include <IRremote.hpp>
#include "Utils.h"
#include "Variables.h"
#include "InfraRouge.h"
#include "Serie.h"
#include "Ecran.h"

// permet de choisir le nombre de checkpoints utilisés
void choixNbCheckpoints(){
  nbCheckpoints = 0;
  char touchePressee[8] = "fb";

  while(strcmp(touchePressee, "A") != 0){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("nb checkpoints: ");
    lcd.setCursor(0, 1);
    lcd.print(nbCheckpoints);

    const char* toucheTemp = waitAndGetTouche();
    strcpy(touchePressee, toucheTemp);

    if(strcmp(touchePressee, "haut") == 0){
      nbCheckpoints++;
    }
    if(strcmp(touchePressee, "bas") == 0){
      nbCheckpoints--;
      if(nbCheckpoints < 0){
        nbCheckpoints = 0;
      }
    }

    //Serial.println(touchePressee);
  }
}


// ajoute l'id dans la liste des ids uniquement s'il y a une place libre et qu'il n'y est pas déjà
// le tri n'est pas effectué
void ajoutIdListeIds(int id){
  int place = -1;


  for(int i = 0; i < nbMaxCheckpoints; i++){
    // on ajoute le checkpoint à la première place libre
    if(ordreCheckpoints[i] == 99 || place == -1){
      place = i;
    }

    // on  arrête d'essayer de l'ajouter si on le trouve dans la liste
    if(ordreCheckpoints[i] == id){
      place = -1;
      break;
    }
  }

  // si place vaut -1, trop de checkpoints ou déjà dans la liste
  if(place != -1){
    ordreCheckpoints[place] = id;
  }else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("checkpt ");
    lcd.print(id);
    lcd.print(" not");
    lcd.setCursor(0, 1);
    lcd.print("add: trop ou deja");

    delay(3000);
  }
}


// add the id given in the list of the checkpoints off only if it's not already in this list
// it also sorts the list then 
void ajoutIdListeIdsOff(int id){
  if(rechercheInt(checkpointsOff, nbCheckpointsOff, id) == -1){

    Serial.println("ce checkpoint n'existe pas, on l'ajoute");

    for(int i = 0; i < nbMaxCheckpointsOff; i++){
      if(checkpointsOff[i] == 99){
        Serial.println("place libre troiuvée, enregistrement de l'id");

        checkpointsOff[i] = id;
        nbCheckpointsOff++;

        // on trie les checkpoints off dans l'ordre croissant
        int n = sizeof(checkpointsOff) / sizeof(checkpointsOff[0]);
        insertionSort(checkpointsOff, n);

        return;
      }
    }
  }
  
}


// permet de supprimer un id de la liste des ids
// trie ensuite la liste des ids pour la remettre dans l'ordre
// retourne true si l'id a été trouvé et supprimé, sinon false
bool suppressionIdListeIds(int id){
  for(int i = 0; i < nbMaxCheckpoints; i++){
    if(ordreCheckpoints[i] == id){
      ordreCheckpoints[i] = 99;

      nbCheckpoints--;

      // on trie les checkpoints dans l'ordre croissant
      int n = sizeof(ordreCheckpoints) / sizeof(ordreCheckpoints[0]);
      insertionSort(ordreCheckpoints, n);

      return true;
    }
  }

  return false;
}


// permet de recevoir le nombre d'id transmis par le serveur et de les AJOUTER à la liste des ids, puis la trie
// ne suppprime pas les ids déjà enregistrés
// n'enregistre pas d'id en double (heureusement)
void receptionIds(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("attente liste ID");


  // attend l'annonce du début de l'envoi des ids
  while(true){
    if(Serial.available()){

      // Serial.println((char)Serial.peek());
      if(Serial.peek() == '!'){
        String request = receptionCommande();
        request.trim();

        Serial.print("request: "); Serial.println(request);
        if(request == "envoie ID"){
          break;
        }
      } else {
        Serial.print("poubelle: ");
        Serial.println((char)Serial.read());
      }

      
    }

    delay(50);
  }

  Serial.println("attente du nombre d'ids qui arrrivent");

  Serial.print("ce qui suit: ");


  Serial.println();

  int quantite;

  // recoit le nombre de checkpoints à recevoir
  while(true){
    // Serial.print("peek: "); Serial.println(Serial.peek());
    if(Serial.available()){
      if(Serial.peek() == '='){
        quantite = receptionQuantite();
        Serial.print("nb d'ids: "); Serial.println(quantite);
        break;
      }else {
        Serial.print("poubelle: ");
        Serial.println(Serial.read());
      }
    }

    delay(10);
  }

  Serial.println("debut reception des ids");


  for(int i = 0; i < quantite; i++){
    // attends la réception d'un id
    while(true){
      if(Serial.available()){
        if(Serial.peek() == '#'){
          break;
        } else {
          Serial.read();
        }
      }
    }

    // ajoute l'id trouvé dans la liste où il y a de la place et uniquement s'il n'y est pas déjà
    ajoutIdListeIds(receptionId().toInt());
  }


  // on trie les checkpoints dans l'ordre croissant
  int n = sizeof(ordreCheckpoints) / sizeof(ordreCheckpoints[0]);
  insertionSort(ordreCheckpoints, n);


  lcd.clear();
  lcd.setCursor(0, 0);
  for(int i = 0; i < nbCheckpoints; i++){
    lcd.print(ordreCheckpoints[i]);
    lcd.print(", ");
  }

  delay(2000);
}


// permet d'acquérir l'id des checkpoints allumés. 
// S'il y a plus de checkpoints allumés que d'utilisés, premier arrivé premier servi
void acquisitionCheckpoints(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("envoie ready?");


  while(true){
    if (Serial.available()) {
      String message = Serial.readStringUntil('\n');
      // on enlève tout les caractères à la con
      message.trim();


      if(message == "wait nb checkpoints"){
        break;
      } else {
        Serial.println("ready?");
      }
    } else {
      Serial.println("ready?");
    }

    delay(500);
    yield();
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("envoie nb checkpts");
  lcd.setCursor(0, 1);
  lcd.print(nbCheckpoints);

  // envoie du nombre de checkpoints
  // Serial.print("nb ceckpoints: "); 
  Serial.println(nbCheckpoints);

  delay(2000);

  receptionIds();

}

// envoie le message passé au checkpoint concerné. 
// si id = 0, le message est envoyé à tout les checkpoints
void envoieMessage(String message, int id){
  // envoie des messages
  // dans ce cas, tout les checkpoints sont concernés
  if(id == 0){
    for(int i = 0; i < nbCheckpoints; i++){
      Serial.println(message);
      String indice = "#";
      indice += String(ordreCheckpoints[i]);
      Serial.println(indice);
    }
  } else { // et ici un seul
    // dit au serveur de demander à ce checkpoint de refaire sa mesure
    Serial.println(message);
    String indice = "#";
    indice += String(id);
    Serial.println(indice);
  }
}

// permet à l'utilisateur de choisir le checkpoint concerné par la commande
// retourne -1 si rien n'a été choisi et 0 si tout les checkpoints ont été choisi.
// Sinon retourne l'id du checkpoint dans le tableau ordreCheckpoint, en comptant le premier élément à l'indice 0
int choixCheckpoint(){
  String menu[nbCheckpoints + 1] = {"all"};

  for (int i = 0; i < nbCheckpoints; i++){
    menu[i+1] = String(ordreCheckpoints[i]);
  }

  int commande = choixMenu(menu, nbCheckpoints + 1, "reglage", "A");

  return commande;
}


// demande au checkpoint de refaire la mesure de distance de départ
void mesureDistanceCheckpoint(){
  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.print("choix du chekpt");
  lcd.setCursor(0, 1);
  lcd.print("a mesurer");


  int commande = choixCheckpoint();

  if(commande == -1){
    return;
  }

  if(commande == 0){
    envoieMessage("!mesure distance", 0);
  } else {
    envoieMessage("!mesure distance", ordreCheckpoints[commande-1]);
  }
  
}

// permet de choisir un nouvel id pour un checkpoint déjà connecté
// retourne -1 si rien n'a été choisie, sinon le  nouvel id (pouvant ne pas avoir changé)
int choixNewId(int currentId){

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("id actuel: ");
  lcd.print(currentId);

  lcd.setCursor(0, 1);
  lcd.print("new id: ");

  int newId = currentId;

  while(true){
    lcd.setCursor(8, 1);
    lcd.print(newId);

    const char* touche = waitAndGetTouche();

    if(strcmp(touche, "A") == 0){
      return -1;
    }

    if(strcmp(touche, "reglage") == 0){
      if(newId != 0){
        return newId;
      }
    }

    if(strcmp(touche, "haut") == 0){
      newId++;

      // on ne doit pas avoir deux id identiques
      while(rechercheInt(ordreCheckpoints, nbCheckpoints, newId) != -1){
        if(newId == currentId){
          break;
        }
        newId++;
      }

      if(newId > 90){
        newId = currentId;
      }
      
    }

    if(strcmp(touche, "bas") == 0){
      newId--;

      // on ne doit pas avoir deux id identiques
      while(rechercheInt(ordreCheckpoints, nbCheckpoints, newId) != -1){
        newId--;
      }

      if(newId < 1){
        newId = currentId;
      }
      
    }
  }
}


// permet de changer l'id d'un checkpoint. Empeche d'avoir 2 ids identiques
void changerIdCheckpoint(){
  int commande = choixCheckpoint();

  Serial.print("id choisi: "); Serial.println(commande);

  int debut;
  int fin;
  
  if(commande == -1){
    return;
  }
  if(commande == 0){
    debut = 0;
    fin = nbCheckpoints;
  } else {
    debut = commande - 1;
    fin = commande;
  }

  for(int i = debut; i < fin; i++){

    Serial.print("i: "); Serial.println(i);
    Serial.print("ID correspondant: "); Serial.println(ordreCheckpoints[i]);
    int newId = choixNewId(ordreCheckpoints[i]);

    if(newId == -1 || newId == ordreCheckpoints[i]){
      continue;
    }

    Serial.print("id concerné: "); Serial.println(ordreCheckpoints[i]);
    envoieMessage("!changement id", ordreCheckpoints[i]);
    String strNewId = "!";
    strNewId += String(newId);
    envoieMessage(strNewId, ordreCheckpoints[i]);
  }

}

// passe le checkpoint passé en paramètre dans la liste des checkpoints éteint et éteint sa diode
void eteindreCheckpoint(int id){
  // si l'id n'a pas pu ^^etre supprimé car inexistant, on s'arrête ici
  if(!suppressionIdListeIds(id)){
    return;
  }

  for( int i = 0; i < nbMaxCheckpointsOff; i++){
    if(checkpointsOff[i] == 99){
      checkpointsOff[i] = id;
      nbCheckpointsOff++;

      // on trie les checkpoints off dans l'ordre croissant
      int n = sizeof(checkpointsOff) / sizeof(checkpointsOff[0]);
      insertionSort(checkpointsOff, n);

      break;
    }
  }

  // permet d'arrêter la diode du checkpoint qui vient d'être éteint
  envoieMessage("!fin mesure", id);

}


// passe le checkpoint passé en paramètre de la liste des checkpoints éteint à celle de ceux allumés
void allumeCheckpoint(int id){
  Serial.println("checkpoints off: ");

  for(int i = 0; i < nbMaxCheckpointsOff; i++){
    Serial.println(checkpointsOff[i]);

    if(checkpointsOff[i] == id){
      Serial.println("id trouvé !");
      checkpointsOff[i] = 99;
      nbCheckpointsOff--;
      nbCheckpoints++;

      // permet d'arrêter la diode du checkpoint qui vient d'être éteint
      envoieMessage("!debut partie", id);

      ajoutIdListeIds(id);

      // on trie les checkpoints off dans l'ordre croissant
      int n = sizeof(checkpointsOff) / sizeof(checkpointsOff[0]);
      insertionSort(checkpointsOff, n);

      // on trie les checkpoints dans l'ordre croissant
      n = sizeof(ordreCheckpoints) / sizeof(ordreCheckpoints[0]);
      insertionSort(ordreCheckpoints, n);
    }
  }
}


// met en service / hors service un checkpoint. 
// le chcekpoint n'est pas allumé et éteint physiquement, c'est juste qu'il va compter ou non pendant les courses
void OnOffCheckpoint(){
  String menu[nbCheckpoints + nbCheckpointsOff];

  Serial.println();
  Serial.print("options: ");
  for (int i = 0; i < nbCheckpoints; i++){
    menu[i] = String(ordreCheckpoints[i]);
    menu[i] += " is on";
    Serial.print(menu[i]);
    Serial.print(", ");
  }

  for(int i = 0; i < nbCheckpointsOff; i++){
    menu[i + nbCheckpoints] = String(checkpointsOff[i]);
    menu[i + nbCheckpoints] += " is off";
    Serial.print(menu[i + nbCheckpoints]);
    Serial.print(", ");
  }

  Serial.println();
  Serial.print("menus: ");
  for(int i = 0; i < (nbCheckpoints + nbCheckpointsOff); i++){
    Serial.print(menu[i]);
  }

  Serial.println();

  Serial.println();

  Serial.print("nb on: ");Serial.println(nbCheckpoints);
  Serial.print("nb off: "); Serial.println(nbCheckpointsOff);

  int commande = choixMenu(menu, nbCheckpoints + nbCheckpointsOff, "reglage", "A");

  if(commande == -1){
    return;
  }

  if(commande < nbCheckpoints){
    eteindreCheckpoint(ordreCheckpoints[commande]);

  } else {
    allumeCheckpoint(checkpointsOff[commande - nbCheckpoints]);
  }

  OnOffCheckpoint();
}

void modeLedCheckpoints(){

}

// permet d'allumer et éteindre les checkpoints, de changer leur id, refaire la mesure de distance, ...
void reglagesCheckpoints(Player* player){
  String menus[] = {"changer un id", "mesure dist debut", "on / off checkpt", "mode leds"};

  int commande = 0;

  while(commande != -1){
    commande = choixMenu(menus, 4, "reglage", "A");

    // vérifie s'il y a des messages série et gère l'action a réalisé s'il y en a 
    gereReceptionMessages(player);

    switch(commande){
      case -1:
        return;

      case 0:
        changerIdCheckpoint();
        break;
      
      case 1:
      // demande au checkpoint de refaire la mesure de distance de départ
        mesureDistanceCheckpoint();
        break;

      case 2:
        OnOffCheckpoint();
        break;

      case 3:
        //choixCheckpointEtEnvoie("!mode leds");
        break;
    }
  }
  
  
}