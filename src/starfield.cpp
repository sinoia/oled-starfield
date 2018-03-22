/*
  3D starfield simulation.
    Inspiration from the code and demo at:
    http://codentronix.com/2011/05/28/3d-starfield-made-using-python-and-pygame/
*/

#include <Wire.h>
#include "SSD1306.h"

// Initialize the oled display at address 0x3c, with SDA/SCL on pins 4/5
SSD1306  display(GEOMETRY_128_64, 0x3c, 4, 5);

const int starCount = 512; // number of stars in the star field
const int maxDepth = 32;   // maximum distance away for a star

// the star field - starCount stars represented as x, y and z co-ordinates
double stars[starCount][3];

int getRandom(int lower, int upper) {
    /* Generate and return a  random number between lower and upper bound */
    return lower + static_cast<int>(rand() % (upper - lower + 1));
}

void drawStars() {
    int origin_x = display.width() / 2;
    int origin_y = display.height() / 2;

    // Iterate through the stars reducing the z co-ordinate in order to move the
    // star closer.
    for (int i = 0; i < starCount; ++i) {
       stars[i][2] -= 0.19;
       // if the star has moved past the screen (z < 0) reposition it far away
       // with random x and y positions.
       if (stars[i][2] <= 0) {
           stars[i][0] = getRandom(-25, 25);
           stars[i][1] = getRandom(-25, 25);
           stars[i][2] = maxDepth;
       }

       // Convert the 3D coordinates to 2D using perspective projection.
       double k = display.width() / stars[i][2];
       int x = static_cast<int>(stars[i][0] * k + origin_x);
       int y = static_cast<int>(stars[i][1] * k + origin_y);

       //  Draw the star (if it is visible in the screen).
       // Distant stars are smaller than closer stars.
       if ((0 <= x and x < display.width()) 
           and (0 <= y and y < display.height())) {
           int size = (1 - stars[i][2] / maxDepth) * 4;
           display.fillRect(x, y, size, size);
       }
    }
}

void setup() {
  display.init();
  display.flipScreenVertically();
  // Initialise the star field with random stars
  for (int i = 0; i < starCount; i++) {
    stars[i][0] = getRandom(-25, 25);
    stars[i][1] = getRandom(-25, 25);
    stars[i][2] = getRandom(0, maxDepth);
  }
}

void loop() {
    Serial.begin(115200);
    display.clear();
    drawStars();
    display.display();
}




