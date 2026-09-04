#include <Arduino.h>
#include "mega_deps/Ecran/Screen.hpp"

#ifndef DISTSENS
#define DISTSENS


class DistSens{
private:
    // the dist under which it means that the car was detected
    int lim_dist;
    // the minimum value for lim_dist
    int dist_min;

    const int sendPinDist = 44;
    const int recievePinDist = 46;


public:
    // measures the distance to be then able to detect when the car passes
    void measure_detect_dist(Screen* screen);
    // measures and returns the current dist of the obstacle in front of the sensor
    float current_dist();

    bool is_car_detected();

    DistSens(Screen* screen);
    DistSens(Screen* screen, int dist_min);

};


#endif