#include "accelero.h"

// retourn true s'il y a eu un mouvement, false sinon
// true -> mouvement, false -> pas de mouvement
bool lectureDonneeAccel(){

  // s'il n'y a pas d'accéléromètre ou qu'il ne fonctionne pas, on dit qu'il n'y a pas eu de mouvement
  if(!isAccel){
    return false;
  }

  uint8_t interrupts = accel.readRegister(ADXL345_REG_INT_SOURCE);


  // Vérifier si une activité a été détectée
  if (interrupts & 0x10) {
    return true;
  }

  return false;
}