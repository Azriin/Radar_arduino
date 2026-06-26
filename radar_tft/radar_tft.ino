// https://passionelectronique.fr/tutorial-ecran-tft-ili9341-arduino-esp32/

#define MINANGLE 25
#define MAXANGLE 155

uint8_t angle_courant = MINANGLE;
char pas = 1;

void setup() {
  setTables();
  initTFT();
  initCapteur();
  initServo();
  
  displayLadder();
  displayNumbers();
  turn(MINANGLE);
  delay(3000);
  turn((MAXANGLE+MINANGLE)/2);
  delay(3000);
  turn(MAXANGLE);
  delay(3000);
}

void loop() {
  for (int i = 0; i <= MAXANGLE-MINANGLE; i ++) {
    angle_courant += pas;
    turn(angle_courant);
    saveDistance(angle_courant, readCapteurMedian());
    displayValue(getDistance(angle_courant));
    renderRay(angle_courant, pas);
  }
  pas *= -1;
}