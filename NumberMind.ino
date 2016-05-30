#include "Arduboy.h"

// arduboy display size 128 x 64
Arduboy arduboy;

// global variables

int iNumGen[4];
int iNumInp[4];

// init

void setup()
{

  arduboy.start();
  arduboy.setFrameRate(30);

  // display title
  arduboy.clearDisplay();
  arduboy.setTextSize(2);
  arduboy.setCursor(23, 20);
  arduboy.print("ARDUBOY");
  arduboy.setTextSize(1);
  arduboy.setCursor(35, 41);
  arduboy.print("Number Mind");
  arduboy.display();

  delay(2000);

  arduboy.initRandomSeed();

}

// main loop

void loop()
{

  boolean bRun;

  int iDigitIndex;
  int iTryIndex;

  int iCursorX = 90;
  int iCursorY = 52;

  while (true) {

    genNum();

    // default 1st number
    for (int i = 0; i <= 3; i++) {
      iNumInp[i] = i + 1;
    }

    bRun = true;

    iDigitIndex = 0;
    iTryIndex = 0;

    arduboy.clearDisplay();

    // right side info
    arduboy.drawRect(75, 0, 53, 24, 1);
    arduboy.drawRect(75, 23, 53, 26, 1);
    arduboy.drawRect(75, 48, 53, 16, 1);
    arduboy.setCursor(85, 3);
    arduboy.print("Number");
    arduboy.setCursor(91, 13);
    arduboy.print("Mind");
    arduboy.setCursor(85, 27);
    arduboy.print("A=Ace");
    arduboy.setCursor(85, 37);
    arduboy.print("G=Good");

    while (bRun)
    {

      if (arduboy.pressed(RIGHT_BUTTON)) {
        // move to right digit
        iDigitIndex++;
        if ( iDigitIndex > 3 ) {
          iDigitIndex = 0;
        }
      }

      if (arduboy.pressed(LEFT_BUTTON)) {
        // move to left digit
        iDigitIndex--;
        if ( iDigitIndex < 0 ) {
          iDigitIndex = 3;
        }
      }

      if (arduboy.pressed(UP_BUTTON)) {
        // increase digit
        iNumInp[iDigitIndex]++;
        if ( iNumInp[iDigitIndex] > 9 ) {
          iNumInp[iDigitIndex] = 0;
        }
      }

      if (arduboy.pressed(DOWN_BUTTON)) {
        // decrease digit
        iNumInp[iDigitIndex]--;
        if ( iNumInp[iDigitIndex] < 0 ) {
          iNumInp[iDigitIndex] = 9;
        }
      }

      if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) {
        // compare number
        if (cmpNum(iTryIndex)) {
          // next try
          iTryIndex++;
        }
        else
        {
          // next game
          bRun = false;
        }
      }

      // input number
      for (int i = 0; i <= 3; i++) {
        arduboy.setCursor(iCursorX + (i * 7), iCursorY);
        arduboy.print(iNumInp[i]);
      }
      arduboy.drawLine(iCursorX, iCursorY + 9, iCursorX + 25, iCursorY + 9, 0);
      arduboy.drawLine(iCursorX + (iDigitIndex * 7), iCursorY + 9, iCursorX + 4 + (iDigitIndex * 7), iCursorY + 9, 1);
      arduboy.display();

      // delay for dpad buttons
      delay(200);

      if (!bRun) {
        // game won or lost
        waitButtonPress();
        // next game
        break;
      }

    }

  }

}

// wait for button a or b press

void waitButtonPress()
{

  while (arduboy.notPressed(A_BUTTON) && arduboy.notPressed(B_BUTTON)) {
    // continue until buttons are pressed
  }
  waitButtonRelease();

}

// wait for button a or b release

void waitButtonRelease()
{

  while (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) {
    // continue until buttons are released
  }

}

// generate number

void genNum()
{

  // generate 4 digit number
  for (int i = 0; i <= 3; i++) {
    iNumGen[i] = int(random(10));
  }

}

// compare number

boolean cmpNum(int iTry)
{

  int iCursorX = 90;
  int iCursorY = 38;

  int iTotDigits = 0;
  int iTotAces = 0;

  int iNumGenTmp[4];
  int iNumInpTmp[4];

  char cResult[5];

  // copy numbers
  for (int i = 0; i <= 3; i++) {
    iNumGenTmp[i] = iNumGen[i];
    iNumInpTmp[i] = iNumInp[i];
  }

  // look for aces
  for (int i = 0; i <= 3; i++) {
    if ( iNumInpTmp[i] == iNumGenTmp[i] ) {
      cResult[iTotDigits] = 'A';
      iNumGenTmp[i] = -1;
      iNumInpTmp[i] = -2;
      iTotDigits++;
      iTotAces++;
    }
  }

  // look for good
  for (int i = 0; i <= 3; i++) {
    for (int j = 0; j <= 3; j++) {
      if ( iNumInpTmp[i] == iNumGenTmp[j] ) {
        cResult[iTotDigits] = 'G';
        iNumGenTmp[j] = -1;
        iNumInpTmp[i] = -2;
        iTotDigits++;
      }
    }
  }

  cResult[iTotDigits] = '\0';

  // display result
  arduboy.setCursor(0, iTry * 9);
  arduboy.print(iTry + 1);
  arduboy.print(" ");
  for (int i = 0; i <= 3; i++) {
    arduboy.print(iNumInp[i]);
  }
  arduboy.print(" ");
  arduboy.print(cResult);
  arduboy.display();

  waitButtonRelease();

  if ( iTotAces == 4 ) {
    // won
    arduboy.fillRect(76, 24, 51, 24, 0);
    arduboy.setCursor(92, 32);
    arduboy.print("WON!");
    return false;
  }

  if ( iTry == 6 ) {
    // lost
    arduboy.fillRect(76, 24, 51, 24, 0);
    arduboy.setCursor(91, 27);
    arduboy.print("LOST");
    for (int i = 0; i <= 3; i++) {
      arduboy.setCursor(iCursorX + (i * 7), iCursorY);
      arduboy.print(iNumGen[i]);
    }
    return false;
  }

  if ( iTry == 0 ) {
    // default 2nd number
    for (int i = 0; i <= 3; i++) {
      iNumInp[i] = i + 5;
    }
  }

  if ( iTry == 1 ) {
    // default 3rd number
    for (int i = 0; i <= 3; i++) {
      iNumInp[i] = 0;
    }
  }

  // next try
  return true;

}

