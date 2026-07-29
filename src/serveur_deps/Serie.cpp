#include "Serie.hpp"

// recoit la commande et la retourne, en enlevant le '!'
// la commande s'arrete lorsqu'il y a un '!', un '#' ou un '\n'
String receiveSerialCommande(){
  String request = "";

  char c = Serial.peek();

  if(c != '!'){
    return request;
  }

  Serial.read();

  while(Serial.available()){
    c = Serial.peek();

    if(c == '!' || c == '#' || c == '\n'){
      return request;
    }

    c = Serial.read();
    request += c;

  }

  return request;
}

// recoir l'id et s'arrête dès qu'il y a un retour à la ligne, espace ou autre caractère du genre
String receiveSerialId(){
  String request = "";

  char c = Serial.peek();

  if(c != '#'){
    return request;
  }

  Serial.read();

  while(Serial.available()){
    c = Serial.peek();

    if(c == '!' || c == '#' || c == '\n'){
      return request;
    }

    c = Serial.read();
    request += c;

  }

  return request;
}






// nb correspond au nombre de fois que la fonction a été appelée, pour sortir de la récursion si les checkpoints ne recoivent pas les messages
void sendMessageClient(String message, int indice, int nb){
  Serial.print("nombre de répétitions: "); Serial.println(nb);
  Serial.print("commande: "); Serial.println(message);
  Serial.print("indice concerné: "); Serial.print(indice);

  // pour ne pas tomber dans une récursion infinie
  // le checkpoint est considéré comme ayant un problème et est donc supprimé après trois messages sans réponse
  if(nb >= 3){
    Serial.println("!suppression checkpoint");
    String requestFormat = "#";
    requestFormat += String(indice);
    Serial.print(requestFormat);
    return;
  }

  int indiceClient;

  for(int i = 0; i < nbTotalCheckpoints; i++){
    if(listeCheckpoints[i] == indice){
      indiceClient = correspondanceIdClient[i];
    }
  }

  if(clients[indiceClient].connected()){
    // transmission de la commande 
    // ! annonce une commande

    String requestFormat = "!";
    requestFormat += message;
    requestFormat += " #";
    requestFormat += String(indice);
    clients[indiceClient].print(requestFormat);
    clients[indiceClient].flush();

    unsigned long debut = millis();

    while(true){
      if(clients[indiceClient] && clients[indiceClient].connected() && clients[indiceClient].available()){
        String request = receptionRequest(indiceClient);
        request.trim();

        if(request == "ok"){
          break;
        }
      }

      if(millis() - debut > 800){
        int repet = nb;
        repet++;
        sendMessageClient(message, indice, repet);
        break;
      }

      delay(10);
    }
  }
}







void gereReceptionSerial(){
  delay(100);

  // Serial.println("message serie");

  char c = Serial.peek();
  String request = "";
  String idRecu = "";
  int id;

  if(c == '!'){
    request = receiveSerialCommande();
    request.trim();
  }


  c = Serial.peek();
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
    idRecu = receiveSerialId();
    idRecu.trim();
    id = idRecu.toInt();
  }

  c = Serial.peek();
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

  // Serial.print("commande: "); Serial.println(request);
  // Serial.print("id: "); Serial.println(id);

  // on ne transmet le message au checkpoint que s'il y a une requête, inutile de juste lui envoyer son id
  if(request.length() > 0){
    sendMessageClient(request, id, 0);
  }

}