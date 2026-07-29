#include <ESP8266WiFi.h>
#include <Arduino.h>

#ifndef UTILS
#define UTILS

// Définir le SSID et le mot de passe du réseau Soft AP
static const char *ssid = "ESP8266_AP";
static const char *password = "12345678";

// nombre max de clients pouvant se connecter au serveur
static const int nbMaxClients = 10;

extern int nbCheckpointsTrouves;
extern int nbTotalCheckpoints;

// Définir le serveur sur le port 80
static WiFiServer server(80);
static WiFiClient clients[nbMaxClients];

static int correspondanceIdClient[nbMaxClients];
extern int listeCheckpoints[nbMaxClients];


bool enregistreCheckpoint(int id, int indice);


#endif