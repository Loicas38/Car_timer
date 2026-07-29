#include "variables.hpp"

// accéléromètre
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

const int trigPin[nbCheckpoints] = {14};
const int echoPin[nbCheckpoints] = {12};

// pins leds
const int pinLeds[nbCheckpoints] = {13};


// identifiant pour savoir le numéro du checkpoint
int ids[nbCheckpoints] = {1};


// enregistre si chaque checkpoint doit effectuer des mesures
bool doitMesurerDistance[nbCheckpoints] = {false};

// enregistre si la led est allumée ou non 
bool isLedOn[nbCheckpoints] = {true};

bool aucuneMesure = true;

bool isAccel = true;


// test 
float distanceNormale[nbCheckpoints] = {-1};
float duration = -1;
float distance = -1;

