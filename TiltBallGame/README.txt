// This game features an I2C connection with an external accelerometer that determines the tilt
// of the board which in turn determines the acceleration of the ball that the player must move
// around on the board. 

// The game is displayed on an LCD screen that is interfaced with via a screen
// driver. The player must move the ball about the board, dodging red balls and collecting green 
// balls. 

// When a ball is hit, the system uses a PWM to create frequencies on a piezo electric buzzer to 
// play winning and losing noises for the two type. The system keeps track of the score and has
// a reset button to revert the score to zero.

// In the repo you'll find a demonstration video (DemonstrationVideo.mov) of the game in action
