#include <LedControl.h>
#include <LiquidCrystal.h>

#define PAD_WIDTH 3
#define HIT_NULL 0
#define HIT_LEFT 1
#define HIT_CENTER 2
#define HIT_RIGHT 3
#define DOT_START_SPEED 200
#define GAME_SPEED 10
#define HORIZONTAL_BOUNCE -1
#define VERTICAL_BOUNCE 1
#define GAME_OFF 0
#define GAME_ON 1
#define GAME_OVER -1
#define LCD_RS 2
#define LCD_EN 3
#define LCD_DIGIT_4 6
#define LCD_DIGIT_5 7
#define LCD_DIGIT_6 8
#define LCD_DIGIT_7 9
#define LCD_V0 A1
#define MAX7219_PIN_1 13
#define MAX7219_CLK 12
#define MAX7219_LOAD 11
#define JOYSTICK_MOVE_PIN A0
#define JOYSTICK_CLICK_PIN 1
#define JOYSTICK_BUTTON_CLICKED LOW
#define PAD_START_RIGHT_POSITION 2
#define PAD_SPEED 100

const byte three[] = {
  B00000000,
  B01111110,
  B00000010,
  B01111110,
  B00000010,
  B01111110,
  B00000000,
  B00000000
  }; 
const byte two[] = {
  B00000000,
  B01111110,
  B00000010,
  B01111110,
  B01000000,
  B01111110,
  B00000000,
  B00000000
  };
const byte one[] = {
  B00000000,
  B00000010,
  B00000010,
  B00000010,
  B00000010,
  B00000010,
  B00000000,
  B00000000
  };

LedControl ledControl (MAX7219_PIN_1, MAX7219_CLK, MAX7219_LOAD, 1);
LiquidCrystal lcd (LCD_RS, LCD_EN, LCD_DIGIT_4, LCD_DIGIT_5, LCD_DIGIT_6, LCD_DIGIT_7);
unsigned long currentArduinoTime;

int customDisplayOnMatrix(byte *toDisplay);


class Dot 
{
private:
  int x;
  int y;
  int yPrevious;
  int direction;
  int speed;
public:
  int setX(int x)
  {
    this->x = x;
    return x;
  }
  int setY(int y)
  {
    this->y = y;
    return y;
  }
  int setYPrevious(int yPrevious)
  {
    this->yPrevious = yPrevious;
    return yPrevious;
  }
  int setDirection(int direction)
  {
    this->direction = direction;
    return direction;
  }
  int setSpeed(int speed)
  {
    this->speed = speed;
    return speed;
  }
  int getX()
  {
    return this->x;
  }
  int getY()
  {
    return this->y;
  }
  int getYPrevious()
  {
    return this->yPrevious;
  }
  int getDirection()
  {
    return this->direction;
  }
  int getSpeed()
  {
    return this->speed;
  }
  int setStart()
  {
    this->x = random(1,7);
    this->y = 6;
    this->yPrevious = 6;
    this->direction = random(3,6);
  }
  int increaseSpeed()
  {
    if (this->speed > 80)
    {
      this->speed = this->speed - 10;
    }
    return this->speed;
  }
} dot;

class Pad
{
private:
  int marginRight;
  int marginLeft;
  int padSize;
public:
  int setMarginRight(int marginRight)
  {
    this->marginRight = marginRight;
    return marginRight;
  }
  int setMarginLeft(int marginLeft)
  {
    this->marginLeft = marginLeft;
    return marginLeft;
  }
  int setPadSize(int padSize)
  {
    this->padSize = padSize;
    return padSize;
  }
  int getMarginRight()
  {
    return this->marginRight;
  }
  int getMarginLeft()
  {
    return this->marginLeft;
  }
  int getPadSize()
  {
    return this->padSize;
  }
  int movePad()
  {
    int joystickMoveInput = analogRead(JOYSTICK_MOVE_PIN);
    if (joystickMoveInput >= 723 && this->marginLeft < 7)
      {
        ++(this->marginRight);
        ++(this->marginLeft);
      }
    else if (joystickMoveInput <= 300 && this->marginRight > 0)
      {
        --(this->marginRight);
        --(this->marginLeft);
      }
    return this->marginRight;
  }
} pad;

class Game
{
private:
  int currentScore;
  int bestScore;
  int gameState;
  unsigned long timeSinceGameStarted;
  unsigned long timeWhenGameStarted;
  unsigned long timeWhenGameFinished;
public:
  Game()
  {
    this->bestScore = 0;
    this->currentScore = 0;
    this->gameState = GAME_OFF;
  }
  unsigned long setTimeSinceGameStarted(int timeSinceGameStarted)
  {
    this->timeSinceGameStarted = timeSinceGameStarted;
    return timeSinceGameStarted;
  }
  unsigned long setTimeWhenGameStarted(int timeWhenGameStarted)
  {
    this->timeWhenGameStarted = timeWhenGameStarted;
    return timeWhenGameStarted;
  }
  unsigned long setTimeWhenGameFinished(int timeWhenGameFinished)
  {
    this->timeWhenGameFinished = timeWhenGameFinished;
    return timeWhenGameFinished;
  }
  int setCurrentScore(int currentScore)
  {
    this->currentScore = currentScore;
    return currentScore;
  }
  int setBestScore(int bestScore)
  {
    this->bestScore = bestScore;
    return bestScore;
  }
  int setGameState(int gameState)
  {
    this->gameState =  gameState;
    return gameState;
  }
  unsigned long getTimeSinceGameStarted()
  {
    return this->timeSinceGameStarted;
  }
  unsigned long getTimeWhenGameStarted()
  {
    return this->timeWhenGameStarted;
  }
  unsigned long getTimeWhenGameFinished()
  {
    return this->timeWhenGameFinished;
  }
  int getCurrentScore()
  {
    return this->currentScore;
  }
  int getBestScore()
  {
    return this->bestScore;
  }
  int getGameState()
  {
    return this->gameState;
  }
  int incrementCurrentScore()
  {
    ++(this->currentScore);
    return this->currentScore;
  }
  int displayTimer()
  {
    if (this->timeSinceGameStarted == 1)
    {
      lcd.clear();
      lcd.print("Score: ");
      lcd.setCursor(7,0);
      lcd.print(this->currentScore);
      lcd.setCursor(0,1);
      lcd.print("Best: ");
      lcd.setCursor(6,1);
      lcd.print(this->bestScore);
      customDisplayOnMatrix(three);
    }
    else if (this->timeSinceGameStarted == 1001)
    {
      customDisplayOnMatrix(two);
    }
    else if (this->timeSinceGameStarted == 2001)
    {
      customDisplayOnMatrix(one);
    }
    return 0;
  }
  int drawGameMatrix()
  {
    int dotX = dot.getX();
    int dotY = dot.getY();
    int dotYLastPosition = dot.getYPrevious();
    int padRight = pad.getMarginRight();
    if (dotYLastPosition != dotY)
    {
      ledControl.setRow(0, dotYLastPosition, 0);
    }
    byte dotRow = 1 << dotX;
    ledControl.setRow(0, dotY, dotRow);
    byte padRow = byte(255 >> (8 - PAD_WIDTH) << padRight);
    ledControl.setRow(0, 0, padRow);
    return 0;
  }
} game;

void setup()
{
  pinMode(LCD_V0, OUTPUT);
  pinMode(JOYSTICK_CLICK_PIN, INPUT);
  pinMode(JOYSTICK_MOVE_PIN, INPUT);
  digitalWrite(JOYSTICK_CLICK_PIN, JOYSTICK_BUTTON_CLICKED);

  randomSeed(analogRead(0));

  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Click to start!");
  lcd.setCursor(0,1);
  lcd.print("Best: ");
  lcd.setCursor(6,1);
  lcd.print(game.getBestScore());
  
  Serial.begin(9600);
  
  ledControl.shutdown(0,false);
  ledControl.setIntensity(0,8);
  ledControl.clearDisplay(0);

  pad.setPadSize(PAD_WIDTH);
  pad.setMarginRight(PAD_START_RIGHT_POSITION);
  pad.setMarginLeft(pad.getMarginRight() + 2);

  dot.setSpeed(DOT_START_SPEED);
}

void loop()
{
  currentArduinoTime = millis();
  int gameState = game.getGameState();
  if (gameState == GAME_OFF)
  {
    int joystickButtonState = digitalRead(JOYSTICK_CLICK_PIN);
    if (joystickButtonState == JOYSTICK_BUTTON_CLICKED)
      {
        game.setGameState(GAME_ON);
        game.setTimeWhenGameStarted(currentArduinoTime);
      }
  }
  else if (gameState == GAME_ON)
  {
    unsigned long timeSinceGameStarted = game.setTimeSinceGameStarted(currentArduinoTime - game.getTimeWhenGameStarted());
    if (timeSinceGameStarted <= 3000)
    {
      game.displayTimer();
    }
    else if (timeSinceGameStarted == 3001)
    {
      dot.setStart();
      ledControl.clearDisplay(0);
    }
    else
    {
      int dotSpeed = dot.getSpeed();
      if (timeSinceGameStarted % PAD_SPEED == 0)
      {
        pad.movePad();
      }
      if (timeSinceGameStarted % dotSpeed == 0)
      {
        moveDotFunction();
      }
      game.drawGameMatrix();
      if (checkIfLost())
      {
        game.setTimeWhenGameFinished(currentArduinoTime);
        game.setGameState(GAME_OVER);
      }
      if (timeSinceGameStarted % 15000 == 0)
      {
        dot.increaseSpeed();
      }
    }
  }
  else if (gameState == GAME_OVER)
  {
    unsigned int timeSinceGameOver = currentArduinoTime - game.getTimeWhenGameFinished();
    if (timeSinceGameOver == 3000)
    {
      if (game.getBestScore() < game.getCurrentScore())
      {
        game.setBestScore(game.getCurrentScore());
      }
      game.setCurrentScore(0);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Click to start!");
      lcd.setCursor(0,1);
      lcd.print("Best: ");
      lcd.setCursor(6,1);
      lcd.print(game.getBestScore());
      game.setGameState(GAME_OFF);
    }
  }
}

int customDisplayOnMatrix(byte *toDisplay)
{
    for(int row = 7; row >= 0; row--)
    {
        ledControl.setRow(0, row, toDisplay[7-row]);
    }
    return 0;
}

void gameIsOver()
{
  ledControl.clearDisplay(0); 
}

int checkWalls()
{
  int dotX = dot.getX();
  int dotY = dot.getY();

  if (dotX == 0 || dotX == 7 || dotY == 1 || dotY == 7)
  {
    int wallBounce;
    if (dotY == 1 || dotY == 7)
    {
      wallBounce = HORIZONTAL_BOUNCE;
    }
    else
    {
      wallBounce = VERTICAL_BOUNCE;
    }
    return wallBounce;
  }
  return 0; 
}

int checkPadHit()
{
  int dotY = dot.getY();
  int dotX = dot.getX();
  int padRight = pad.getMarginRight();
  int padLeft = pad.getMarginLeft();
  int padSize = pad.getPadSize();

  if (dotY != 1 || dotX < padRight || dotX > padLeft)
  {
    return HIT_NULL;
  }
  if (dotX == padRight + padSize/2 && padSize % 2 == 1)
  {
    return HIT_CENTER;
  }
  if (dotX > padRight + padSize/2)
  {
    return HIT_RIGHT;
  }
  return HIT_LEFT;
}

bool checkIfLost()
{
  if (dot.getY() == 1 && checkPadHit() == HIT_NULL)
  {
    return true;
  }
  return false;
}

void moveDotFunction()
{
  int wallBounce = checkWalls();
  int dotDirection = dot.getDirection();
  int dotX = dot.getX();
  int dotY = dot.getY();

  if (wallBounce != 0)
  {
    switch(dotDirection)
    {
      case 0:
        dotDirection = 4;
        break;
      case 4:
        dotDirection = 0;
        break;
      case 2:
        dotDirection = 6;
        break;
      case 6:
        dotDirection = 2;
        break;
      case 1:
        if (wallBounce == VERTICAL_BOUNCE)
        {
          dotDirection = 7;
        }
        else
        {
          dotDirection = 3;
        }
        break;
      case 5:
        if (wallBounce == VERTICAL_BOUNCE)
        {
          dotDirection = 3;
        }
        else
        {
          dotDirection = 7;
        }
        break;
      case 3:
        if (wallBounce == VERTICAL_BOUNCE)
        {
          dotDirection = 5;
        }
        else
        {
          dotDirection = 1;
        }
        break;
      case 7:
        if (wallBounce == VERTICAL_BOUNCE)
        {
          dotDirection = 1;
        }
        else
        {
          dotDirection = 5;
        }
        break;
    }
  }

  int padHit = checkPadHit();
  if (padHit == HIT_LEFT || padHit == HIT_CENTER || padHit == HIT_RIGHT)
  {
    lcd.setCursor(7,0);
    lcd.print(game.incrementCurrentScore());
  }
  switch(padHit)
  {
    case HIT_LEFT:
      if (dotDirection == 0)
      {
        dotDirection = 7;
      }
      else if (dotDirection == 1)
      {
        dotDirection = 0;
      }
      break;
    case HIT_RIGHT:
      if (dotDirection == 0) 
      {
        dotDirection == 1;
      }
      else if (dotDirection == 7)
      {
        dotDirection = 0;
      }
      break;
    default:
      break;
  }

  if ((dotDirection == 4 && dotX == 7) || (dotDirection == 0 && dotX == 0))
  {
    dotDirection ++;
  }
  if (dotDirection == 0 && dotX ==7)
  {
    dotDirection = 7;
  }
  if (dotDirection == 4 && dotX == 7)
  {
    dotDirection = 3;
  }
  if (dotDirection == 2 && dotY == 7)
  {
    if (dotY == 7)
    {
      dotDirection = 3;
    }
    else if (dotY == 1)
    {
      dotDirection = 1;
    }
  }
  if (dotDirection == 2)
  {
    if (dotY == 7)
    {
      dotDirection = 5;
    }
    else if (dotY == 1)
    {
      dotDirection = 7;
    }
  }

  if (dotX == 0)
  {
    if (dotY == 7)
    {
      dotDirection = 3;
    }
    else if (dotY == 1)
    {
      dotDirection = 1;
    }
  }
  if (dotX == 7)
  {
    if (dotY == 7)
    {
      dotDirection = 5;
    }
    else if (dotY == 1)
    {
      dotDirection = 7;
    }
  }
  dot.setDirection(dotDirection);
  
  dot.setYPrevious(dotY);
  if (dotDirection > 2 && dotDirection < 6)
  {
    --dotY;
  }
  else if (dotDirection != 2 && dotDirection != 6)
  {
    ++dotY;
  }

  if (dotDirection > 0 && dotDirection < 4)
  {
    ++dotX;
  }
  else if (dotDirection != 0 && dotDirection != 4)
  {
    --dotX;
  }
  dot.setX(max(0, min(7, dotX)));
  dot.setY(min(7, max(1, dotY))); 
}



