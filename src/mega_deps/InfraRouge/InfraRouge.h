#ifndef INFRAROUGE_H
#define INFRAROUGE_H

int getCodeTouche(const char* touche);
const char* getNomTouche(int touche);
void waitClickTouche(const char* touche, bool displayDistance);
const char* waitAndGetTouche();
const char* waitAndGetTouche(uint16_t timeout);

#endif