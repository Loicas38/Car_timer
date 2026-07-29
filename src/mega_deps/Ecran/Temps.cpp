#include "Temps.h"
#include <Arduino.h>
#include "Ecran.h"
#include "Vrac.h"
#include "InfraRouge.h"
#include "Variables.h"
#include "Team.hpp"
#include "Player.hpp"


// permet l'affichage des temps des joueurs, de leurs bests et tout avec navigation
// grâce aux flèches de la télécommande 
bool afficheTemps(Team* team){
  int position = 0;
  char touchePressee[8] = "";

  String bestPlayer = team->get_best_player()->get_name();

  // on affiche d'abord le meilleur temps
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("** ");
  lcd.print(bestPlayer);
  printBest(2, 1, team);
  lcd.setCursor(14, 0);
    lcd.print("**");

  delay(500);
  const char* toucheTemp = waitAndGetTouche();
  strcpy(touchePressee, toucheTemp);

  Player* joueurs = team->get_players();
  uint8_t nbJoueurs = team->get_nb_players();

  // on boucle et on affiche un premier temps
  // on fait défiler les temps avec les flèches, réglage permet de sortir
  while(strcmp(touchePressee, "haut") != 0 && strcmp(touchePressee, "bas") != 0){
    lcd.clear();

    lcd.setCursor(0, 0);

    // on affiche des étoiles autour du nom du joueur qui a le meilleur temps 
    if (joueurs[position].get_name() == bestPlayer){
      lcd.print("** ");
      lcd.print(joueurs[position].get_name());
      lcd.setCursor(14, 0);
      lcd.print("**");
    } else {
      lcd.print("  ");
      lcd.print(joueurs[position].get_name());
    }
    
    printTime(2, 1, joueurs[position].getBestTime());

    const char* toucheTemp = waitAndGetTouche();
    strcpy(touchePressee, toucheTemp);

    // pour effectuer le décalage
    if(strcmp(touchePressee, "gauche") == 0){
      position--;
      if(position < 0){
        position = nbJoueurs - 1;
      }
    }
    if(strcmp(touchePressee, "droite") == 0){
      position++;
      if(position > nbJoueurs - 1){
        position = 0;
      }
    }

    if(strcmp(touchePressee, "A") == 0){
      return true;
    }
  }

  return false;
}

// affiche le nom du meilleur joueur sur la première ligne et son temps sur la deuxième
void displayBestPlayer(Team* team){
  lcd.clear();

  lcd.setCursor(0, 0);

  lcd.print(team->get_best_player()->get_name());
  lcd.setCursor(0, 1);
  String bestTimeStr = String(team->get_best_player()->getBestTimeMinutes()) + "m " + String(team->get_best_player()->getBestTimeSeconds() % 60) + "s";
  lcd.print(bestTimeStr);
}

