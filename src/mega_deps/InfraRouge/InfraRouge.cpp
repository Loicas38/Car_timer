// ****************************************************************** about IR command *********************************************************************
#include <Arduino.h>
#include <stdlib.h>
#define USE_IRREMOTE_HPP_AS_PLAIN_INCLUDE
#include <IRremote.hpp>
#include <LiquidCrystal.h>
#include "Distance.h"
#include "InfraRouge.h"
#include "Variables.h"


// retourne le code associé à la touche
int getCodeTouche(const char* touche){

    for(int i = 0; i < NB_TOUCHES; i++){
        if (strcmp(touche, touches_lst[i]) == 0){
            return codes_lst[i];
        }
    }

  return 0;
}

const char* getNomTouche(int touche){
    for(int i = 0; i < NB_TOUCHES; i++){
        if(touche == codes_lst[i]){
            return touches_lst[i];
        }
    }

    return "unknown";
}

void waitClickTouche(const char* touche, bool displayDistance){
    int codeTouche = getCodeTouche(touche);
    IrReceiver.decodedIRData.command = 0;

    while (true) {

        // choix d'afficher ou non la distance
        if(displayDistance){
          float distance = mesureDistance();

          lcd.setCursor(7, 0);
          lcd.print("         ");
          lcd.setCursor(7, 0);
          lcd.print(distance);
        }


        // pour détecter une touche cliquée et relancer l'attente
        if (IrReceiver.decode()) {

            IrReceiver.resume();
            if (IrReceiver.decodedIRData.command == codeTouche){
              IrReceiver.decodedIRData.command = 0;
              return;
            }
            
        }

        delay(100);
    }
}

const char* waitAndGetTouche(){
  IrReceiver.decodedIRData.command = 0;
  IrReceiver.resume();

  while (true) {
      // pour détecter une touche cliquée et relancer l'attente
      if (IrReceiver.decode()) {
          IrReceiver.resume();
          
          // Serial.println(IrReceiver.decodedIRData.command);
          return getNomTouche(IrReceiver.decodedIRData.command);
      }

      delay(10);
  }
}

const char* waitAndGetTouche(uint16_t timeout){
  IrReceiver.decodedIRData.command = 0;
  IrReceiver.resume();

  uint32_t startTime = millis();

  while (true) {
      // pour détecter une touche cliquée et relancer l'attente
      if (IrReceiver.decode()) {
          IrReceiver.resume();
          
          // Serial.println(IrReceiver.decodedIRData.command);
          return getNomTouche(IrReceiver.decodedIRData.command);
      }

      if (millis() - startTime >= timeout) {
          return NULL; // Retourne NULL si le délai est écoulé sans détection de touche
      }

      delay(10);
  }
}