#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS        10
#define TFT_RST       9
#define TFT_DC        8

#define NOIR          0x0000
#define GRIS          0x738e
#define VERT          0x07E0
#define ROUGE         0xF800

#define MAXWIDTH      320
#define MAXHEIGHT     240

#define ESPACERAYON   15
#define TAILLERAYON   200
#define ORIGINERAYONX MAXWIDTH/2
#define ORIGINERAYONY MAXHEIGHT

#define ECHELLEDIST   4 // 1cm dans la vrai vie = 4 pixels
#define RCERCLEDIST   ECHELLEDIST * 10
#define MAXDISTAFF    TAILLERAYON/ECHELLEDIST

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
int rayCourant[4];
int rayPrecedent[4];

// initialise l'ecran TFT et affiche les infos de bases
void initTFT(void) {
  tft.begin();

  tft.setRotation(3);
  tft.fillScreen(NOIR);
  tft.setTextColor(VERT);
  tft.setTextSize(2);
  
  tft.setCursor(2, 2);
  tft.println("Radar de wish");
  tft.setCursor(190, 2);
  tft.println("Dist :  ");

  tft.drawRect(0, 18, MAXWIDTH, 2, VERT);
  tft.drawRect(MAXWIDTH/2+10, 0, 2, 18, VERT);
}

// affiche value (max 3 chiffres) en haut a droite de l'ecran
void displayValue(int value) {
  tft.setCursor(280, 2);
  tft.setTextSize(2);
  tft.setTextColor(VERT, NOIR);
  tft.print(value/100);
  value %= 100;
  tft.print(value/10);
  value %= 10;
  tft.print(value);
}

// affiche les cercles concentriques et les rayons d'angles
void displayLadder(void) {
  int x = ORIGINERAYONX;
  int y = ORIGINERAYONY;
  for (int i = RCERCLEDIST; i <= TAILLERAYON; i += RCERCLEDIST) {
    tft.drawCircle(x, y, i, GRIS);
  }
  for (int angle = ESPACERAYON; angle < 180; angle += ESPACERAYON) {
    double rad = angle * (M_PI/180);
    int x1 = x - TAILLERAYON * cos(rad);
    int y1 = y - TAILLERAYON * sin(rad);
    tft.drawLine(x, y, x1, y1, GRIS);
  }
}

// calcul le rayon avec de angle degre et sauvegarde le precedent rayon
void calcNewRay(int angle) {
  for (int i = 0; i < 4; i++) {
    rayPrecedent[i] = rayCourant[i];
  }
  rayCourant[0] = ORIGINERAYONX;
  rayCourant[1] = ORIGINERAYONY;
  rayCourant[2] = rayCourant[0] - TAILLERAYON * getCos(angle);
  rayCourant[3] = rayCourant[1] - TAILLERAYON * getSin(angle);
}

// calcul et affiche le nouveau rayon
void displayRay(uint8_t angle, int couleur1, int couleur2) {
  calcNewRay(angle);
  tft.drawLine(rayPrecedent[0], rayPrecedent[1], rayPrecedent[2], rayPrecedent[3], couleur2);
  tft.drawLine(rayCourant[0], rayCourant[1], rayCourant[2], rayCourant[3], couleur1);
}

// affiche un point
void renderDistance(uint8_t angle, int couleur) {
  double c = getCos(angle);
  double s = getSin(angle);

  int distance = getLastDistance(angle) * ECHELLEDIST;
  int x = ORIGINERAYONX - distance * c;
  int y = ORIGINERAYONY - distance * s;
  tft.fillCircle(x, y, 2, NOIR);

  distance = getDistance(angle);
  if (distance > MAXDISTAFF) return;
  distance *= ECHELLEDIST;
  x = ORIGINERAYONX - distance * c;
  y = ORIGINERAYONY - distance * s;
  tft.fillCircle(x, y, 2, couleur);
}

// affiche les angles des rayons
void displayNumbers(void) {
  float rad;
  int x, y;
  tft.setTextSize(1);
  tft.setTextColor(GRIS);
  for (int i = 50; i < 140; i += ESPACERAYON) { // avant 50 et apres 140, l'ecran est trop petit pour etre affiche 
    x = ORIGINERAYONX - (TAILLERAYON+15) * getCos(i);
    y = ORIGINERAYONY - (TAILLERAYON+15) * getSin(i);
    tft.setCursor(x-5, y);
    tft.print(i);
  }
}

void renderRay(uint8_t angle, int pas) {
  int couleur = GRIS;
  if ((angle-pas)%ESPACERAYON) {
    couleur = NOIR;
  }
  displayRay(angle, VERT, couleur);
  renderDistance(angle-pas, ROUGE);
  // renderDistance(angle_courant, ROUGE);
}