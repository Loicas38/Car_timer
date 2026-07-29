#include <Arduino.h>
#include "distance.h"
#include "variables.hpp"
#include "utils.h"

float mesureDistance(int id) {
  // mesure distance
  digitalWrite(trigPin[id], LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin[id], HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin[id], LOW);

  duration = pulseIn(echoPin[id], HIGH);
  distance = (duration * 0.0343) / 2;

  Serial.println(distance);

  return distance;
}

float mesureDistancePrecise(int id) {
  float mesureRapide[7];

  for (int nb = 0; nb < 7; nb++) {
    mesureRapide[nb] = mesureDistance(id);
    delay(5);
  }
  
  int size = sizeof(mesureRapide) / sizeof(mesureRapide[0]);
  qsort(mesureRapide, size, sizeof(float), compareFloats);
  
  return mesureRapide[3];
}

void mesureDistanceDepart() {
  float mesures[20];

  for( int i = 0; i<nbCheckpoints; i++){
    // prend plusieurs mesures de la distance de l'obstacle en face du capteur
    for (int nb = 0; nb < 20; nb++) {
      distance = mesureDistance(i);
      mesures[nb] = distance;

      digitalWrite(pinLeds[i], LOW);
      delay(100);
      digitalWrite(pinLeds[i], HIGH);
      delay(100);
    }

    // tri des mesures et calcul de la médiane
    int size = sizeof(mesures) / sizeof(mesures[0]);
    qsort(mesures, size, sizeof(float), compareFloats);
    distanceNormale[i] = mesures[9];
  }
    
}