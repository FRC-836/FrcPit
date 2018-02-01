// Team 836 The RoboBees
// 2018 Pit Truss Lighting
// Options: Buttons for settings:
//          Red/Blue - When pressed, if button color is same as existing color, changes effect.
//                     If button color is different, then color changes to button pressed.
//          Yellow - When pressed, changes effect.
// version 2018.01.31.01

#include <Adafruit_NeoPixel.h>
// --------------------------------- Button and Pin definitions
#define BUTTON_YELLOW 1
#define BUTTON_RED    2
#define BUTTON_BLUE   3

#define PIN_LEFT      4
#define PIN_RIGHT     5
#define PIN_YELLOW    6

// --------------------------------- LED count and speed
#define NUM_LEDS    238    //Upper "U" - 10' configuration - don't have to change for other configs
#define NUM_LEDS_RB  59    //Rear posts
#define ChaseDelay   75

// --------------------------------- Configure NEO Pixel Strips: 1 top "U", 2 for rear posts
Adafruit_NeoPixel stripLeft    = Adafruit_NeoPixel(NUM_LEDS_RB, PIN_LEFT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripRight   = Adafruit_NeoPixel(NUM_LEDS_RB, PIN_RIGHT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripYellow  = Adafruit_NeoPixel(NUM_LEDS, PIN_YELLOW, NEO_GRB + NEO_KHZ800);

int bright = 127;
int vbright = 255;
int q = 0; //chaser variable used for every third pixel


//yellow color combo for top
int r = 235;
int g = 80;
int b = 0;
int railColorYellow[] = {r, g, b};
int bumperColor[] = {200,0,0};

////Set the starting q variables for chaser direction.
//int qmin=0;
//int qmax=3;
//int qinc=1;

//Set current bumper and Top U states and max effects
int currentBumperColor = 0;   // Red
int currentBumperEffect = 0;  // Chaser
int currentYellowEffect = 0;  // Chaser 0/1 flip directions
int bumperEffectMax = 0;
int yellowEffectMax = 1; 

//debounce tracking
int buttonState;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 150;
int x = 0;    //temp variable for tracking how many times the yellow interrupt is called


void setup() {
  //Serial.begin(9600);
  //initialize and clear each strip
  stripYellow.begin();
  stripYellow.show();
  stripLeft.begin();
  stripLeft.show();
  stripRight.begin();
  stripRight.show();

  //set initial brightness
  stripYellow.setBrightness(bright);
  stripLeft.setBrightness(bright);
  stripRight.setBrightness(bright);

  pinMode(BUTTON_YELLOW,INPUT);
  pinMode(BUTTON_RED,INPUT);
  pinMode(BUTTON_BLUE,INPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON_RED), pinRed_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BUTTON_BLUE), pinBlue_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BUTTON_YELLOW), pinYellow_ISR, HIGH);  
}

void loop() {
  if (currentBumperColor == 0) {
    bumperColor[0]=255;
    bumperColor[1]=0;
    bumperColor[2]=0;
  } else {
    bumperColor[0]=0;
    bumperColor[1]=0;
    bumperColor[2]=255;
  }
  if (currentYellowEffect == 0) {
    // Play effect for strips
    Chaser(0,0,3,1,NUM_LEDS, 235, 80, 0, currentYellowEffect);
  } else {
    // Play effect for strips
    Chaser(3,3,0,-1,NUM_LEDS, 235, 80, 0, currentYellowEffect);
  }
}

// --------------------------------- Functions
void Chaser(int q, int qmin, int qmax, int qinc, int LED_cnt, int c1, int c2, int c3, int effect) {

    do { 
      for (int i=0; i < LED_cnt; i=i+3) {
        setPixelYellow(i+q, 235, 80, 0);
        if (i+q <= NUM_LEDS_RB) {
          setPixelPosts(i+q, bumperColor[0], bumperColor[1], bumperColor[2]);
        }
      }
      
      stripYellow.show();
      stripLeft.show();
      stripRight.show();
      
      delay(ChaseDelay);

      for (int i=0; i < LED_cnt; i=i+3) {
        setPixelYellow(i+q, 0,0,0);
        if (i+q <= NUM_LEDS_RB) {
          setPixelPosts(i+q, 0, 0, 0);
        }
      }
      q=q+qinc;
    } while ((effect == 0 && q < qmax) or (effect == 1 && q > qmax));
}

void setPixelYellow(int Pixel, int red, int green, int blue) {
  stripYellow.setPixelColor(Pixel, stripYellow.Color(red, green, blue));
}

void setPixelPosts(int Pixel, int red, int green, int blue) {
  stripLeft.setPixelColor(Pixel, stripLeft.Color(red, green, blue));    
  stripRight.setPixelColor(Pixel, stripRight.Color(red, green, blue));
}

// --------------------------------- Interrupts for button presses
void pinYellow_ISR() {
 
  int reading = digitalRead(BUTTON_YELLOW);
  buttonState = reading;
  if (buttonState == HIGH) {
    x = x + 1;
    //Serial.println(x);
    lastDebounceTime = millis();
  
    do {
      
    } while ((digitalRead(BUTTON_YELLOW) == HIGH) && (millis()-lastDebounceTime > debounceDelay));
    if (buttonState == digitalRead(BUTTON_YELLOW)) {
      currentYellowEffect += 1;
      if (currentYellowEffect > yellowEffectMax) {
        currentYellowEffect = 0;
        x=0;
      }
      //Serial.println(currentYellowEffect);
    }
  }
}

void pinRed_ISR() {
  if (digitalRead(BUTTON_RED) == HIGH) {
    currentBumperColor=0;
  }
}

void pinBlue_ISR() {
  if (digitalRead(BUTTON_BLUE) == HIGH) {
    currentBumperColor=1;
  }
}




