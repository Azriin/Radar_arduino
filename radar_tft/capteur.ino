#define trigPin 4 
#define echoPin 5 

uint16_t lstDistances[180] = {255};
uint16_t lstLastDistances[180] = {255};


void initCapteur() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
} 

uint16_t readCapteur() { 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
  return pulseIn(echoPin, HIGH) * 0.017; 
}

void saveDistance(uint8_t angle, uint16_t distance) {
  lstLastDistances[angle] = lstDistances[angle];
  lstDistances[angle] = distance;
}

uint16_t getDistance(uint8_t angle) {
  return lstDistances[angle];
}

uint16_t getLastDistance(uint8_t angle) {
  return lstLastDistances[angle];
}

