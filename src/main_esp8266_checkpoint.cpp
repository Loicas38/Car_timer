#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <ESP8266WiFi.h>
#include "esp8266_checkpoint_deps/variables.hpp"
#include "esp8266_checkpoint_deps/utils.h"
#include "esp8266_checkpoint_deps/distance.h"
#include "esp8266_checkpoint_deps/accelero.h"
#include "esp8266_checkpoint_deps/communication.h"


void setup() {
  Serial.begin(115200);
  delay(5000);

  // Initialiser l'accéléromètre
  if (!accel.begin()) {
    Serial.println("Impossible de trouver l'ADXL345. Vérifiez les connexions.");

    // pas d'accéléromètre présent ou fonctionel
    isAccel = false;
  }
  
  Serial.println("ADXL345 Initialisé!");

  // Configurer l'ADXL345 pour détecter l'activité
  accel.writeRegister(ADXL345_REG_THRESH_ACT, 0x23); // Définir le seuil d'activité (ajuster cette valeur selon vos besoins)
  accel.writeRegister(ADXL345_REG_ACT_INACT_CTL, 0x70); // Activer la détection d'activité sur X, Y, Z (0x70 = X, Y, Z)
  
  // Activer les interruptions pour l'activité
  accel.writeRegister(ADXL345_REG_INT_ENABLE, 0x10); // Activer les interruptions pour activité (bit 4)

  // initialise les pins de distance
  for(int i = 0; i<nbCheckpoints; i++){
    // pins mesure de distance
    pinMode(trigPin[i], OUTPUT);
    pinMode(echoPin[i], INPUT);

    // pins leds
    pinMode(pinLeds[i], OUTPUT);
    digitalWrite(pinLeds[i], HIGH);
  }


  // Connexion au réseau Soft AP
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // connection au réseau wi fi
  WiFi.begin(ssid, password);

  // on attend d'être connecté au wi fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Connecting to ");
  Serial.println(host);

  // Connexion au serveur (il faut d'abord être connecté au wi fi pour réaliser cette étape)
  // initialise la connexion au serveur
  while (!client.connect(host, port)) {
    Serial.println("Connection failed");
    delay(1000);
  }


  //bool idRecu = false;

  // permet au serveur de savoir que ce checkpoint sera utilisé
  Serial.println("envoie de notre id");

  for(int i = 0; i<nbCheckpoints; i++){
    String envoieId = "!envoie id";
    envoieId += " #";
    envoieId += String(ids[i]);
    client.print(envoieId);

    Serial.println(envoieId);
  }

  Serial.println("mesure distance départ");
  mesureDistanceDepart();
  Serial.println("fin mesure distance départ");
}

void loop() {
  // Serial.println(ids[0]);

  // Vérifier si la connexion WiFi est toujours active
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      delay(500);
      Serial.print(".");
    }
    Serial.println("WiFi reconnected");
  }

  // Vérifier si la connexion au serveur est toujours active
  if (!client.connected()) {
    Serial.println("Connection to server lost. Reconnecting...");
    while (!client.connect(host, port)) {
      Serial.println("Reconnection to server failed");
      delay(1000);
    }
    Serial.println("Reconnected to server");
  }

  receptionMessages();


  aucuneMesure = true;
  for(int i = 0; i<nbCheckpoints; i++){

    // vérifie si le checkpoint doit mesurer ou non, et effectue la mesure si besoin
    if(doitMesurerDistance[i]){

      // on fait clignoter la led
      isLedOn[i] = !isLedOn[i];
      if(isLedOn[i]){
        digitalWrite(pinLeds[i], HIGH);
      } else {
        digitalWrite(pinLeds[i], LOW);
      }

      //il y a des mesures à effectuer, donc on change
      // on laisse à true que s'il n'y a pas de capteur ultrason allumé
      aucuneMesure = false;

      // Vérifier si une activité a été détectée
      // S'il y a eu un mouvement, on ne fait pas la mesure de distance
      if (lectureDonneeAccel()) {
        Serial.println("!collision");
        String idEnvoi = "#";
        idEnvoi += String(ids[i]);
        Serial.println(idEnvoi);
        digitalWrite(pinLeds[i], HIGH);

        delay(100);

        /*
        // on informe le serveur de la collision
        client.print("!collision");
        client.flush();*/

        envoiMessage("collision", ids[0], 0);

        

        // on arrête d'effectuer les mesures
        doitMesurerDistance[i] = false;

        // on allume la led pour dire dommage en gros
        isLedOn[i] = true;
        digitalWrite(pinLeds[i], HIGH);

        aucuneMesure = true;
        break;
      }
      
      distanceEtCommunication(i);

    } 
  }

  // dans le cas où tous les capteurs ultrason sont éteints, on attend avant de vérifier à nouveau 
  if(aucuneMesure){    
    delay(500);
  }
  
}