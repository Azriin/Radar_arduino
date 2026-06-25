// https://passionelectronique.fr/tutorial-ecran-tft-ili9341-arduino-esp32/

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Servo.h>

#define SIG_SERVO 3

#define TFT_CS    10
#define TFT_RST   9
#define TFT_DC    8

#define NOIR      0x0000
#define GRIS      0x738e
#define VERT      0x07E0
#define ROUGE     0xF800

#define MAXWIDTH  320
#define MAXHEIGHT 240

#define ESPACERAYON 10

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
// Servo servo;
int angle_courant = 90;
uint8_t pas = 1;
int rayCourant[4];
int rayPrecedent[4] = {};

void displayAngle(int angle) {
  tft.setCursor(280, 2);
  tft.setTextSize(2);
  tft.setTextColor(VERT, NOIR);
  angle = 180-angle;
  tft.print(angle/100);
  angle %= 100;
  tft.print(angle/10);
  angle %= 10;
  tft.print(angle);
}

void displayLadder(void) {
  int x = MAXWIDTH/2;
  int y = MAXHEIGHT;
  for (int i = 40; i <= 200; i += 40) {
    tft.drawCircle(x, y, i, GRIS);
  }
  for (int angle = ESPACERAYON; angle < 180; angle += ESPACERAYON) {
    float rad = angle * (M_PI/180);
    int x1 = x - 200 * cos(rad);
    int y1 = y - 200 * sin(rad);
    tft.drawLine(x, y, x1, y1, GRIS);
  }
}

void calcNewRay(int angle) {
  for (int i = 0; i < 4; i++) {
    rayPrecedent[i] = rayCourant[i];
  }
  float rad = angle * (M_PI/180);
  rayCourant[0] = MAXWIDTH/2;
  rayCourant[1] = MAXHEIGHT;
  rayCourant[2] = rayCourant[0] - 200 * cos(rad);
  rayCourant[3] = rayCourant[1] - 200 * sin(rad);
}

void displayRay(uint8_t angle, int couleur1, int couleur2) {
  calcNewRay(angle);
  tft.drawLine(rayPrecedent[0], rayPrecedent[1], rayPrecedent[2], rayPrecedent[3], couleur2);
  tft.drawLine(rayCourant[0], rayCourant[1], rayCourant[2], rayCourant[3], couleur1);
}

void renderDistance(uint8_t angle, int couleur) {
  float rad = angle  * (M_PI/180);
  double c = cos(rad);
  double s = sin(rad);

  int distance = getLastDistance(angle);
  int x = MAXWIDTH/2 - (distance*4) * c;
  int y = MAXHEIGHT  - (distance*4) * s;
  tft.fillCircle(x, y, 2, NOIR);

  distance = getDistance(angle);
  if (distance > 50) return;
  x = MAXWIDTH/2 - (distance*4) * c;
  y = MAXHEIGHT  - (distance*4) * s;
  tft.fillCircle(x, y, 2, couleur);
}

void displayNumbers(void) {
  float rad;
  int x, y;
  tft.setTextSize(1);
  tft.setTextColor(GRIS);
  for (int i = 50; i < 140; i += ESPACERAYON) {
    rad = (180-i) * (M_PI/180);
    x = MAXWIDTH/2 - 215 * cos(rad);
    y = MAXHEIGHT - 215 * sin(rad);
    tft.setCursor(x-5, y);
    tft.print(i);
  }
}

void setup() {
  // servo.attach(SIG_SERVO);
  tft.begin();
  initCapteur();
  Serial.begin(9600);

  tft.setRotation(3);
  tft.fillScreen(NOIR);
  tft.setTextColor(VERT);
  tft.setTextSize(2);
  
  tft.setCursor(2, 2);
  tft.println("Radar de wish");
  tft.setCursor(190, 2);
  tft.println("angle : ");

  tft.drawRect(0, 18, MAXWIDTH, 2, VERT);
  tft.drawRect(MAXWIDTH/2+10, 0, 2, 18, VERT);

  displayLadder();
  displayNumbers();
}


void loop() {
  if (angle_courant <= 25 || angle_courant >= 155) {
    pas *= -1;
  }
  angle_courant += pas;
  saveDistance(angle_courant, readCapteur());
  displayAngle(angle_courant);
  int couleur = GRIS;
  if ((angle_courant-pas)%ESPACERAYON) {
    couleur = NOIR;
  }
  displayRay(angle_courant, VERT, couleur);
  Serial.println(getDistance(angle_courant));
  renderDistance(angle_courant-pas, ROUGE);
  renderDistance(angle_courant, ROUGE);
  // servo.write(angle_courant);
  delay(1000);
}