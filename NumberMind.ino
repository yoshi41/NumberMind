#include "Arduboy.h"

// arduboy display size 128 x 64
Arduboy arduboy;

// global variables

int iNumGen[4];
int iNumInp[4];

int iGameIndex = 0;
int iTryIndex;
int iDigitIndex;

int iDpadDelay = 200;

int iCursorX;
int iCursorY;

boolean bRun;

// init

void setup()
{

  arduboy.begin();

  // display title
  arduboy.clear();
  arduboy.setTextSize(2);
  arduboy.setCursor(23, 20);
  arduboy.print("ARDUBOY");
  arduboy.setTextSize(1);
  arduboy.setCursor(35, 41);
  arduboy.print("Number Mind");
  arduboy.display();

  delay(2000);

  arduboy.initRandomSeed();

  arduboy.clear();

}

// main loop

void loop()
{

  genNum();

  // default 1st number
  for (int i = 0; i <= 3; i++) {
    iNumInp[i] = i + 1;
  }

  bRun = true;

  iDigitIndex = 0;
  iTryIndex = 0;

  // clear screen except stats bar
  arduboy.fillRect(0, 0, 72, 64, 0);
  arduboy.fillRect(81, 0, 128, 64, 0);
  // right side info
  arduboy.drawRect(73, 0, 9, 64, 1);
  arduboy.drawRect(81, 0, 47, 24, 1);
  arduboy.drawRect(81, 23, 47, 26, 1);
  arduboy.drawRect(81, 48, 47, 16, 1);
  arduboy.setCursor(87, 3);
  arduboy.print("Number");
  arduboy.setCursor(93, 13);
  arduboy.print("Mind");
  arduboy.setCursor(87, 27);
  arduboy.print("A=Ace");
  arduboy.setCursor(87, 37);
  arduboy.print("G=Good");

  while (bRun)
  {

    if (arduboy.pressed(RIGHT_BUTTON)) {
      // move to right digit
      iDigitIndex++;
      if ( iDigitIndex > 3 ) {
        iDigitIndex = 0;
      }
      delay(iDpadDelay);
    }

    if (arduboy.pressed(LEFT_BUTTON)) {
      // move to left digit
      iDigitIndex--;
      if ( iDigitIndex < 0 ) {
        iDigitIndex = 3;
      }
      delay(iDpadDelay);
    }

    if (arduboy.pressed(UP_BUTTON)) {
      // increase digit
      iNumInp[iDigitIndex]++;
      if ( iNumInp[iDigitIndex] > 9 ) {
        iNumInp[iDigitIndex] = 0;
      }
      delay(iDpadDelay);
    }

    if (arduboy.pressed(DOWN_BUTTON)) {
      // decrease digit
      iNumInp[iDigitIndex]--;
      if ( iNumInp[iDigitIndex] < 0 ) {
        iNumInp[iDigitIndex] = 9;
      }
      delay(iDpadDelay);
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
        iGameIndex++;
        bRun = false;
      }
    }

    // input number
    iCursorX = 92;
    iCursorY = 52;
    for (int i = 0; i <= 3; i++) {
      arduboy.setCursor(iCursorX + (i * 7), iCursorY);
      arduboy.print(iNumInp[i]);
    }
    arduboy.drawLine(iCursorX, iCursorY + 9, iCursorX + 25, iCursorY + 9, 0);
    arduboy.drawLine(iCursorX + (iDigitIndex * 7), iCursorY + 9, iCursorX + 4 + (iDigitIndex * 7), iCursorY + 9, 1);
    arduboy.display();

    if (!bRun) {
      // game won or lost
      waitButtonPress();
    }

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
    displayStat(iTry);
    waitButtonPress();
    arduboy.fillRect(82, 24, 45, 24, 0);
    arduboy.setCursor(94, 32);
    arduboy.print("WON!");
    return false;
  }

  if ( iTry == 6 ) {
    // lost
    displayStat(-1);
    waitButtonPress();
    arduboy.fillRect(82, 24, 45, 24, 0);
    arduboy.setCursor(93, 27);
    arduboy.print("LOST");
    iCursorX = 92;
    iCursorY = 38;
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

// display statistic

void displayStat(int iTry)
{

  int iCursorY = 61 - (iGameIndex - (62 * (iGameIndex / 62)));

  arduboy.drawLine(74, iCursorY, 80, iCursorY, 1);

  iCursorY++;
  
  arduboy.drawLine(74, iCursorY, 80, iCursorY, 0);

  switch (iTry) {
    case -1:
      break;
    case 6:
      arduboy.drawLine(77, iCursorY, 77, iCursorY, 1);
      break;
    case 5:
      arduboy.drawLine(76, iCursorY, 78, iCursorY, 1);
      break;
    default:
      arduboy.drawLine(75, iCursorY, 79, iCursorY, 1);
      break;
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
