#include <Arduino.h>
#include "utils.hpp"
#include "WiFi.hpp"

#ifndef SERIE
#define SERIE

String receiveSerialCommande();
String receiveSerialId();
void sendMessageClient(String message, int indice, int nb = 0);
void gereReceptionSerial();

#endif