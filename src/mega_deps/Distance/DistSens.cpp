#include "DistSens.hpp"


DistSens::DistSens(Screen* screen): dist_min(15){
    // initialisation pins in et out pour le capteur ultrasons
    pinMode(sendPinDist, OUTPUT);
    pinMode(recievePinDist, INPUT);

    uint16_t average_dist = 0;

    screen->display("Ready for dist measure ?", "");
    Serial.println("Beginning dist measure");

    for (int i = 0; i < 1; i++){
        float dist = current_dist();
        screen->display("Dist : " + String(dist), "", 100);
        average_dist += dist;
    }

    lim_dist = average_dist / 20 - 10;
    // TEMP
    lim_dist = 15;

    screen->display("Final dist : " + String(average_dist / 20), "", 3500);
    
}


DistSens::DistSens(Screen* screen, int dist_min): dist_min(dist_min){
    // initialisation pins in et out pour le capteur ultrasons
    pinMode(sendPinDist, OUTPUT);
    pinMode(recievePinDist, INPUT);

    uint16_t average_dist = 0;

    for (int i = 0; i < 20; i++){
        float dist = current_dist();
        screen->display("Measuring dist : " + String(dist), "", 100);
        average_dist += dist;
    }

    lim_dist = average_dist / 20 - 10;

    screen->display("Final dist : " + String(average_dist / 20), "", 3500);
    
}

void DistSens::measure_detect_dist(Screen* screen){
    uint16_t average_dist = 0;

    for (int i = 0; i < 20; i++){
        float dist = current_dist();
        screen->display("Measuring dist : " + String(dist), "", 100);
        average_dist += dist;
    }

    lim_dist = average_dist / 20 - 10;

    screen->display("Final dist : " + String(average_dist / 20), "", 3500);
}



float DistSens::current_dist(){
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

bool DistSens::is_car_detected(){
    float dist = current_dist();
    return dist < lim_dist;
}