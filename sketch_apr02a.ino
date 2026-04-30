#include <WiFi.h>        // Permet de connecter l'ESP32 au WiFi
#include <HTTPClient.h> // Permet d'envoyer des requêtes HTTP (POST/GET)
#include <WebServer.h>  // Permet de créer un serveur web sur l'ESP32

// WIFI 
// Nom du réseau WiFi et mot de passe
const char* ssid = " NomRéseau";
const char* password = "Motdepasse";

//  SERVEUR PHP 
// Adresse du serveur (VM Ubuntu avec Apache + PHP)
const char* serverName = "IPDELAVM/DOSSIERVOULU/PAGE.PHP";

// WEB SERVER 
// Création du serveur web sur le port 80 (port HTTP classique)
WebServer server(80);

// PINS 
// Définition des broches utilisées
const int led = 27;     // LED branchée sur la broche 27
const int buzzer = 26;  // Buzzer branché sur la broche 26

// TIMER
// Permet d'envoyer les données toutes les X secondes sans bloquer le programme
unsigned long previousMillis = 0;
const long interval = 5000; // 5 secondes

// CONNEXION WIFI
void connectWiFi() {

  // Démarre la connexion au WiFi
  WiFi.begin(ssid, password);

  Serial.print("Connexion WiFi");

  // Boucle tant que l'ESP32 n'est pas connecté
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Une fois connecté
  Serial.println("\nConnecté !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP()); // Affiche l'IP de l'ESP32
}

// ENVOI DE DONNÉES
void sendData() {

  // Génère une valeur aléatoire entre 0 et 99
  int valeur = random(0, 100);

  // Création de l'objet HTTP
  HTTPClient http;

  // Connexion au serveur PHP
  http.begin(serverName);

  // Indique qu'on envoie des données de type formulaire
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Création de la donnée à envoyer (POST)
  String postData = "valeur=" + String(valeur);

  // Envoi de la requête POST
  int code = http.POST(postData);

  // Affichage dans le moniteur série
  Serial.println("--------------");
  Serial.print("Valeur envoyée: ");
  Serial.println(valeur);

  Serial.print("Code HTTP: ");
  Serial.println(code);

  // Si le serveur répond correctement
  if (code > 0) {
    String response = http.getString(); // récupère la réponse
    Serial.print("Réponse serveur: ");
    Serial.println(response);
  }

  // Ferme la connexion
  http.end();
}

// GESTION LED
void handleLed() {

  // Allume la LED
  digitalWrite(led, HIGH);
  delay(300);

  // Éteint la LED
  digitalWrite(led, LOW);

  // Réponse envoyée au navigateur
  server.send(200, "text/plain", "LED OK");
}

// GESTION BUZZER 
void handleBuzzer() {

  // Initialisation du PWM pour le buzzer
  // (nécessaire sur ESP32 pour produire un son)
  ledcAttach(buzzer, 2000, 8);

  // Joue plusieurs sons
  ledcWriteTone(buzzer, 1000);
  delay(300);

  ledcWriteTone(buzzer, 500);
  delay(300);

  ledcWriteTone(buzzer, 1500);
  delay(300);

  // Arrête le son
  ledcWriteTone(buzzer, 0);

  // Réponse envoyée au navigateur
  server.send(200, "text/plain", "BUZZER OK");
}

// SETUP 
void setup() {

  // Initialise le moniteur série
  Serial.begin(115200);

  // Configure la LED comme sortie
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW); // LED éteinte au départ

  // Connexion au WiFi
  connectWiFi();

  // Définition des routes du serveur web ESP32
  // Quand on tape /led → exécute handleLed()
  server.on("/led", handleLed);

  // Quand on tape /son → exécute handleBuzzer()
  server.on("/son", handleBuzzer);

  // Démarre le serveur web
  server.begin();

  Serial.println("Serveur ESP32 prêt");
}

// LOOP 
void loop() {

  // Permet au serveur de traiter les requêtes (très important)
  server.handleClient();

  // Si le WiFi est perdu → on reconnecte
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
    return;
  }

  // Envoi automatique des données toutes les 5 secondes
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    sendData();
  }
}
