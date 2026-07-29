// ************************************************* communication série ***************************************************************************

#include "Serie.h"
#include <Arduino.h>
#include "Checkpoint.h"
#include "Variables.h"
#include "Vrac.h"

// effectue l'acion appropriée en fonction de la requête reçue
void gestionRequetes(String request, int id, Player* player){
  Serial.print("requete: "); Serial.println(request);
  Serial.print("id concerné: "); Serial.println(id);

  if(request == "collision"){
    collision = true;
    return;
  }

  if(request == "checkpoint valide"){
    checkCheckpoint(id, player);
  }

  if(request == "suppression checkpoint"){
    suppressionIdListeIds(id);
  }

  if(request == "envoie id"){
    Serial.print("nouvel id détecté: "); Serial.println(id);
    ajoutIdListeIdsOff(id);
  }
}


// receives Serial messages
// the message must be after the character which is given 
String receptionSerie(char caractere){
  Serial.println("reception d'une commande: ");
  String request = "";

  char c = Serial.peek();

  if(c != caractere){
    return request;
  }

  Serial.read();

  while(Serial.available()){
    c = Serial.peek();

    Serial.print(c);

    if(c == '!' || c == '#' || c == '\n' || c == '='){
      Serial.print("caractere de fin: "); Serial.println(c);
      return request;
    }

    c = Serial.read();
    request += c;

  }

  Serial.println();Serial.println();
  return request;
}


// gère la reception de la commande et la retourne
String receptionCommande(){
  String request = receptionSerie('!');

  request.trim();
  return request;
}

// gere la reception de l'id et le retourne
String receptionId(){
  String request = receptionSerie('#');

  request.trim();
  return request;
}

// gere la reception de la quantité et la retourne
int receptionQuantite(){
  String request = receptionSerie('=');

  request.trim();
  return request.toInt();
}


// recoit les messages série et fais les appels de fonction nécessaires pour réaliser les actions à faire
void gereReceptionMessages(Player* player){
  if(!Serial.available()){
    return;
  }

  // laisse le temps à tout d'arriver
  delay(10);

  // Serial.println("message serie");

  char c = Serial.peek();
  String request = "";
  String idRecu = "";
  int id = -1;

  if(c == '!'){
    request = receptionCommande();
    request.trim();
  }


  c = Serial.peek();

  // Serial.print(c);

  // supprime tout ce qui ne sert à rien
  if(c != '!' && c != '#'){
    while(Serial.available()){
      Serial.read();
      c = Serial.peek();
      if(c == '!' || c == '#'){
        break;
      }
    }
  }

  if(c == '#'){
    idRecu = receptionId();
    Serial.println(idRecu);
    idRecu.trim();
    id = idRecu.toInt();
  }

  
  c = Serial.peek();
  Serial.print((char)c);

  // supprime tout ce qui ne sert à rien
  if(c != '!' && c != '#'){
    Serial.print("poubelle: ");

    while(Serial.available()){
      Serial.read();
      c = Serial.peek();
      if(c == '!' || c == '#' || c == '='){
        break;
      }
    }

    Serial.println();
    Serial.println();
  }

  

  // Serial.print("commande: "); Serial.println(request);
  // Serial.print("id: "); Serial.println(id);

  // on ne transmet le message au checkpoint que s'il y a une requête, inutile de juste lui envoyer son id
  if(request.length() > 0 && id != -1){
    gestionRequetes(request, id, player);
  }
}