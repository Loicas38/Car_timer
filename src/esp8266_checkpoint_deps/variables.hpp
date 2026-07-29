#include <ESP8266WiFi.h>
#include <Adafruit_ADXL345_U.h>

#ifndef VARIABLES_esp
#define VARIABLES_esp

extern Adafruit_ADXL345_Unified accel;

// Définir le SSID et le mot de passe du réseau Soft AP
static const char *ssid = "ESP8266_AP";
static const char *password = "12345678";

// Définir l'adresse IP et le port du serveur
static const char *host = "192.168.4.1";  // L'adresse IP par défaut du point d'accès ESP8266 en mode Soft AP
static const int port = 80;

// nombre de capteurs de distance reliés à l'esp
static const int nbCheckpoints = 1;

extern float distanceNormale[nbCheckpoints];
extern const int trigPin[nbCheckpoints];
extern const int echoPin[nbCheckpoints];
extern const int pinLeds[nbCheckpoints];

extern int ids[nbCheckpoints];

extern bool doitMesurerDistance[nbCheckpoints];

extern bool isLedOn[nbCheckpoints];

// pour la mesure de distance
extern float duration;
extern float distance;


// enregistre si il y a des mesures à faire ou si aucune n'est à faire
// true : les capteurs ultrason sont éteints, false certains ou tous sont allumés
extern bool aucuneMesure;

// définit si l'accéléromètre est présent et fonctionne
extern bool isAccel;

static WiFiClient client;
#endif