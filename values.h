
//-------------------------------------------- HEADER FILE -----------------------------------------------
// BALLDUINO ARDUINO BRICK BREAKER
// Meredith Myers Spring 2013

#include <inttypes.h>
#include <avr/pgmspace.h>

/* Variable definitions */
#define potPin        5       // analog input pin connected to pan pot 
float ballX = 55;
float ballY = 42;
float oldBallX = 55;
float oldBallY = 42;
float xincrement = 1.3;
float yincrement = 1.3;
int rad = 3;
int score = 0;
int blockWidth = 10;
int blockHeight = 5;
int paddleX = 64;
int paddleY = 58;
int paddleXOld = 64;
int numBlocks = 24;
boolean continueGame = true;

char bricks[24][3] = {
  // in play, xloc, yloc 
   {1,   4,    10}  ,
   {1,   19,    10}  ,
   {1,   34,    10}  ,
   {1,   49,    10}  ,
   {1,   64,    10}  ,
   {1,   79,    10}  ,
   {1,   94,    10}  ,
   {1,   109,    10}  ,
   {1,   4,    20}  ,
   {1,   19,    20}  ,
   {1,   34,    20}  ,
   {1,   49,    20}  ,
   {1,   64,    20}  ,
   {1,   79,    20}  ,
   {1,   94,    20}  ,
   {1,   109,    20}  ,
   {1,   4,    30}  ,
   {1,   19,    30}  ,
   {1,   34,    30}  ,
   {1,   49,    30}  , 
   {1,   64,    30}  ,
   {1,   79,    30}  ,
   {1,   94,    30}  ,
   {1,   109,    30}  
};
/* Block shape */
static uint8_t brick[] PROGMEM = {
  10,
  8,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};