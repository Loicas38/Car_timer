#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "serveur_deps/utils.hpp"
#include "serveur_deps/WiFi.hpp"
#include "serveur_deps/Serie.hpp"

void setup() {
  Serial.begin(115200);
  delay(10);

  // on attend que le contrôleur initialise la communication
  while (true) {
    if (Serial.available()) {
      String message = Serial.readStringUntil('\n');
      message.trim();
      if (message == "ready?") {
        break;
      }
    }
    yield();  // Ajout de yield pour éviter le watchdog reset
  }

  int nbCheckpointsRecherches;

  // on attend que le controlleur envoie le nombre de checkpoints recherchés
  while (true) {
    // on demande le nombre de checkpoints
    Serial.println("wait nb checkpoints");

    if (Serial.available()) {
      String message = Serial.readStringUntil('\n');
      message.trim();
      nbCheckpointsRecherches = message.toInt();
      break;
    }

    yield();  // Ajout de yield pour éviter le watchdog reset

    delay(500);
  }

  // Démarrer le mode Soft AP
  WiFi.softAP(ssid, password);

  // Démarrer le serveur
  server.begin();


  // on attend qu'autant de checkpoints différents se connectent qu'il en a été demandé
  while (nbCheckpointsTrouves < nbCheckpointsRecherches) {
    WiFiClient client = server.available();
    if (client) {
      // ajouter le nouveau client à la liste
      for (int i = 0; i < nbMaxClients; i++) {
        if (!clients[i]) {
          clients[i] = client;
          break;
        }
      }
    }

    // gérer les clients existants
    for (int i = 0; i < nbMaxClients; i++) {
      // vérifie si un client existe et s'il est connecté
      if (clients[i] && clients[i].connected()) {
        // cas où il a envoyé quelque chose
        if (clients[i].available()) {
          
          String request = receptionRequest(i);
          int idRecu;
          request.trim();

          if(request == "envoie id"){
            delay(50);
            while(true){
              if(clients[i] && clients[i].connected() && clients[i].available()){
                // Serial.println(clients[i].peek());
                if(clients[i].peek() == '#'){
                  break;
                }else{
                  clients[i].read();
                }
              }

              delay(50);
            }

            String idString = receptionId(i);
            idRecu = idString.toInt();

           // Serial.print("id du checkpt: "); Serial.println(idString);
          } else {
            continue;
          }

          // enregistre le checkpoint si c'est un nouveau 
          if(enregistreCheckpoint(idRecu, i)){
            nbCheckpointsTrouves++;
          }
        }
        yield();  // évite le watchdog reset
      } else if (clients[i]) {
        // déconnecter le client s'il n'est plus connecté
        clients[i].stop();
        clients[i] = WiFiClient();
      }
    }
    yield();  // évite le watchdog reset
  }

  // on prévient la carte qu'on va lui envoyer les id qu'on a trouvé
  Serial.println("!envoie ID");

  delay(10);

  // on lui dit le nombre qu'on va lui en envoyer
  Serial.print("="); Serial.println(nbCheckpointsTrouves);

  // on les envoie
  for (int i = 0; i < nbCheckpointsTrouves; i++) {
    // Serial.print(i);
    Serial.print("#"); Serial.println(listeCheckpoints[i]);
  }

  // Serial.print("fin boucle");

}

void loop() {
  // Vérifier si un nouveau client est connecté
  WiFiClient client = server.available();

  // recoit les commandes série et transmet aux checkpoints si besoin
  if (Serial.available()) {
    gereReceptionSerial();
  }

  if (client) {
    // Serial.println("client connecté");
    // ajouter le nouveau client à la liste
    for (int i = 0; i < nbMaxClients; i++) {
      if (!clients[i]) {
        clients[i] = client;
        break;
      }
    }
  }

  // recoit les messages des checkpoints
  for (int i = 0; i < nbMaxClients; i++) {
    if (clients[i] && clients[i].connected()) {
      if (clients[i].available()) {
        receptionMessagesWifi(i);        
      }

    } else if (clients[i]) {
      // déconnecter le client s'il n'est plus connecté
      clients[i].stop();
      clients[i] = WiFiClient();
    }
  }
}