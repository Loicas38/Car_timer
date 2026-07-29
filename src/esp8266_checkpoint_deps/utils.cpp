#include "utils.h"

int compareFloats(const void *a, const void *b) {
    float floatA = *(const float *)a;
    float floatB = *(const float *)b;
    if (floatA < floatB) return -1;
    if (floatA > floatB) return 1;
    return 0;
}

// retourne l'indice ou se trouve l'élément demandé dans le tableau
// retourne -1 s'il n'a pas été trouvé
int getIndiceIntListe(int array[], int size, int recherche){
  for(int i = 0; i< size; i++){
    if(array[i] == recherche){
      return i;
    }
  }

  return -1;
}
