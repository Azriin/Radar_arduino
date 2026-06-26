// #define OPTI

#ifdef OPTI

float table_sin[MAXANGLE-MINANGLE];
float table_cos[MAXANGLE-MINANGLE];

// initialise les tables cos et sin
void setTables(void) {
  for (int i = MINANGLE; i < MAXANGLE; i++) {
    double angle = i * (M_PI/180);
    table_cos[i-MINANGLE] = (float)cos(angle);
    table_sin[i-MINANGLE] = (float)sin(angle);
  }
}

// obtiens le sin d'angle
float getSin(int angle) {
  angle -= MINANGLE;
  return table_sin[angle];
}

// obtiens le cos d'angle
float getCos(int angle) {
  angle -= MINANGLE;
  return table_cos[angle];
}

#else
// initialise les tables cos et sin
void setTables(void) {
  // pass
}

// obtiens le sin d'angle
float getSin(int angle) {
  return sin(angle*M_PI/180);
}

// obtiens le cos d'angle
float getCos(int angle) {
  return cos(angle*M_PI/180);
}
#endif