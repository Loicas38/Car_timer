// ************************************************************** carte SD ************************************************************************
// librairie carte sd 
#include <SPI.h>
#include <SD.h>
#include "SdCard.h"
#include <ArduinoJson.h>
#include "Variables.h"
#include "Vrac.h"
#include "Ecran.h"
#include "Utils.h"

// initialise la connexion à la carte sd
void initialisationCarteSd(){
  // cas où la carte sd ne fonctionne pas
  if(!SD.begin(chipSelectPin)) {
    carteSd = false;
    Serial.println("echec connexion carte sd");
    return;
  }

  carteSd = true;
  Serial.println("connexion carte sd réussie");
}


// on met les noms des fichiers trouvés dans le tableau passé en paramètre
void listerFichiers(File dir, String* tableau) {
  // place dans le tableau
  int id = 0;

  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // Pas plus de fichiers à lire
      break;
    }

    // on enregistre le nom du fichier
    tableau[id] = entry.name();
    entry.close();

    id++;
  }

  dir.close();
}



// opens the configuration given in parameter
// the extension of the file needs to be given
// return false if it didn't work, true otherwise
Team* ouvertureConfig(String nomConfig){
  if(!carteSd){
    Serial.println("carte sd ne fonctionne pas !");
    return NULL;
  }

  Serial.print("ouverture config "); Serial.println(nomConfig);

  String cheminConfig = "/CONFIG/";
  cheminConfig += nomConfig;
  // cheminConfig += ".txt";

  Serial.print("chemin config: "); Serial.println(cheminConfig);

  if(SD.exists(cheminConfig)){
    Serial.println("configuration trouvée !");

  } else {
    Serial.println("configuratio non trouvée");
    return NULL;
  }

  File dataFile = SD.open(cheminConfig, FILE_READ);

  // the file hasn't been opened
  if(!dataFile){
    return NULL;
  }

  StaticJsonDocument<384> doc;

  DeserializationError error = deserializeJson(doc, dataFile);

  dataFile.close();

  // problem with deserialization
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return NULL;
  }

  Team* team = new Team();

  // retrieve of the players' array
  JsonArray joueursJson = doc["joueurs"];

  // we retrieve and save players
  for(int i = 0; i < joueursJson.size(); i++){
    const char* joueurJson = joueursJson[i];
    Serial.print(joueurJson); Serial.println(" a été ajouté à l'équipe");
    team->add_player(String(joueurJson));
  }

  // retrieve of the rest of the data
  nbCheckpoints = doc["checkpoints"];
  jouerSon = doc["son"]; 
  saveTimeOnSd = doc["enregistrement temps"]; 
  autoPlaySolo = doc["autoplay"]; 
  distanceDeclenchement = doc["distance declenchement"]; 
  distanceNormal = doc["distance"]; 
  newSession = doc["new session"]; 

  return team;
}


// allows the user to create a config that he will be able to save on the SD card
void creerConfig(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("not implemented");

  delay(2000);
}


// save the current config on the SD card and allows the user to chose the name of the config
void saveCurrentConfig(Team* team){
  lcd.clear();
  lcd.setCursor(0, 0);

  if(!carteSd){
    Serial.println("carte sd ne fonctionne pas !");
    lcd.print("problème avec");
    lcd.setCursor(0, 1);
    lcd.print("la carte Sd");

    delay(2000);
    return;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("choisis un nom");
  lcd.setCursor(0, 1);
  lcd.print("de 8 char max");

  delay(2500);

  String nom = "/CONFIG/";
  nom += creationChaine(8);
  nom += ".txt";

  if(SD.exists(nom)){
    if(!SD.remove(nom)){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("erreur");

      delay(2000);
    }
  }

  File dataFile = SD.open(nom, FILE_WRITE);

  if(!dataFile){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("erreur");

    delay(2000);
  }

  StaticJsonDocument<256> doc;

  JsonArray joueursJson = doc.createNestedArray("joueurs");

  Player* joueurs = team->get_players();
  for(int i = 0; i < team->get_nb_players(); i++){
    joueursJson.add(joueurs[i].get_name());
  }

  doc["checkpoints"] = nbCheckpoints;
  doc["son"] = jouerSon;
  doc["enregistrement temps"] = saveTimeOnSd;
  doc["autoplay"] = autoPlaySolo;
  doc["distance declenchement"] = distanceDeclenchement;
  doc["distance"] = distanceNormal;
  doc["new session"] = newSession;

  serializeJson(doc, dataFile);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("termine !");

  delay(2000);

  dataFile.close();
}


// allows the user to chose if he wants to save the current config or create a new one
void enregistrementConfig(Team* team){
  // prevent wasting time with configs when it's not available
  if(!carteSd){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("carte SD ne");
    lcd.setCursor(0, 1);
    lcd.print("marche pas");

    delay(2000);
    return;
  }


  String menu[2] = {"save config actu", "param une config"};

  int choix = choixMenu(menu, 2, "reglage", "A");

  if(choix == -1){
    return;
  }
  if(choix == 0){
    saveCurrentConfig(team);
  }
  if(choix == 1){
    creerConfig();
  }
}


// si un fichier avec des noms de joueurs existe sur la carte sd, on l'ouvre et on ajoute son contenu au
// tableau contenant la liste des utilisateurs enregistrés. Le max est de 10 éléments
void lectureUtilisateursCarteSd() {
  // cas où la carte sd ne fonctionne pas
  if(!carteSd) {
    Serial.println("echec connexion carte sd");
    return;
  }

  // test de l'existence du fichier
  if (SD.exists("users.txt")) {
    // s'il existe on l'ouvre en mode lecture
    File dataFile = SD.open("USERS.txt", FILE_READ);

    // si l'ouverture a fonctionné, on extrait les noms
    if (dataFile) {
      Serial.println("lecture des noms");

      // on lit tous les noms, séparés par des retours à la ligne
      while (dataFile.available()) {
        String nom = dataFile.readStringUntil('\n');

        // suppression du retour à la ligne
        nom.trim();

        Serial.println(nom);

        // on enregistre le nom qu'on vient de lire
        utilisateurs[nbUtilisateurs] = nom;
        nbUtilisateurs++;

        if(nbUtilisateurs == 10){
          break;
        }
      }
      dataFile.close();  // fermeture du fichier
    } else {
      // Gestion de l'erreur si le fichier n'a pas pu être ouvert
      Serial.println("Erreur d'ouverture du fichier");
    }
  } else {
    // Gestion de l'erreur si le fichier n'existe pas
    Serial.println("Fichier USERS.txt non trouvé");
  }
}

void ajoutDonneesFichier(String data, String fichier) {
  if(!carteSd){
    return;
  }

  File dataFile = SD.open(fichier, FILE_WRITE);

  // si l'ouverture a fonctionné, on écrit les données
  if (dataFile) {
    dataFile.println(data);
    dataFile.close();
  }
}


// récupère les données des courses précédentes pour les enregistrer dans les variables qui les utilisent
void recupDataCourseAvant(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pas implemente");
  delay(4000);
  return;
  /* if(!carteSd){
    Serial.println("carte sd marche pas");
    return;
  }


  String cheminFichier = cheminTemps;
  cheminFichier += "UTILS.txt";

  Serial.print("fichier: "); Serial.println(cheminFichier);


  // Ouvrir le fichier JSON sur la carte SD
  File file = SD.open(cheminFichier);
  if (!file) {
    Serial.println("Impossible d'ouvrir le fichier");
    return;
  }
  
  // Allouer un document JSON avec une taille appropriée
  // Ajuste la taille en fonction de la taille de ton fichier JSON
  StaticJsonDocument<260> doc;

  // Lire et parser le contenu JSON du fichier
  DeserializationError error = deserializeJson(doc, file);
  
  // Fermer le fichier après lecture
  file.close();


  // Check for errors in deserialization
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  // Extracting "course" value
  idCourse = doc["course"];
  Serial.print("Course: ");
  //Serial.println(doc["course"]);

  // Accessing "records" object
  JsonObject records = doc["records"];

  // Iterating over all records
  for (JsonPair kv : records) {
    const char* key = kv.key().c_str();
    unsigned long value = kv.value().as<unsigned long>();


    int emplacement = getIdElement(joueurs, nbMaxJoueurs, key);

    if(emplacement != -1){
      bestTimeUtilisateurs[emplacement] = value;
    }

    Serial.print("Key: ");
    Serial.print(key);
    Serial.print(" -> Value: ");
    Serial.println(value);

  }
    */
}



// création d'un dossier par joueur pour enregistrer ses temps
// new enregistre si on considèreles courses qui vont suivre comme une nouvelle session ou non
// true -> nouvelle session, on crée un nouveau dossier et on met dedans
// false -> on met à la suite des derniers fichiers de temps
void creationFichiersCourse(bool newSession){
  Serial.println("detection fichiers course");

  if(!carteSd){
    Serial.println("pas de carte sd");
  }


  // chemin sur la carte SD qui mène au dossier où sont enregistrés les temps
  // on va ensuite chercher la fin du chemin en fonction de si c'est une nouvelle session ou non
  cheminTemps = "/TEMPS/";

  // permet d'ouvrir le dossier
  File dossier = SD.open("/TEMPS/");

  // enregistre le nom du dernier dossier de temps trouvé pour le moment
  int derniereSession = 0;

  while (true) {
    File entry = dossier.openNextFile();
    if (!entry) {
      // Pas plus de fichiers à lire
      break;
    }

    if(String(entry.name()).substring(4, 8).toInt() > derniereSession){
      derniereSession = String(entry.name()).toInt();
    }
  }

  dossier.close();

  cheminTemps += "RACE";

  // si on doit continuer dans le même dossier, on enregistre son chemin, sinon on ajoute 1
  if(!newSession){
    cheminTemps += String(derniereSession);
    
  } else {
    cheminTemps += String(derniereSession + 1);
  }


  cheminTemps += "/";

  // si on doit continuer la session précédente et que le fichier existe, on charge les données
  if(!newSession && SD.exists(cheminTemps)){
    Serial.println("recup des données de la dernière session");
    recupDataCourseAvant();

  } else { // sinon on crée un nouveau fichier

    // création des trucs à récupérer pour une prochaine fois
    String chemin = cheminTemps;
    chemin += "UTILS.txt";
    File file = SD.open(cheminTemps, FILE_WRITE);
    file.close();

    // création du fichier de temps
    chemin = cheminTemps;
    cheminTemps += "TEMPS.txt";
    file = SD.open(cheminTemps, FILE_WRITE);
    file.close();
  }
  
}


// enregistre les données nécessaires pour reprendre une partie où elle a été arrêtée
void enregistrementUtils(Team* team){
  String nomFichier = cheminTemps;
  nomFichier += "UTILS.txt";


  // Lire le fichier JSON existant
  File dataFile = SD.open(nomFichier);
  if (dataFile) {
    StaticJsonDocument<260> doc;

    // Parse le fichier JSON
    DeserializationError error = deserializeJson(doc, dataFile);
    dataFile.close();

    if (error) {
      Serial.print(F("Erreur de parsing: "));
      Serial.println(error.f_str());
      return;
    }

    // Accéder et modifier les données
    int course = doc["course"];
    const char* joueur = doc["records"]["Lucas"];

    Serial.print("Course actuelle: ");
    Serial.println(course);
    Serial.print("Temps actuel pour Lucas: ");
    Serial.println(joueur);

    // Modifier les données
    doc["course"] = idCourse;  // Exemple de modification

    // on modifie le record au besoin
    if(team->get_best_time() == tempsTotal){
      doc["records"][team->getActivePlayer()->get_name()] = tempsTotal;
    }

    // Ouvrir le fichier en mode écriture pour écraser les données
    dataFile = SD.open(nomFichier, FILE_WRITE | O_TRUNC);
    if (dataFile) {
      serializeJson(doc, dataFile);
      dataFile.close();
      Serial.println("Données modifiées avec succès.");
    } else {
      Serial.println("Erreur lors de l'ouverture du fichier en écriture.");
    }
  } else {
    Serial.println("Erreur lors de l'ouverture du fichier.");
  }
}


// enregistre le temps qui vient d'être réalisé
void enregistrementTemps(Team* team){
  if(!carteSd){
    Serial.println("enregistrement du temps impossible, carte sd absente");
    return;
  }

  String nomFichier = cheminTemps;
  nomFichier += "TEMPS.txt";

  Serial.print("ouverture de: "); Serial.println(nomFichier);

  File dataFile = SD.open(nomFichier, FILE_WRITE);

  // si l'ouverture a fonctionné, on écrit le temps
  if (dataFile) {
    Serial.println("enregistrement des données de course");

    // crée un tableau manuellement
    if (dataFile.size() == 0) {
      // Si le fichier est vide, commencer le tableau
      dataFile.println("[");
    } else {
      // Revenir en arrière pour remplacer le dernier ']' par une ','
      dataFile.println(",");
    }

    StaticJsonDocument<192> doc;

    JsonObject doc_0 = doc.to<JsonObject>();
    doc_0["joueur"] = team->getActivePlayer()->get_name();
    doc_0["id"] = idCourse;

    // création d'un tableau où stocker les temps aux checkpoints
    JsonArray doc_0_checkpoints = doc_0.createNestedArray("checkpoints");

    for(int i = 0; i < nbCheckpoints; i++){
      doc_0_checkpoints.add(team->getActivePlayer()->get_time_checkpoint(i));
    }

    doc_0["temps"] = team->getActivePlayer()->get_last_time();

    serializeJson(doc, dataFile);

    dataFile.close();
  } else {
    Serial.println("echec ouverture fichier");
  }

  // enregistre le dernier id de course et les records
  enregistrementUtils(team);
}