#include <Arduino.h>
#include "utils.hpp"

#ifndef WIFI
#define WIFI

String receptionRequest(int indice);
String receptionId(int indice);
void receptionMessagesWifi(int indice);
void gestionMessages(String request, int indice, int connexion);

#endif