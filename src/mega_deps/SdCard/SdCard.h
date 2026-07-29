#ifndef SDCARD_H
#define SDCARD_H

#include <Arduino.h>
#include "mega_deps/Joueurs/Team.hpp"


void initialisationCarteSd();
void listerFichiers(File dir, String* tableau);
Team* ouvertureConfig(String nomConfig);
void creerConfig();
void saveCurrentConfig(Team* team);
void enregistrementConfig(Team* team);
void lectureUtilisateursCarteSd();
void ajoutDonneesFichier(String data, String fichier);
void recupDataCourseAvant();
void creationFichiersCourse(bool newSession);
void enregistrementUtils(Team* team);
void enregistrementTemps(Team* team);

#endif