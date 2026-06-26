# Radar_arduino
Radar simpliste fait avec un Arduino UNO, un capteur HC SR04 sur un servo moteur et un écran TFT 2.8'

>[!NOTE]
> * Pour des questions de place et d'efficacitée, le capteur se limite à une distance de 255 cm. Il peut detecter plus loin (voir la fiche du capteur) mais la distance enregistrée est limité à 8 octets. 
> * Pour des questions d'efficacitée, les calculs des cosinus et sinus peuvent être fait en amont mais cela consome beaucoup trop de mémoire pour la faible accélération. De plus, cela réduit la mémoire pour les variables locales. Ce qui, par moment, entraine des ralentissement.

# Fonctionnement
* L'Arduino lit les données du capteur convertit en metre, puis applique un filtre median sur **20** de ces valeurs au même angle afin de limiter les erreurs.
* Ensuite, il dessine à les points les plus proches du capteurs. Les points à plus de **50**cm sont ignorés. L'échelle de l'écrant est de 1 pour 4 (1 cm est égal à 4 pixels).
* Toutes ces valeurs peuvent être changé rapidement en modifiant les définitions de ces constantes.

# Branchement
### Servo :
* Pin 3 de l'arduino
### HC SR04 :
* Pin 4 trigger
* Pin 5 echo
### Ecran TFT :
* Pin 10 CS
* Pin 9 RST
* Pin 8 DC
* Pin 11 SDI/MOSI/SDA
* Pin 13 SCK/SCLK
* Pour plus d'information : https://passionelectronique.fr/tutorial-ecran-tft-ili9341-arduino-esp32/
