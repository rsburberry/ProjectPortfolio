#include "sounds.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

// ########################################################################## //
// ###   Notes                                                            ### //
// ########################################################################## //
#define NOTE_H   0
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978 

// ########################################################################## //
// ###   Songs                                                            ### //
// ########################################################################## //
const int win_song[] = { 6,
                        NOTE_E5, 10, NOTE_G5, 10, NOTE_E6, 10, NOTE_C6, 10,
                        NOTE_D6, 10, NOTE_G6, 10 };
const int lose_song[] = { 15,
                          NOTE_C4, 8, NOTE_H, 8, NOTE_H, 8, NOTE_G3, 8, NOTE_H,
                          4, NOTE_E3, 4, NOTE_A3, 6, NOTE_B3, 6, NOTE_A3, 6,
                          NOTE_GS3, 6, NOTE_AS3, 6, NOTE_GS3, 6, NOTE_G3, 8,
                          NOTE_F3, 8, NOTE_G3, 4 };

// bit masks for the lights and buttons
#define PIN_MASK_PIEZO (1<<1) 

void sounds_init(void) {
  SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF; // enable Port F GPIO.
  GPIO_PORTF_DIR_R |= PIN_MASK_PIEZO;  // Set the speaker pin to an output.
  GPIO_PORTF_DEN_R |= PIN_MASK_PIEZO;  // enable the speaker digital pin.
  GPIO_PORTF_PCTL_R |= (0x5 << 4);
  GPIO_PORTF_AFSEL_R |= PIN_MASK_PIEZO; // Use alternate funct. on the speaker.

   // Set the PWM clock divider.
  SYSCTL_RCC_R |= (1 << 20);
  SYSCTL_RCC_R |= (0x3 << 17);
  SYSCTL_RCGCPWM_R |= 0x2;

  // Set the basic PWM settings.
  PWM1_INVERT_R &= ~(1 << 5);
  PWM1_2_CTL_R |= 0x3;
  PWM1_2_LOAD_R = 100;
  PWM1_2_GENA_R = 0xE;
  PWM1_2_GENB_R = 0xE;
  PWM1_2_DBCTL_R = 0x1;
  PWM1_ENABLE_R &= ~(1 << 5);
}

void playWin(void) {
  for (int i = 1; i < (win_song[0] * 2); i += 2) {
    PWM1_ENABLE_R &= ~(1 << 5);
    if (win_song[i] == 0) {
      PWM1_2_LOAD_R = 1000000;
    } else {
      PWM1_2_LOAD_R = ((1000000 / 4) / win_song[i]);
    }
    PWM1_ENABLE_R |= (1 << 5);
    for (int j = 0; j < (30000 * win_song[i + 1]); j++) {}
  }
  PWM1_ENABLE_R &= ~(1 << 5);
}

void playLose(void) {
  for (int i = 1; i < (lose_song[0] * 2 + 1); i += 2) {
    PWM1_ENABLE_R &= ~(1 << 5);
    if (lose_song[i] == 0) {
      PWM1_2_LOAD_R = 1000000;
    } else {
      PWM1_2_LOAD_R = ((1000000 / 4) / lose_song[i]);
    }
  PWM1_ENABLE_R |= (1 << 5);
  for (int j = 0; j < (30000 * lose_song[i + 1]); j++) {}
}
  PWM1_ENABLE_R &= ~(1 << 5);
}