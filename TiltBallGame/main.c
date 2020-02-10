#include <stdlib.h>
#include "i2c.h"
#include "GPIO.h"
#include "SSD2119.h"
#include "sounds.h"

// ########################################################################## //
// ###   Function Prototypes                                              ### //
// ########################################################################## //
void updatePortals(void);

// ########################################################################## //
// ###   Graphics and Game Physics Defines                                ### //
// ########################################################################## //
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#define BALL_RADIUS 15
#define PORTAL_RADIUS 20
#define X_COMPENSATION 332
#define Y_COMPENSATION 14
#define MOVEMENT_DIVIDER 10000

// ########################################################################## //
// ###   Game variables                                                   ### //
// ########################################################################## //
uint8_t GyroX_H, GyroX_L, GyroY_H, GyroY_L;
int16_t GyroX_vel = 0, GyroY_vel = 0;
int32_t GyroX_pos = 0, GyroY_pos = 0;
float ballX, ballY, ballXold, ballYold, dX, dY;
uint32_t t = 0; // Current system time.
int32_t portalX = 100, portalY = 100;
int32_t portalX_hell = 265, portalY_hell = 123; 
int32_t score = 0;

int main()
{
  PLL_Init(40); // We need the faster clock to make the rendering smooth.
  sounds_init();
  LCD_Init();
  i2c_init();
  gpio_init_pin(0XE3, INPUT); // Reset button.

  // Reset the Gyroscope and bring it out of hybernation.
  gyro_writeRegister(0x6B, 0x00);

  // Start the ball at the center of the screen.
  ballX = ballXold = SCREEN_WIDTH/2;
  ballY = ballYold = SCREEN_HEIGHT/2;

  // Draw the new ball.
  LCD_DrawCircle(ballX, ballY, BALL_RADIUS, convertColor(0x00, 0x00, 0xFF));
  
  // Draw the first portals
  LCD_DrawFilledCircle(portalX, portalY, PORTAL_RADIUS, convertColor(0x00, 0xFF, 0x00));
  LCD_DrawFilledCircle(portalX_hell, portalY_hell, PORTAL_RADIUS, convertColor(0xFF, 0x00, 0x00));

  while(1) {
    // Check for reset.
    if(!gpio_read(0XE3)){
      // Start the ball at the center of the screen.
      ballX = ballXold = SCREEN_WIDTH/2;
      ballY = ballYold = SCREEN_HEIGHT/2;
      portalX = 100;
      portalY = 100;
      portalX_hell = 265;
      portalY_hell = 123; 
      LCD_ColorFill(convertColor(0x00, 0x00, 0x00));
      gyro_writeRegister(0x6B, 0x00); // Reset the unit.
       // Draw the first portal
      LCD_DrawFilledCircle(portalX, portalY, PORTAL_RADIUS, convertColor(0x00, 0xFF, 0x00));
      LCD_DrawFilledCircle(portalX_hell, portalY_hell, PORTAL_RADIUS, convertColor(0xFF, 0x00, 0x00));
      score = 0;
    }
    
    // Draw the portals
    LCD_DrawFilledCircle(portalX, portalY, PORTAL_RADIUS, convertColor(0x00, 0xFF, 0x00));
    LCD_DrawFilledCircle(portalX_hell, portalY_hell, PORTAL_RADIUS, convertColor(0xFF, 0x00, 0x00));

    t++; // Increment system time.

    // Display the score
    char scoreString[20];
    sprintf(scoreString, "Score: %3d", score);
    LCD_SetCursor(10,10);
    LCD_PrintString(scoreString);

    // Read off the X gyroscope value.
    GyroX_H = gyro_readRegister(0x43);
    GyroX_L = gyro_readRegister(0x44);
    GyroX_vel = ((GyroX_H << 8) | GyroX_L) + X_COMPENSATION;
    GyroX_pos = GyroX_pos + GyroX_vel;

    // Read off the Y gyroscope value.
    GyroY_H = gyro_readRegister(0x45);
    GyroY_L = gyro_readRegister(0x46);
    GyroY_vel = ((GyroY_H << 8) | GyroY_L) + Y_COMPENSATION;
    GyroY_pos = GyroY_pos + GyroY_vel;

    // Calculate the new ball location.
    ballXold = ballX;
    ballYold = ballY;
    dX =  -((float) GyroY_pos)/(((float) MOVEMENT_DIVIDER)/(1.0+((float)score)/2.0));
    dY =  -((float) GyroX_pos)/(((float) MOVEMENT_DIVIDER)/(1.0+((float)score)/2.0));
    ballX = ballX + dX;
    ballY = ballY + dY;
    if(ballX < BALL_RADIUS) ballX = BALL_RADIUS/2 + dX;
    if(ballY < BALL_RADIUS) ballY = BALL_RADIUS/2 + dY;
    if(ballX > SCREEN_WIDTH - BALL_RADIUS) ballX = SCREEN_WIDTH - BALL_RADIUS/2 + dX;
    if(ballY > SCREEN_HEIGHT - BALL_RADIUS) ballY = SCREEN_HEIGHT - BALL_RADIUS/2 + dY;

    // Clear the old ball.
    LCD_DrawCircle(ballXold, ballYold, BALL_RADIUS, convertColor(0x00, 0x00, 0x00));
    // Draw the new ball.
    LCD_DrawCircle(ballX, ballY, BALL_RADIUS, convertColor(0x00, 0x00, 0xFF));

    // Check for collision with portal.
    if((abs(ballX - portalX) < PORTAL_RADIUS/3) && (abs(ballY - portalY) < PORTAL_RADIUS/3)) {
      LCD_DrawFilledCircle(portalX, portalY, PORTAL_RADIUS, convertColor(0x00, 0x00, 0x00));
      playWin();
      updatePortals();
      score++;
    }

    // Check for collision with portal to hell.
    if((abs(ballX - portalX_hell) < PORTAL_RADIUS/3) && (abs(ballY - portalY_hell) < PORTAL_RADIUS/3)) {
      LCD_DrawFilledCircle(portalX_hell, portalY_hell, PORTAL_RADIUS, convertColor(0x00, 0x00, 0x00));
      playLose();
      updatePortals();
      score = 0;
    }
  }

  return 0;
}

// Helper funciton to update the portals on the game map.
void updatePortals(){
  // Clear the portals
  LCD_DrawFilledCircle(portalX, portalY, PORTAL_RADIUS, convertColor(0x00, 0x00, 0x00));
  LCD_DrawFilledCircle(portalX_hell, portalY_hell, PORTAL_RADIUS, convertColor(0x00, 0x00, 0x00));
  
  // Calculate new random portal locations from the current time.
  portalX_hell = (t*53 % SCREEN_WIDTH) - PORTAL_RADIUS + PORTAL_RADIUS/2;
  if(portalX_hell < PORTAL_RADIUS) portalX_hell = PORTAL_RADIUS;
  portalY_hell = (t*59 % SCREEN_HEIGHT) - PORTAL_RADIUS + PORTAL_RADIUS/2;
  if(portalY_hell < PORTAL_RADIUS) portalY_hell = PORTAL_RADIUS;
  portalX = (t*61 % SCREEN_WIDTH) - PORTAL_RADIUS + PORTAL_RADIUS/2;
  if(portalX < PORTAL_RADIUS) portalX = PORTAL_RADIUS;
  portalY = (t*67 % SCREEN_HEIGHT) - PORTAL_RADIUS + PORTAL_RADIUS/2;
  if(portalY < PORTAL_RADIUS) portalY = PORTAL_RADIUS;
  
  // Make sure the portals dont overlap.
  // If they do overlap move the portal to a new location.
  if(abs(portalX_hell - portalX) < PORTAL_RADIUS*2){
    portalX_hell = (portalX_hell + 100)%SCREEN_WIDTH;
  }
  if(abs(portalY_hell - portalY) < PORTAL_RADIUS*2){
    portalY_hell = (portalY_hell + 100)%SCREEN_HEIGHT;
  } 
}