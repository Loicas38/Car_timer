// ********************************************************** écran ******************************************************
#include "Ecran.h"
#include "Variables.h"
#include "Utils.h"
#include "InfraRouge.h"
#include "Vrac.h"
#include "Serie.h"

// permet de créer une chaine de caractère retourne "" si pas de chaine créée, ou la chaine sinon
String creationChaine(int nbMaxChar){
  char nom[11] = "";
  // enregistre notre emplacement dans le tableau nom
  int indiceNom = 0;

  // eneregistre notre emplacement dans l'alphabet
  int indiceCaracteres = 97;

  String nomString;

  while(true){
    lcd.clear();
    lcd.setCursor(0, 0);

    lcd.print((char)indiceCaracteres);
    lcd.setCursor(0, 1);
    lcd.print(nom);

    const char* toucheTemp = waitAndGetTouche();

    if(strcmp(toucheTemp, "droite") == 0){
      indiceCaracteres++;

      if(indiceCaracteres == 128){
        indiceCaracteres = 97;
      }

      // si on a fini l'alphabet, on va sur le caractere flèche
      // signifie suppression du dernier caractère écrit
      if(indiceCaracteres == 123){
        indiceCaracteres = 127;
      }
    }

    if(strcmp(toucheTemp, "gauche") == 0){
      indiceCaracteres--;

      if(indiceCaracteres == 126){
        indiceCaracteres = 122;
      }

      // si on a fini l'alphabet, on va sur le caractere flèche
      // signifie suppression du dernier caractère écrit
      if(indiceCaracteres == 96){
        indiceCaracteres = 127;
      }
    }

    if(strcmp(toucheTemp, "reglage") == 0){

      // suppression d'un caractère
      if(indiceCaracteres == 127){
        nom[indiceNom] = ' ';
        indiceNom--;

        // pour ne pas passer à un indice négatif
        if(indiceNom < 0){
          indiceNom = 0;
        }
      } else {
        if(indiceNom < nbMaxChar){
          nom[indiceNom] = indiceCaracteres;
          indiceNom++;
        }
      }

    }

    if(strcmp(toucheTemp, "Ok") == 0){
      nomString = nom;
      break;
    }
  }

  delay(500);

  // avant de retourner on demande à l'utilisateur s'il est sûr de lui
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("save = A del = B");
  lcd.setCursor(0, 1);
  lcd.print(nomString);

  const char* toucheTemp = waitAndGetTouche();
  if(strcmp(toucheTemp, "A") == 0){
    return nomString;
  } else {
    return "";
  }
}


// retourne l'id du menu choisi dans le tableau passé. 
// retourne -1 si la touche échap est pressée
int choixMenu(String menus[], int longueur, String toucheEntree_s, String toucheEchap_s){
  // On convertit en char* pour la suite parce que besoin de ça 
  char* toucheEntree = (char*) toucheEntree_s.c_str();
  char* toucheEchap = (char*) toucheEchap_s.c_str();

  // id des deux menus affichés
  int idMenu1 = 0;
  int idMenu2 = 1;

  // Serial.println(idMenu1);
  // Serial.println(idMenu2);
  // Serial.println();

  while(true){
    // Serial.println("tour");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print((char)255);
    lcd.print(" ");

    lcd.print(menus[idMenu1]);
    lcd.setCursor(0, 1);

    // évite les erreurs s'il n'y a qu'un menu
    if(longueur > 1){
      lcd.print(menus[idMenu2]);
    }
    
    const char* toucheTemp = waitAndGetTouche();

    if(strcmp(toucheTemp, toucheEntree) == 0){
      return idMenu1;
    }

    

    if(strcmp(toucheTemp, "haut") == 0){
      idMenu1 --;
      idMenu2--;

      if(idMenu1 < 0){
        idMenu1 = longueur - 1;
      }

      if(idMenu2 < 0){
        idMenu2 = longueur - 1;
      }
    }

    if(strcmp(toucheTemp, "bas") == 0){
      idMenu1 ++;
      idMenu2++;

      if(idMenu1 == longueur){
        idMenu1 = 0;
      }

      if(idMenu2 == longueur){
        idMenu2 = 0;
      }
    }

    if(strcmp(toucheTemp, toucheEchap) == 0){
      return -1;
    }
  }
}


// affiche les commandes disponibles à la fin de la partie. 
// En gros c'est un menu
// retourne true si la partie doit être relancée, false sinon
bool afficheCommandes(Team* team, Player* player){
  // vérifie s'il y a des messages série et gère l'action a réalisé s'il y en a 
  gereReceptionMessages(player);

  // commandes possibles à afficher entre deux parties
  String commandes[] = {"rejouer",  "mode jeu", "temps mini", "activer son", "del nw best", "reset times", "reglages checkpt", "mesure distance", "distance declenchement", "new config", "choix joueurs", "new session", "recommencer"};

  lcd.clear();

  // int idCommande = 0;
  // char touchePressee[8] = "";

  // enregistre l'id de la commande choisie
  // -1 équivaut à rejouer
  int commande = 0;

  while(true){
    commande = choixMenu(commandes, 13, "ok", "retour");

    // vérifie s'il y a des messages série et gère l'action a réalisé s'il y en a 
    gereReceptionMessages(player);

    if(commande == -1 || commande == 0){
      return true;
    }

    gererMenus(commande, team);
  }

  return false;
}


// affiche le menu pour aller dans les paramètres ou afficher les temps
void gereAffichageFin(Team* team, Player* player){
  lcd.clear();

  //int choixLigne = 0;
  //char touchePressee[8] = "";

  String options[3] = {"parametres", "voir temps", "classement"};;

  // permet de choisir le menu ou de continuer à jouer
  while(true){
    int menu = choixMenu(options, 3, "ok", "retour");

    Serial.print("menu choisi: "); Serial.println(menu);

    if(menu == 0){
      // on fait ça pour détecter si le joueur veut relancer la partie
      if(afficheCommandes(team, player)){
        return;
      }
    }

    if(menu == 1){
      if(afficheTemps(team)){
        return;
      }
    }

    // caractère de fin pressé (A)
    if(menu == -1){
      return;
    }
  }
  
}


void affichageClignotant(String message1, int col1, String message2, int col2, int nbClignotements, int delay_display, int delay_entre_clignotements){
  for(int i = 0; i < nbClignotements; i++){
    lcd.clear();
    lcd.setCursor(col1, 0);
    lcd.print(message1);
    lcd.setCursor(col2, 1);
    lcd.print(message2);

    delay(delay_display);

    lcd.clear();

    delay(delay_entre_clignotements);
  }
}

void affichageClignotant(String message1, int col1, uint32_t message2, int col2, int nbClignotements, int delay_display, int delay_entre_clignotements){
  for(int i = 0; i < nbClignotements; i++){
    lcd.clear();
    lcd.setCursor(col1, 0);
    lcd.print(message1);
    lcd.setCursor(col2, 1);
    printTime(col2, 1, message2);

    // On attend le temsp demandé, sauf si une touch est pressée, auquel cas on arrête d'attendre et on sort de la fonction
    const char* touche = waitAndGetTouche(delay_display);

    if (touche != NULL){
      return;
    }

    lcd.clear();

    const char* touche2 = waitAndGetTouche(delay_entre_clignotements);

    if (touche2 != NULL){
      return;
    }
  }
}