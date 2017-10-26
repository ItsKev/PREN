/* 
 Ultraschall Entfernungsmessung
 Funktioniert mit dem HC-04 und SRF-06 anderen Ultraschall Modulen
 Öffne eine Serielle Verbindung um das Ergebnis zu sehen
 Refferenz: http://playground.arduino.cc/Code/NewPing
*/

// Importieren der benötigten Bibliothek
#include <NewPing.h>

// Definieren der Konstanten für Trigger und Echo
#define  TRIGGER_PIN  11
#define  ECHO_PIN     10
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters).
                         //Maximum sensor distance is rated at 400-500cm.

// Objekt für den Sensor deklarieren
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Setup für NewPing (Pins und maximale Distanz).

// Deklarieren der Variablen für die Messwerte
int DistanceCm;

void setup()   // Setup um den Arduino zu konfigurieren
{
  Serial.begin(9600);
  Serial.println("Ultraschall Entfernungsmessung");
  Serial.println("Burak Kizilkaya - PREN Gruppe 33");
}


void loop()     // Loop für die Messung der Werte, läuft endlos
{ 
  delay(100);  // Wartet 100 Milisekunden zwischen den Pings (ca. 10 Pings pro Sekunde). 29 Millisekunden ist der kürzest mögliche Delay zwischen zwei Pings.
  DistanceCm = sonar.ping_cm();
  Serial.print("Ping: ");
  Serial.print(DistanceCm); 
  Serial.println(" cm");  

}
