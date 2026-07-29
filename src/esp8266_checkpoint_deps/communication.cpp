#include "communication.h"

// nb permet de ne pas tomber dans une récursion infinie
void envoiMessage(String message, int id, int nb){
  if(nb >= 3){
    return;
  }

  String requestFormat = "!"; 
  requestFormat += message;
  requestFormat += " #";
  requestFormat += String(id);
  client.print(requestFormat);
  client.flush();

  unsigned long debut = millis();

  while(true){
    if(client && client.connected() && client.available()){
      String request = receptionRequest();
      request.trim();

      Serial.print("accusé de reception: "); Serial.println(request);

      if(request == "ok"){
        break;
      }
    }

    if(millis() - debut > 800){
      int repet = nb;
      repet++;
      envoiMessage(message, id, repet);
      break;
    }

    delay(10);
  }
}


void distanceEtCommunication(int id) {
  // Serial.print("mesure distance id ");
  // Serial.println(ids[id]);

  float mesure = mesureDistancePrecise(id);

  // Serial.print("distance: ");
  // Serial.println(mesure);
  if ((distanceNormale[id] - mesure) > 5) {
    // Serial.print("passage détecté, id ");
    // Serial.println(ids[id]);

    // Serial.println("envoie de l'id");
    //bool idRecu = false;

    /*
    client.print("!checkpoint valide");
    String request = "#";
    request += String(ids[id]);
    client.print(request);
    client.flush();
    */
    envoiMessage("checkpoint valide",  ids[id], 0);

    // on éteint la led pour montrer que le checkpoint a été validé 
    digitalWrite(pinLeds[id], LOW);
  }
}

// fait l'action appropriée en fonction du message
void gestionMessages(String request, int id){
  // on cherche à quelle place se trouve l'id concerné dans la liste
  for(int i = 0; i < nbCheckpoints; i++){

    String commande = "debut partie";
    if(commande == request){
      isLedOn[i] = true;
      digitalWrite(pinLeds[i], HIGH);
      continue;
    }

    if(ids[i] == id){
      
      commande = "debut mesure";
      // règle s'il faut mesurer ou non
      if(commande == request){
        doitMesurerDistance[i] = true;

        // pour supprimer ce qui a pu être enregistré avant les mesures
        lectureDonneeAccel();
      } 
      commande = "fin mesure";
      if(commande == request){
        doitMesurerDistance[i] = false;

        // on éteint la led une fois la mesure terminée
        isLedOn[i] = false;
        digitalWrite(pinLeds[i], LOW);
      }

      commande = "mesure distance";
      if(commande == request){
        mesureDistanceDepart();
      }

      commande = "changement id";
      if(commande == request){
        delay(1000);
        
        int newId = receptionRequest().toInt();
        // pour supprimer le texte inutile dans le buffer
        receptionId();
        int place = getIndiceIntListe(ids, nbCheckpoints, id);

        if(place != -1){
          ids[place] = newId;
        }
      }
    }

    //Serial.println(doitMesurerDistance[i]);
  }
}

// retourne le texte compris entre ! et le prochain retour à la ligne
String receptionRequest(){
  char c;
  String request = "";

  c = client.peek();
  if(c == '!'){
    client.read();
  }

  while (client.available()) {
    // lit le prochain caractère sans le consommer
    c = client.peek();

    if(c == '#'){
      return request;
    }

    if(c == '\n'){
      client.read();
      return request;
    }

    if(c == '!'){
      return request;
    }

    c = client.read();
    request += c;
  }

  return request;
}

// recoit l'id et le retourne
String receptionId(){
  char c;
  String request = "";

  c = client.peek();
  if(c == '#'){
    client.read();
  }

  while (client.available()) {
    // lit le prochain caractère sans le consommer
    c = client.peek();

    if(c == '#'){
      return request;
    }

    if(c == '\n'){
      client.read();
      return request;
    }

    if(c == '!'){
      return request;
    }

    c = client.read();
    request += c;
  }

  return request;
}

// gère la reception des messages et leur gestion ensuite
void receptionMessages(){
  // permet de recevoir les messages du serveur
  if (client && client.connected()) {
    if (client.available()) {      
      // enregistre la commande
      String request = "";
      // enregistre  l'id
      String idRecu = "";

      if(client.peek() == '!'){
        request = receptionRequest();
        Serial.print("request: "); Serial.println(request);
      }
      
      if(client.peek() == '#'){
        idRecu = receptionId();
        Serial.print("id concerné: "); Serial.println(idRecu);

      }

      Serial.println();
      Serial.print("poubelle: ");
      if(client.peek() != '!' && client.peek() != '#'){
        while(client.available()){
          if(client.peek() != '!' && client.peek() != '#'){
            Serial.print((char)client.read());
          } else {
            break;
          }
        }
        
      }

      Serial.println();
      Serial.println();

      request.trim();
      idRecu.trim();

      // Serial.print("request: "); Serial.println(request);
      // Serial.print("id concerné: "); Serial.println(idRecu);

      int id = idRecu.toInt();
      // Serial.println(id);

      
      // on envoie un accusé de réception si on a recu un id qui correspond au notre et une requete
      // pourrait être amélioré en vérifiant que la requête est un truc qui existe
      if(request != "" && getIndiceIntListe(ids, nbCheckpoints, id) != -1){
        client.print("!ok");
      }
      

      gestionMessages(request, id);
    }
  }
}