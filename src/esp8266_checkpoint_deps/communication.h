#include <Arduino.h>
#include "variables.hpp"
#include "distance.h"
#include "accelero.h"
#include "utils.h"

#ifndef COMMUNICATION_esp

#define COMMUNICATION_esp


void envoiMessage(String message, int id, int nb);
void distanceEtCommunication(int id);
void gestionMessages(String request, int id);
String receptionRequest();
String receptionId();
void receptionMessages();

#endif
