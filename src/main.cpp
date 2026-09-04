#include <Arduino.h>

#include <stdlib.h>

// librairie infrarouge
#include <IRremote.hpp>

// librairie carte sd 
#include <SPI.h>
#include <SD.h>

// pour l'enregistrement des temps en json
#include <ArduinoJson.h>

#include "mega_deps/Checkpoint/Checkpoints.hpp"
#include "mega_deps/Variables/Variables.h"
#include "mega_deps/Utils/Utils.h"
#include "mega_deps/InfraRouge/InfraRouge.h"
//#include "mega_deps/SdCard/SdCard.h"
#include "mega_deps/Serie/Serie.h"
#include "mega_deps/Vrac/Vrac.h"

#include "mega_deps/Joueurs/Team.hpp"
#include "mega_deps/Joueurs/Player.hpp"
#include "mega_deps/Distance/DistSens.hpp"
#include "mega_deps/Ecran/Screen.hpp"


Screen* screen;
Checkpoints* checkpoints;
Team* team;
DistSens* dist;

String menu[3][2] = {
    {"Ready to play ?", ""},
    {"See the records", ""},
    {"Buzzer management", "(for the sound)"}
};



void chose_menu(){
    while(true){
        int option = screen->double_line_menu(menu, 3);
        Serial.println("End menu");
        delay(100);

        switch (option)
        {
        case 0:
            return;
        
        case 1:
            screen->display("opt 1", "");
            break;

        case 2:
            screen->display("opt 2", "");
            break;
        }
    }
    
}



void setup(){
    // permet de reset l'esp, utile lors de l'utilisation de reset sur la grosse carte
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, LOW);

    delay(100);
    digitalWrite(resetPin, HIGH);

    delay(3000);

    // communication avec l'ordi
    Serial.begin(115200);
    Serial2.begin(115200);

    // initialisation récepteur infrarouge
    IrReceiver.begin(11, ENABLE_LED_FEEDBACK);

    delay(2000);

    screen = new Screen;
    Serial.println("Screen initilized");
    checkpoints = new Checkpoints(screen);
    Serial.println("Checkpoints initialized");
    team = new Team(screen);
    Serial.println("Team initialized");
    dist = new DistSens(screen);
    Serial.println("Dist initialized");

    Serial.println("End setup");

}


void loop(){
    menu[0][1] = String(team->getActivePlayer()->get_name());
    chose_menu();

    screen->display("Waiting car", "", 5);
    while(! dist->is_car_detected()){
        delay(10);
    }

    uint32_t deb = millis();

    screen->display("Go !", "", 500);

    while(! dist->is_car_detected()){
        unsigned long secs = (millis() - deb) / 1000;
        uint16_t milli = (millis() - deb - secs * 1000) / 100; 
        screen->display_once("Time : " + String(secs) + "." + String(milli), "");
    }

    team->save_active_player_time(millis() - deb);
    team->display_last_time(screen);

    team->nextActivePlayer();

}