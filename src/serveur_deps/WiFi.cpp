#include "WiFi.hpp"

// retourne le texte compris entre ! et le prochain retour à la ligne
// indice correspond à l'emplacement du client dans la liste des clients
String receptionRequest(int indice){
  char c;
  String request = "";

  c = clients[indice].peek();
  if(c == '!'){
    clients[indice].read();
  } else {
    return request;
  }

  while (clients[indice].available()) {
    // lit le prochain caractère sans le consommer
    c = clients[indice].peek();

    if(c == '#'){
      return request;
    }

    if(c == '\n'){
      clients[indice].read();
      return request;
    }

    if(c == '!'){
      return request;
    }

    c = clients[indice].read();
    request += c;
  }

  return request;
}

// recoit l'id et le retourne
// indice correspond à l'emplacement du client dans la liste des clients
String receptionId(int indice){
  char c;
  String request = "";

  c = clients[indice].peek();
  if(c == '#'){
    clients[indice].read();
  }

  while (clients[indice].available()) {
    // lit le prochain caractère sans le consommer
    c = clients[indice].peek();

    if(c == '#'){
      return request;
    }

    if(c == '\n'){
      clients[indice].read();
      return request;
    }

    if(c == '!'){
      return request;
    }

    c = clients[indice].read();
    request += c;
  }

  return request;
}


// gère la reception des messages et leur gestion ensuite
// indice correspond à l'emplacement du client dans la liste des clients
void receptionMessagesWifi(int indice){
  // permet de recevoir les messages du serveur
  if (clients[indice] && clients[indice].connected()) {
    if (clients[indice].available()) {
      // pour laisser le temps à tout d'arriver 
      delay(100);
      
      // enregistre la commande
      String request = "";
      // enregistre  l'id
      String idRecu = "";

      if(clients[indice].peek() == '!'){
        request = receptionRequest(indice);
      }

      delay(50);
      
      if(clients[indice].peek() == '#'){
        idRecu = receptionId(indice);
      }
      
      if(clients[indice].peek() != '!' && clients[indice].peek() != '#'){
        while(clients[indice].available()){
          if(clients[indice].peek() != '!' && clients[indice].peek() != '#'){
            clients[indice].read();
          } else {
            break;
          }
        }
        
      }

      request.trim();
      idRecu.trim();

      
      // Serial.print("request: "); Serial.println(request);
      // Serial.print("id concerné: "); Serial.println(idRecu);

      int id = idRecu.toInt();
      // Serial.println(id);

      // on envoie un accusé de réception si on a recu un id qui correspond au notre et une requete
      // pourrait être amélioré en vérifiant que la requête est un truc qui existe
      if(request != "" && id >= 0){
        clients[indice].print("!ok");
      }

      gestionMessages(request, id, indice);
    }
  }
}


// transmission des messages recus du checkpoint à l'unité centrale
// sauf si c'est un nouveau checkpoint qui vient de se connecter
void gestionMessages(String request, int indice, int connexion){

  // enregistre le nouveau checkpoint
  if(request == "envoie id"){
    // Serial.println("enregistrement de l'id");
    enregistreCheckpoint(indice, connexion);
  }

  String requette = "!";
  requette += request;
  requette += " #";
  requette += String(indice);

  Serial.println(requette);
}