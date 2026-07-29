#include "utils.hpp"


int nbTotalCheckpoints = 0;
int nbCheckpointsTrouves = 0;

// enregistre les ids trouvés
int listeCheckpoints[nbMaxClients] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
// enregistre la correspondane entre les ids et les clients
// si listeCheckpoints[0] = 2, correspondanceIdClient[0] correspond à l'indice du client qui a envoyé cet id
//int correspondanceIdClient[nbMaxClients];
// Liste des clients connectés

// ************************************************************** nouveau checkpoint ****************************************************************

// enregistre les nouveaux checkpoints
// id est celui du checkpoint, indice correspond à l'emplacement dans la liste des clients
// retourne true si ca a marché, false, sinon
bool enregistreCheckpoint(int id, int indice){
  // lorsqu'on parcourt les id déjà enregistrés, on met à true si on trouve l'id qu'on vient de recevoir
  bool idFound = false;

  for (int j = 0; j < nbCheckpointsTrouves; j++) {
    // cas où l'id reçu est déjà dans la liste
    if (listeCheckpoints[j] == id) {
      idFound = true;
      break;
    }
  }

  // Serial.print("id existe déjà: "); Serial.println(idFound);

  // si l'id qu'on a reçu n'est pas dans la liste, on le met
  if (!idFound) {
    for(int i = 0; i < nbMaxClients; i++){
      if(listeCheckpoints[i] == -1){
        listeCheckpoints[i] = id;
        correspondanceIdClient[i] = indice;

        // Serial.print("l'id "); Serial.print(id); Serial.print("a bien été enregistré à la place "); Serial.println(i);
        // Serial.print("et correspond au client d'indice "); Serial.println(indice);

        nbTotalCheckpoints++;

        return true;
      }
    }
  }

  return false;
}