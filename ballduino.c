/* ARDUINO BRICK BREAKER
ELECTRONICS II SPRING 2013


  Copyright 2013 Meredith Myers

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

#include <glcd.h>
#include "fonts/SystemFont5x7.h"   
#include "values.h"      // <-- See this file for variable definitions    

/* Setup the LCD and display Welcome message */
void setup(){
  GLCD.Init();
  Serial.begin(9600);
  GLCD.SelectFont(System5x7);
  randomSeed(analogRead(potPin));
  welcome();
}

/* Game play loop */
void  loop(){
  
  if (continueGame){                                                      // If the game is still in play
    drawFrame();                                                          // Draw the frame
    drawScore();                                                          // Draw the score
    movePaddle();                                                         // Update the location of the paddle
    boolean paddleCollision = checkPaddleCollision();                     // Determine if the ball has hit the paddle or block
    boolean blockCollision = checkBlockCollision();
    if(score == numBlocks)                                                // If the score is equivalent to the number of blocks, game is over
      winner();                                                           // Display message to user
    else{                                                                 // The game is still in play
      if(paddleCollision || blockCollision)                               // Redraw screen to draw over any collisions
        drawFrame();
      delay(50);                                                          // Slight delay
      continueGame = updatePos();                                         // Update the position of the ball
    }
  }
  else                                                                    // The game is over, the ball fell off the screen. Display message to user.
    gameOver();
}

/* Draw the frame with ball and paddle */
void drawFrame(){

  GLCD.FillCircle(oldBallX, oldBallY, rad, WHITE);                        // Draw over the old ball
  GLCD.FillCircle(ballX, ballY, rad, BLACK);                              // Draw the new ball
  GLCD.FillRect(paddleXOld, 58, 15, 5, WHITE);                            // Draw over old paddle
  GLCD.FillRect(paddleX, 58, 15, 5, BLACK);                               // Draw new paddle

  for(int i=0;i<numBlocks;i++){                                           // Draw the blocks (24x3)
    if(bricks[i][0] == 1)                                                 // If still in play
      GLCD.DrawBitmap(brick,bricks[i][1],bricks[i][2],BLACK);             // Draw the block
  }
}

/* Move the ball */
boolean updatePos(){
  if (ballX > 132 - rad*2 || ballX < 0 + rad*2)                           // If the ball hits the right or left of the screen
    xincrement = -xincrement;

  if(ballY > 64)                                                          // Ball has hit the bottom. GAME OVER
    return false;

  else if (ballY < 0 + rad*2)                                             // If the ball is at top of the screen
    yincrement = -yincrement;

  oldBallX = ballX;                                                       // Save the ball's current position
  oldBallY = ballY;

  ballX += xincrement;                                                    // Update the ball's location
  ballY += yincrement;
 
  return true;
}

/* Checks if the ball has hit the paddle */
boolean checkPaddleCollision(){
  int ballTop = ballY-rad;                                               // Define some values for easy reference
  int ballBottom = ballY+rad;
  int ballLeft = ballX-rad;
  int ballRight = ballX+rad;
  int paddleX1 = paddleX+15-1;
  int middleOffset = 5;
  if(ballBottom >= paddleY){                                            // If the ball is hitting the TOP of the paddle
    if(ballX >= paddleX && ballX <= paddleX1){                          // If the ball has hit in between the left and right edge of paddle
        float paddleMidPt = (paddleX+15-1)/2;                           // Determine paddle midpt to determine whether to flip x direction
        yincrement = -yincrement;                                       // Flip y increment
        if(ballX <= paddleMidPt-middleOffset)                           // If the ball's xloc is less than the paddle's mid point, flip the x direction
          xincrement = -xincrement; 
        return true;
    }
    if(ballRight >= paddleX && ballLeft < paddleX){                     // If the ball is hitting the LEFT of the paddle
      if(xincrement > 0)                                                // If it's coming from the left, flip x
        xincrement = -xincrement;
      yincrement = -yincrement;
      return true;
    }
    else if(ballLeft <= paddleX1 && ballRight > paddleX1){              // If the ball is hitting the RIGHT of the paddle
      if(xincrement < 0)                                                // If it's coming from the right, flip x
        xincrement = -xincrement;
      yincrement = -yincrement;
      return true;
    }
  }
  return false;
   
}
/* Checks if the ball has collided with a block */
boolean checkBlockCollision(){
    int ballTop = ballY-rad;                                            // Values for easy reference
    int ballBottom = ballY+rad;
    int ballLeft = ballX-rad;
    int ballRight = ballX+rad;
    
    for(int i=0;i<numBlocks;i++){                                       // Loop through the blocks
        if(bricks[i][0] == 1){                                          // If the block hasn't been eliminated
         int blockX = bricks[i][1];                                     // Grab x and y location
         int blockY = bricks[i][2];
         if(ballBottom >= blockY && ballTop <= blockY+blockHeight){     // If hitting BLOCK
           if(ballRight >= blockX && ballLeft <= blockX+blockWidth){       
             removeBlock(i);                                            // Mark the block as out of play
             return true;
           }
         }
      }
    }
  return false;                                                         // No collision detected
}
/* Removes a block from game play */
void removeBlock(int index){
      GLCD.DrawBitmap(brick,bricks[index][1],bricks[index][2],WHITE);   // Draw over the removed block
      bricks[index][0] = 0;                                             // Mark it as out of play
      score++;                                                          // Increment score
      yincrement = -yincrement;                                         // Flip the y increment
      adjustSpeed();                                                    // Increment the speed of the ball
}
/* Move the paddle */
void movePaddle(){
  paddleXOld = paddleX;                                                 // Save the paddle's old location
  paddleX = map(analogRead(potPin), 0, 1023, 0, 56*2);                  // Get the current paddle location
}
/* Increments the speed of the ball */
void adjustSpeed(){
    if(yincrement < 0)
      yincrement-=0.09;
    else
      yincrement+=0.09;
      
    if(xincrement < 0)
      xincrement-=0.09;
    else
      xincrement+=0.09; 
      
      Serial.print(xincrement);
      Serial.print(" ");
      Serial.println(yincrement);
}
/* Draw the score */
void drawScore(){
  GLCD.CursorTo(0,0);
  GLCD.print('S');
  GLCD.print(score);
}
/* Draws a welcome message before game play begins */
void welcome(){
  gText t3 = gText(10, GLCD.CenterY, 20, 1, SystemFont5x7);
  t3.Printf("Get ready to play...");
  delay(2000);
  GLCD.ClearScreen();
  t3 = gText(10, GLCD.CenterY, 20, 1, SystemFont5x7);
  t3.Printf("BALLDUINO!");
  delay(500);
  GLCD.ClearScreen(); 
}
/* Start a new game */
void gameOver(){
  GLCD.ClearScreen();
  gText t3 = gText(10, GLCD.CenterY, 8, 1, SystemFont5x7);
  t3.Printf("FAILURE!");
  delay(3000);
  GLCD.ClearScreen();
  reset();
}
/* Tell the user they won */
void winner(){
  GLCD.ClearScreen();
  gText t3 = gText(10, GLCD.CenterY, 8, 1, SystemFont5x7);
  t3.Printf("YOU WIN!");
  delay(3000);
  GLCD.ClearScreen();
  reset();
}
/* Resets all variables, blocks, ball and paddle to restart game play */
void reset(){
  ballX = 55;
  ballY = 42;
  oldBallX = 55;
  oldBallY = 42;
  xincrement = 1.7;
  yincrement = 1.7;
  paddleX = 64;
  score = 0;
  paddleXOld = 64;
  continueGame = true;
  resetBlocks();
  welcome();

}
/* Resets all the blocks */
void resetBlocks(){
  char brickReset[24][3] = {
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
  // Loop through and deep copy into the block array. 
  for(byte entity = 0; entity < numBlocks; entity++){
    for(byte data = 0; data <= 3; data++){
      bricks[entity][data] = brickReset[entity][data];
    }
  }
}
