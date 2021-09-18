/*******************************************************************
    WIP: Snake game using a 64x64 RGB LED Matrix,
    an ESP32 and a Wii Nunchuck.

    Parts:
    ESP32 D1 Mini * - https://s.click.aliexpress.com/e/_dSi824B
    ESP32 I2S Matrix Shield (From my Tindie) = https://www.tindie.com/products/brianlough/esp32-i2s-matrix-shield/

 *  * = Affilate

    If you find what I do useful and would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/


    Written by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/

// ----------------------------
// Standard Libraries
// ----------------------------

#include <Wire.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <NintendoExtensionCtrl.h>
// This library is for interfacing with the Nunchuck

// Can be installed from the library manager
// https://github.com/dmadison/NintendoExtensionCtrl

#include <ESP32-RGB64x32MatrixPanel-I2S-DMA.h>
// This is the library for interfacing with the display

// Can be installed from the library manager (Search for "ESP32 64x32 LED MATRIX")
// https://github.com/mrfaptastic/ESP32-RGB64x32MatrixPanel-I2S-DMA


// Adafruit GFX library is a dependancy for the matrix Library
// Can be installed from the library manager
// https://github.com/adafruit/Adafruit-GFX-Library

//--------------------------------
//Game Config Options:
//--------------------------------

//Display settings
#define SCREEN_X 64
#define SCREEN_Y 64

#define WORLD_TO_PIXEL 1

#define WORLD_X 64
#define WORLD_Y 64

//Snake Settings
#define SNAKE_START_LENGTH 10

// Fat Snake - Woohooo
//#define WORLD_TO_PIXEL 3

//#define WORLD_X 21
//#define WORLD_Y 21

//Snake Settings
//#define SNAKE_START_LENGTH 3

//Game Settings
#define DELAY_BETWEEN_FRAMES 300 // smaller == faster snake
#define BOOSTED_DELAY_BETWEEN_FRAMES 25
//--------------------------------

//--------------------------------
//Pin Definitions:
//--------------------------------

//--------------------------------

unsigned long delayBetweenFrames = DELAY_BETWEEN_FRAMES;

Nunchuk nchuk;   // Controller on bus #1

RGB64x32MatrixPanel_I2S_DMA dma_display;


uint16_t myRED = dma_display.color565(255, 0, 0);
uint16_t myGREEN = dma_display.color565(0, 255, 0);
uint16_t myBLUE = dma_display.color565(0, 0, 255);
uint16_t myWHITE = dma_display.color565(255, 255, 255);
uint16_t myYELLOW = dma_display.color565(255, 255, 0);
uint16_t myCYAN = dma_display.color565(0, 255, 255);
uint16_t myMAGENTA = dma_display.color565(255, 0, 255);
uint16_t myBLACK = dma_display.color565(0, 0, 0);

uint16_t snakeBodyColour = myMAGENTA;
uint16_t snakeHeadColour = myGREEN;
uint16_t snakeBodyColourDead = myRED;

uint16_t appleColour = myMAGENTA;

int controllerX = 0;
int controllerY = 0;

int moveThreshold = 30;

unsigned long updateDue = 0;

int appleX = -1;
int appleY = -1;
boolean appleHidden = true;

struct snakeLink
{
  int x;
  int y;
  snakeLink *next;
};

struct snake
{
  int xSpeed = 0;
  int ySpeed = 0;
  int movingX = 0;
  int movingY = 0;
  boolean ateAppleLastTurn = false;
  boolean alive = true;
  snakeLink *head;
};

snake *player;

void initSnake(int snakeLength, int x, int y) {
  snakeLink *current;
  snakeLink *previous = NULL;
  for (int i = x - snakeLength + 1; i <= x; i++) {
    current = new snakeLink;
    current->x = i;
    current->y = y;
    current->next = previous;
    previous = current;
  }

  player = new snake;
  player->head = current;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dma_display.begin();



  nchuk.begin();

  while (!nchuk.connect()) {
    Serial.println("Nunchuk on bus #1 not detected!");
    dma_display.setCursor(5, 0);
    dma_display.setTextColor(myBLUE);
    dma_display.println("No");
    dma_display.setCursor(5, 9);
    dma_display.setTextColor(myRED);
    dma_display.println("Nunchuck");
    dma_display.setCursor(5, 18);
    dma_display.setTextColor(myGREEN);
    dma_display.println("detected!");
    delay(1000);
  }

  //Serial.println("Snake init start");
  initSnake(SNAKE_START_LENGTH, WORLD_X / 2, WORLD_Y / 2);

  //Serial.println("Snake init done");
}

void readControllerInput()
{
  boolean success = nchuk.update();  // Get new data from the controller

  if (!success) {  // Ruh roh
    Serial.println("Controller disconnected!");
    delay(1000);
  }
  else {

    if (nchuk.buttonC()) {
      initSnake(SNAKE_START_LENGTH, WORLD_X / 2, WORLD_Y / 2);
    }

    if (nchuk.buttonZ()) {
      delayBetweenFrames = BOOSTED_DELAY_BETWEEN_FRAMES;
    } else {
      delayBetweenFrames = DELAY_BETWEEN_FRAMES;
    }


    // Read a joystick axis (0-255, X and Y)
    int joyY = nchuk.joyY();
    int joyX = nchuk.joyX();

    if (joyX > 127 + moveThreshold) {
      controllerX = 1;
    } else if (joyX < 127 - moveThreshold) {
      controllerX = -1;
    } else {
      controllerX = 0;
    }

    if (joyY > 127 + moveThreshold) {
      controllerY = -1;
    } else if (joyY < 127 - moveThreshold) {
      controllerY = 1;
    } else {
      controllerY = 0;
    }
  }
}

void calculateSnakeMovement() {

  if (controllerX != 0 || controllerY != 0) {
    //We have some update
    if (player->movingX != 0) {
      // Snake is moving on the X
      if (controllerY != 0)
      {
        player->xSpeed = 0;
        player->ySpeed = controllerY;
      } else if (controllerX == player->movingX) {
        player->ySpeed = 0;
        player->xSpeed = controllerX;
      }
    } else if (player->movingY != 0) {
      // Snake is moving on the Y
      if (controllerX != 0)
      {
        player->ySpeed = 0;
        player->xSpeed = controllerX;
      } else if (controllerY == player->movingY) {
        player->xSpeed = 0;
        player->ySpeed = controllerY;
      }
    } else {
      // Snake is not moving
      if (controllerX == 1)
      {
        //-1 instantly killed the snake!
        player->xSpeed = controllerX;
      } else {
        // will just be 0 if it's not
        // being pressed.
        player->ySpeed = controllerY;
      }
    }
  }
}

void processSnake() {

  if (player->alive)
  {
    int newHeadX = player->head->x + player->xSpeed;
    int newHeadY = player->head->y + player->ySpeed;

    player->movingX = player->xSpeed;
    player->movingY = player->ySpeed;

    if (newHeadX >= WORLD_X) {
      newHeadX = 0;
    } else if (newHeadX < 0) {
      newHeadX = WORLD_X - 1;
    }

    if (newHeadY >= WORLD_Y) {
      newHeadY = 0;
    } else if (newHeadY < 0) {
      newHeadY = WORLD_Y - 1;
    }

    snakeLink *current;
    snakeLink *previous;
    current = player->head;
    boolean collision = false;
    while (current->next != NULL)
    {
      collision = collision || ((current->x == newHeadX) && (current->y == newHeadY));
      previous = current;
      current = current->next;
    }

    player->alive = !collision;

    if (player->ateAppleLastTurn) {
      // Snake ate apple last turn, so we create a new
      // link as the head.
      player->ateAppleLastTurn = false;
      snakeLink *newHead = new snakeLink;
      newHead->x = newHeadX;
      newHead->y = newHeadY;
      newHead->next = player->head;
      player->head = newHead;
    } else {
      // Move tail to the head, and make the second
      // last link the tail

      //current is the old tail
      current->x = newHeadX;
      current->y = newHeadY;
      current->next = player->head;
      player->head = current;

      //previous is the new tail
      previous->next = NULL;
    }

    //Serial.print("Count: ");
    //Serial.println(count);
  } else {
    //Serial.print("No movement yet");
  }

}

void processApple() {
  boolean appleEaten = ((player->head->x == appleX) && (player->head->y == appleY));
  if (appleHidden || appleEaten) {
    if (appleHidden) {
      appleHidden = false;
    }

    if (appleEaten) {
      player->ateAppleLastTurn = true;
    }

    // need new Apple
    boolean verifiedPosition = false;
    while (!verifiedPosition) {
      int randomNum = random(WORLD_X * WORLD_Y);
      appleX = randomNum % WORLD_X;
      appleY = randomNum / WORLD_X;

      Serial.print(appleX);
      Serial.print(",");
      Serial.println(appleY);

      verifiedPosition = true;

      snakeLink *current;
      current = player->head;
      while (current->next != NULL)
      {
        if ((current->x == appleX) && (current->y == appleY)) {
          // apple can't go here, it clashes.
          verifiedPosition = false;
          break;
        }
        current = current->next;
      }

    }
  }
}

void drawBodyPart(int x, int y, uint16_t colour) {
  int realX = (x * WORLD_TO_PIXEL);
  int realy = (y * WORLD_TO_PIXEL);
  if (WORLD_TO_PIXEL > 1) {
    dma_display.drawRect(realX, realy, 3, 3, colour);
  } else {
    dma_display.drawPixel(realX, realy, colour);
  }
}

void drawSnake() {
  snakeLink *tmp = player->head;
  boolean head = true;
  uint16_t snakeColour = player->alive ? snakeBodyColour : snakeBodyColourDead;
  while (tmp != NULL) {
    drawBodyPart(tmp->x, tmp->y, snakeColour);
    tmp = tmp->next;
  }
  //adding a different colour for the head
  drawBodyPart(player->head->x, player->head->y, snakeHeadColour);
}

void drawApple() {
  drawBodyPart(appleX, appleY, appleColour);
}

void loop() {

  readControllerInput();
  calculateSnakeMovement();
  unsigned long now = millis();
  if (now > updateDue) {
    if (player->xSpeed != 0 || player->ySpeed != 0) {
      processSnake();
      processApple();
    }
    //  display.clearDisplay();
    dma_display.fillScreen(myBLACK);

    drawSnake();
    drawApple();
    updateDue = now + delayBetweenFrames;
  }
}
