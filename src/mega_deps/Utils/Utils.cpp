// ************************************************************** useful functions ****************************************************************************
#include "Utils.h"

// Fonction de comparaison pour les floats pour le tri 
int compareFloats(const void *a, const void *b) {
    float floatA = *(const float *)a;
    float floatB = *(const float *)b;
    if (floatA < floatB) return -1;
    if (floatA > floatB) return 1;
    return 0;
}

void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        // Déplacer les éléments de arr[0..i-1], qui sont
        // plus grands que key, d'une position en avant
        // de leur position actuelle
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// return if an element is in the array or no
bool rechercherString(String tableau[], int taille, String recherche) {
  for (int i = 0; i < taille; i++) {
    if (tableau[i] == recherche) {
      return true;
    }
  }
  return false; // Retourne false si la chaîne n'est pas trouvée
}

// return the id of the element if t's in the array, otherwise you get -1
int getIdElement(String array[], int size, String element) {
  for (int i = 0; i < size; i++) {
      if (array[i]== element) {
          return i;  // L'élément a été trouvé dans le tableau
      }
  }
  return -1;  // L'élément n'a pas été trouvé dans le tableau
}

// déplace tout les éléments après depart vers la gauche. Le premier élément déplacé est celui après départ, qui est copié dans départ
void shiftLeft(String arr[], int size, int depart) {
  // Vérifiez si le tableau a plus d'un élément
  if (size > 1) {
    // Déplacez chaque élément du tableau vers la gauche
    for (int i = depart; i < size - 1; i++) {
      arr[i] = arr[i + 1];
    }
    // Assurez-vous que le dernier élément soit vide
    arr[size - 1] = "";
  }
}

// return the place of the element in the array
// return -1 if this element doesn't exist
int rechercheInt(int array[], int size, int recherche){
  for(int i = 0; i < size; i++){
    if(array[i] == recherche){
      return i;
    }
  }

  return -1;
}

// **************************************************************  about formater le temps  *************************************************************


int calculMinutes(unsigned long temps){
  return temps / 60000;
}
int calculSecondes(unsigned long temps){
  return (temps - long(temps / 60000)*60000)/1000;
}
int calculMilli(unsigned long temps){
  return temps - long(temps / 1000)*1000;
}