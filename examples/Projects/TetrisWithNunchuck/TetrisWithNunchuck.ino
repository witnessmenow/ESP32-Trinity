/*******************************************************************
    WIP: Tetris game using a 64x64 RGB LED Matrix,
    an ESP32 and a Wii Nunchuck.

    This is based on from code from onelonecoder.com (@javidx9)

    Original Video (Well worth checking out): https://youtu.be/8OK8_tHeCIA
    Original Code: https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_Tetris.cpp

    Parts Used:
      ESP32 Trinity - https://github.com/witnessmenow/ESP32-Trinity
      Nunchuck Add-on - https://www.tindie.com/products/brianlough

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

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
// This is the library for interfacing with the display

// Can be installed from the library manager (Search for "ESP32 MATRIX DMA")
// https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA


// Adafruit GFX library is a dependency for the matrix Library
// Can be installed from the library manager
// https://github.com/adafruit/Adafruit-GFX-Library

// --------------------------------
// -------   Matrix Config   ------
// --------------------------------

const int panelResX = 64;   // Number of pixels wide of each INDIVIDUAL panel module.
const int panelResY = 64;   // Number of pixels tall of each INDIVIDUAL panel module.
const int panel_chain = 1;  // Total number of panels chained one to another.

//--------------------------------
// Game Config Options:
//--------------------------------

#define LEFT_OFFSET 13
#define TOP_OFFSET 10

#define WORLD_TO_PIXEL 3 //each dot on the game world will be represented by these many pixels.

#define DELAY_BETWEEN_FRAMES 50
#define DELAY_ON_LINE_CLEAR 100

#define NUM_PIECES_TIL_SPEED_CHANGE 20

Nunchuk nchuk;

MatrixPanel_I2S_DMA *dma_display = nullptr;

uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myYELLOW = dma_display->color565(255, 255, 0);
uint16_t myCYAN = dma_display->color565(0, 255, 255);
uint16_t myMAGENTA = dma_display->color565(255, 0, 255);
uint16_t myBLACK = dma_display->color565(0, 0, 0);

// [0] is empty space
// [1-7] are tetromino colours
// [8] is the colour a completed line changes to before disappearing
// [9] is the walls of the board.
uint16_t gameColours[10] = {myBLACK, myRED, myGREEN, myBLUE, myWHITE, myYELLOW, myCYAN, myMAGENTA, myRED, myGREEN};

char tetromino[7][17];
int nFieldWidth = 12;
int nFieldHeight = 18;
char *pField = nullptr;

int Rotate(int px, int py, int r)
{
  int pi = 0;
  switch (r % 4)
  {
    case 0: // 0 degrees        // 0  1  2  3
      pi = py * 4 + px;         // 4  5  6  7
      break;                    // 8  9 10 11
                                //12 13 14 15

    case 1: // 90 degrees       //12  8  4  0
      pi = 12 + py - (px * 4);  //13  9  5  1
      break;                    //14 10  6  2
                                //15 11  7  3

    case 2: // 180 degrees      //15 14 13 12
      pi = 15 - (py * 4) - px;  //11 10  9  8
      break;                    // 7  6  5  4
                                // 3  2  1  0

    case 3: // 270 degrees      // 3  7 11 15
      pi = 3 - py + (px * 4);   // 2  6 10 14
      break;                    // 1  5  9 13
  }                             // 0  4  8 12

  return pi;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
  // All Field cells >0 are occupied
  for (int px = 0; px < 4; px++)
    for (int py = 0; py < 4; py++)
    {
      // Get index into piece
      int pi = Rotate(px, py, nRotation);

      // Get index into field
      int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

      // Check that test is in bounds. Note out of bounds does
      // not necessarily mean a fail, as the long vertical piece
      // can have cells that lie outside the boundary, so we'll
      // just ignore them
      if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
      {
        if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
        {
          // In Bounds so do collision check
          if (tetromino[nTetromino][pi] != L'.' && pField[fi] != 0)
            return false; // fail on first hit
        }
      }
    }

  return true;
}

void displaySetup() {
  HUB75_I2S_CFG mxconfig(
    panelResX,   // Module width
    panelResY,   // Module height
    panel_chain  // Chain length
  );

  // If you are using a 64x64 matrix you need to pass a value for the E pin
  // The trinity connects GPIO 18 to E.
  // This can be commented out for any smaller displays (but should work fine with it)
  mxconfig.gpio.e = 18;

  // This is how you enable the double buffer.
  // Double buffer can help with animation heavy projects
  mxconfig.double_buff = true;

  // May or may not be needed depending on your matrix
  // Example of what needing it looks like:
  // https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA/issues/134#issuecomment-866367216
  mxconfig.clkphase = false;

  // Some matrix panels use different ICs for driving them and some of them have strange quirks.
  // If the display is not working right, try this.
  //mxconfig.driver = HUB75_I2S_CFG::FM6126A;

  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
}

void setup() {
  Serial.begin(115200);

  displaySetup();
  dma_display->fillScreen(myBLACK);

  nchuk.begin();

  while (!nchuk.connect()) {
    Serial.println("Nunchuk on bus #1 not detected!");
    dma_display->setCursor(5, 0);
    dma_display->setTextColor(myBLUE);
    dma_display->println("No");
    dma_display->setCursor(5, 9);
    dma_display->setTextColor(myRED);
    dma_display->println("Nunchuck");
    dma_display->setCursor(5, 18);
    dma_display->setTextColor(myGREEN);
    dma_display->println("detected!");
    dma_display->flipDMABuffer();
    delay(1000);
  }

  strcpy(tetromino[0], "..X...X...X...X."); // Tetronimos 4x4
  strcpy(tetromino[1], "..X..XX...X.....");
  strcpy(tetromino[2], ".....XX..XX.....");
  strcpy(tetromino[3], "..X..XX..X......");
  strcpy(tetromino[4], ".X...XX...X.....");
  strcpy(tetromino[5], ".X...X...XX.....");
  strcpy(tetromino[6], "..X...X..XX.....");

  // inits field
  restartGame();

  // Clearing both buffers
  dma_display->fillScreen(myBLACK);
  dma_display->flipDMABuffer();
  dma_display->fillScreen(myBLACK);
  dma_display->flipDMABuffer();
}

bool bGameOver = false;

bool moveLeft;
bool moveRight;
bool dropDown;
bool rotatePiece;
bool cButtonPressed;

bool lastCButtonState;

bool isPaused;

int moveThreshold = 30;

int nCurrentPiece = 2;
int nCurrentRotation = 0;
int nCurrentX = (nFieldWidth / 2) - 2;
int nCurrentY = 0;

int nSpeed = 20;
int nSpeedCount = 0;
bool bForceDown = false;
bool bRotateHold = true;

int nPieceCount = 0;
int nScore = 0;

int completedLinesIndex[4];
int numCompletedLines;

void getNewPiece()
{
  // Pick New Piece
  nCurrentX = (nFieldWidth / 2) - 2;
  nCurrentY = 0;
  nCurrentRotation = 0;
  nCurrentPiece = random(7);
}

void gameTiming() {
  delay(DELAY_BETWEEN_FRAMES);
  nSpeedCount++;
  bForceDown = (nSpeedCount == nSpeed);
}

void clearLines() {
  if (numCompletedLines > 0)
  {
    delay(DELAY_ON_LINE_CLEAR);
    for (int i = 0; i < numCompletedLines; i ++ )
    {
      for (int px = 1; px < nFieldWidth - 1; px++)
      {
        for (int py = completedLinesIndex[i]; py > 0; py--)
          pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
        pField[px] = 0;
      }
    }
    numCompletedLines = 0;
  }
}

void gameLogic() {

  //Handle updating lines cleared last tick
  clearLines();

  nCurrentX += (moveRight && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
  nCurrentX -= (moveLeft && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
  nCurrentY += (dropDown && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

  if (rotatePiece)
  {
    nCurrentRotation += (bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
    bRotateHold = false;
  }
  else
    bRotateHold = true;

  // Force the piece down the playfield if it's time
  if (bForceDown)
  {
    // Update difficulty every 50 pieces
    nSpeedCount = 0;
    nPieceCount++;
    if (nPieceCount % NUM_PIECES_TIL_SPEED_CHANGE == 0)
      if (nSpeed >= 10) nSpeed--;

    // Test if piece can be moved down
    if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
      nCurrentY++; // It can, so do it!
    else
    {
      // It can't! Lock the piece in place
      for (int px = 0; px < 4; px++)
        for (int py = 0; py < 4; py++)
          if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != '.')
            pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

      // Check for lines
      for (int py = 0; py < 4; py++)
        if (nCurrentY + py < nFieldHeight - 1)
        {
          bool bLine = true;
          for (int px = 1; px < nFieldWidth - 1; px++)
            bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

          if (bLine)
          {
            // Remove Line, set to =
            for (int px = 1; px < nFieldWidth - 1; px++)
              pField[(nCurrentY + py) * nFieldWidth + px] = 8;
            //vLines.push_back(nCurrentY + py);
            completedLinesIndex[numCompletedLines] = nCurrentY + py;
            numCompletedLines++;

          }
        }

      nScore += 25;
      if (numCompletedLines > 0) nScore += (1 << numCompletedLines) * 100;

      // Pick New Piece
      getNewPiece();

      // If piece does not fit straight away, game over!
      bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
    }
  }
}

void gameInput() {

  boolean success = nchuk.update();  // Get new data from the controller

  if (!success) {  // Ruh roh
    Serial.println("Controller disconnected!");
    delay(1000);
  }
  else {

    rotatePiece = nchuk.buttonZ();

    lastCButtonState = cButtonPressed;
    cButtonPressed = nchuk.buttonC();

    if (cButtonPressed && !lastCButtonState)
    {
      isPaused = !isPaused;
    }

    // Read a joystick axis (0-255, X and Y)
    int joyY = nchuk.joyY();
    int joyX = nchuk.joyX();

    moveRight = (joyX > 127 + moveThreshold);
    moveLeft = (joyX < 127 - moveThreshold);
    dropDown = (joyY < 127 - moveThreshold);

  }

}

uint16_t getFieldColour(int index, bool isDeathScreen) {
  if (isDeathScreen && pField[index] != 0) {
    return myRED;
  } else {
    return gameColours[pField[index]];
  }
}

void displayLogic(bool isDeathScreen = false) {

  dma_display->flipDMABuffer();
  int realX;
  int realY;

  dma_display->fillScreen(myBLACK);

  // Draw Field
  for (int x = 0; x < nFieldWidth; x++)
    for (int y = 0; y < nFieldHeight; y++)
    {
      realX = (x * WORLD_TO_PIXEL) + LEFT_OFFSET;
      realY = (y * WORLD_TO_PIXEL) + TOP_OFFSET;
      uint16_t fieldColour = getFieldColour((y * nFieldWidth + x), isDeathScreen);
      if (fieldColour != myBLACK) {
        if (WORLD_TO_PIXEL > 1) {
          dma_display->drawRect(realX, realY, WORLD_TO_PIXEL, WORLD_TO_PIXEL, fieldColour);
        } else {
          dma_display->drawPixel(realX, realY, fieldColour);
        }
      }
    }

  // Draw Current Piece
  for (int px = 0; px < 4; px++)
    for (int py = 0; py < 4; py++)
      if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != '.')
      {
        realX = ((nCurrentX + px) * WORLD_TO_PIXEL) + LEFT_OFFSET;
        realY = ((nCurrentY + py) * WORLD_TO_PIXEL) + TOP_OFFSET;
        if (WORLD_TO_PIXEL > 1) {
          dma_display->drawRect(realX, realY, WORLD_TO_PIXEL, WORLD_TO_PIXEL, gameColours[nCurrentPiece + 1]);
        } else {
          dma_display->drawPixel(realX, realY, gameColours[nCurrentPiece + 1]);
        }
      }

  dma_display->setTextColor(myBLUE);

  if (isPaused) {
    dma_display->setCursor(20, 1);
    dma_display->print("Paws");
  } else {
    // Display the Score

    // As it changes, we need to calculate where
    // the center is
    int16_t  x1, y1;
    uint16_t w, h;
    dma_display->getTextBounds(String(nScore), 0, 0, &x1, &y1, &w, &h);
    dma_display->setCursor((panelResX/2) - w/2, 1);
    dma_display->print(nScore);
  }

  dma_display->flipDMABuffer();
}

void restartGame() {

  bGameOver = false;
  pField = new char[nFieldWidth * nFieldHeight]; // Create play field buffer
  for (int x = 0; x < nFieldWidth; x++) // Board Boundary
    for (int y = 0; y < nFieldHeight; y++)
      pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;

  // Pick New Piece
  getNewPiece();

  nScore = 0;

}

void loop() {
  // put your main code here, to run repeatedly:
  if (!bGameOver)
  {
    gameInput();
    if (!isPaused)
    {
      gameTiming();
      gameLogic();
    } else {
      delay(DELAY_BETWEEN_FRAMES); //stopping pulsing of LEDS
    }
    displayLogic();
  } else {
    delay(DELAY_BETWEEN_FRAMES);
    gameInput();
    displayLogic(true);
    if (cButtonPressed)
    {
      restartGame();
    }
  }
}
