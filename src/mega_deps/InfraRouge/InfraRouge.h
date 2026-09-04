#ifndef INFRAROUGE_H
#define INFRAROUGE_H

int getCodeTouche(const char* touche);
const char* getNomTouche(int touche);
void waitClickTouche(const char* touche, bool displayDistance);
bool waitClickTouche(unsigned int touche, uint16_t timeout);
int waitAndGetTouche();
int waitAndGetTouche(uint16_t timeout);
bool has_touch_been_pressed(unsigned int code);

#endif