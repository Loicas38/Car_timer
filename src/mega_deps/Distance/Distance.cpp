// ************************************************  about distance *****************************************************************************

#include "Distance.h"
#include <Arduino.h>
#include <LiquidCrystal.h>
#define USE_IRREMOTE_HPP_AS_PLAIN_INCLUDE
#include <IRremote.hpp>
#include "Variables.h"
#include "Utils.h"
#include "InfraRouge.h"

// mesure la distance une seule fois et renvoie la mesure
float mesureDistance(){
  digitalWrite(sendPinDist, LOW);
  delayMicroseconds(2);
  digitalWrite(sendPinDist, HIGH);
  delayMicroseconds(10);
  digitalWrite(sendPinDist, LOW);

  float duration = pulseIn(recievePinDist, HIGH);
  float distance = (duration*.0343)/2;

  // on conserve 2 chiffres après la virgule
  distance = floorf(distance*100)/100;

  //Serial.print("Distance: ");
  //Serial.println(distance);

  return distance;
}

// mesure une distance avec ultrasons et renvoie un float avec 2 chiffres après la virgule
// efectue plusieurs mesures et renvoie la médiane
float mesureDistancePrecise(){
  int nb;
  float mesureRapide[nbMesureRapide];

  for (nb = 0; nb < nbMesureRapide; nb++){
    mesureRapide[nb] = mesureDistance();
    delay(5);
  }
  
  int size = sizeof(mesureRapide) / sizeof(mesureRapide[0]);
  qsort(mesureRapide, size, sizeof(float), compareFloats);
  
  return mesureRapide[milieuMesureRapide];
}

// mesure la distance de l'objet enface de nombreuse fois et calcul la médiane. Cette médiane sera considérée comme la distance de la chose en face du capteur
// C'est cette distance qui va permettre de détecter le passage d'une voiture
void mesureDistanceDepart(){
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Dist : ");

  lcd.setCursor(0, 1);
  lcd.print("getting distance");

  float mesures[nbVals];

  // prend plusieurs mesures de la distance de l'obstacle en face du capteur
  for(int nb = 0; nb < nbVals; nb ++){
    float distance = mesureDistance();

    lcd.setCursor(7, 0);
    lcd.print(distance);

    mesures[nb] = distance;

    delay(100);
  }

  // tri des mesures et calcul de la médiane
  int size = sizeof(mesures) / sizeof(mesures[0]);
  qsort(mesures, size, sizeof(float), compareFloats);
  distanceNormal = mesures[int(nbVals / 2)];

  lcd.setCursor(0, 1);
  lcd.print("fin, dist = ");
  lcd.print(distanceNormal);
  delay(2000);
}


// permet de régler la distance de déclenchement, c'est à dire que si distanceNormal = 20 et distanceDeclenche = 10, 
// il faut que la voiture passe à 20 - 10 du capteur au plus loin pour être détectée
void reglerDistanceMini(){

  char touchePressee[8] = "";

  while(strcmp(touchePressee, "A") != 0){
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("distance declenchement");
    lcd.setCursor(0, 1);
    lcd.print(distanceDeclenchement);
    lcd.print("cm");

    const char* toucheTemp = waitAndGetTouche();
    strcpy(touchePressee, toucheTemp);

    if(strcmp(touchePressee, "haut") == 0){
      distanceDeclenchement++;
    }

    if(strcmp(touchePressee, "bas") == 0){
      distanceDeclenchement++;

      if(distanceDeclenchement < 0){
        distanceDeclenchement = 0;
      }
    }
  }
  
}