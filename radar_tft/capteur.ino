#define trigPin 4 
#define echoPin 5 

#define FILTRE 20 // taille du filtre median

uint8_t lstDistances[MAXANGLE-MINANGLE];
uint8_t lstLastDistances[MAXANGLE-MINANGLE];

// initialie le capteur
void initCapteur() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  for (int i = 0; i < MAXANGLE-MINANGLE; i++) {
    lstDistances[i] = 255;
    lstLastDistances[i] = 255;
  }
} 

// lit les donnees du capteur
int readCapteur(void) { 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
  return pulseIn(echoPin, HIGH) * 0.017; // 0.034 cm/micro s (aller retour donc on divise par 2)
}

// lisse les donnees avec un filtre median
int readCapteurMedian(void) {
  int median[FILTRE];
  for (int i = 0; i < FILTRE; i++) {
    delay(10);
    median[i] = readCapteur();
    char j = i-1;
    while (j >= 0  && median[j] > median[j+1]) {
      int temp = median[j];
      median[j] = median[j+1];
      median[j+1] = temp;
      j--;
    }
  }
  return median[FILTRE/2];
}

// enregistre la distance avec un angle donnee et retiens la precedente distance
void saveDistance(uint8_t angle, int distance) {
  angle -= MINANGLE;
  distance = min(distance, 255);
  lstLastDistances[angle] = lstDistances[angle];
  lstDistances[angle] = distance;
}

// obtien la distance enregistree avec l'angle
uint8_t getDistance(uint8_t angle) {
  angle -= MINANGLE;
  return lstDistances[angle];
}

// obtien la penultieme distance enregistree avec l'angle
uint8_t getLastDistance(uint8_t angle) {
  angle -= MINANGLE;
  return lstLastDistances[angle];
}

