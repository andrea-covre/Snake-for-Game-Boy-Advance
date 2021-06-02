/**
    @file game.c
    @author Andrea Covre
    @version 1.3 (March 22nd, 2020)
*/

#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"

#include "images/welcomeScreen.h"
#include "images/gameOver.h"
#include "images/garbage.h"
#include "images/target.h"

struct snake Snake;

typedef enum {
  START,
  WAIT_TO_START,
  PLAY,
  WIN,
  LOSE,
} GBAState;

char startPrompt[] = "Press Start";
char winnigText[] = "Congrats, you won!";
int updateExitCode = 0;

int main(void) {

  REG_DISPCNT= MODE3 | BG2_ENABLE;

  REG_DISPCNT;

  // Saving current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Loading initial game state
  GBAState state = START;

  int loopCounter = 0;
  char score[] = "          ";

  while (1) {
    waitForVBlank();
    currentButtons = BUTTONS;  // Loading the current state of the buttons
    if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
        state = START;
    }

    switch (state) {
      case START:
        drawFullScreenImageDMA(startScreen);
        drawCenteredString(100, 95, 50, 50, startPrompt, RED);
        state = WAIT_TO_START;
        break;

      case WAIT_TO_START:
        //clearing body memory
        for (int i = 0; i < 60; i++) {
            for (int j = 0; j < 40; j++) {
                Snake.body[i][j] = 0;
            }
        }
        Snake.length = 1;
        Snake.direction = 0;
        Snake.speed = 20;
        Snake.x = 30;
        Snake.y = 20;
        Snake.body[30][20] = 1;
        Snake.xTail = 30;
        Snake.yTail = 20;
        Snake.tailDirection[30][20] = 0;
        Snake.targetHit = 0;
        newTarget();

        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
            state = PLAY;
            //Prepping Screen
            fillScreenDMA(RED);
            drawRectDMA(4, 4, 232, 152, BLACK);

        }

        break;

      case PLAY:
        drawTarget();
        updateDirection(currentButtons, previousButtons);
        if (loopCounter >= Snake.speed) {
            int updateExitCode = updatePosition();
            if(updateExitCode == 1) {
                state = LOSE;
            } else if (updateExitCode == 2) {
                state = WIN;
            } else {
                drawSnake();
                drawTarget();
            }
        }
        break;

      case WIN:
        fillScreenDMA(BLACK);
        drawCenteredString(0, 0, 240, 160, winnigText, GREEN);
        break;

      case LOSE:
        drawFullScreenImageDMA(gameOver);
        drawCenteredString(100, 95, 50, 50, startPrompt, RED);
        sprintf(score, "Score: %d", (Snake.length - 1)*10);
        drawCenteredString(20, 95, 50, 50, score, RED);
        state = WAIT_TO_START;
        break;

    }

    if (loopCounter >= Snake.speed) {
        loopCounter = 0;
    } else {
        loopCounter++;
    }
    previousButtons = currentButtons;  // storing the current state of the buttons
  }
  return 0;
}

void updateDirection(u32 currentButtons, u32 previousButtons) {
    if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons)) {
        if (Snake.direction == 2) {
            if (Snake.length < 2) {
                Snake.direction = 0;
            }
        } else {
            Snake.direction = 0;
        }

    } else if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons)) {
        if (Snake.direction == 3) {
            if (Snake.length < 2) {
                Snake.direction = 1;
            }
        } else {
            Snake.direction = 1;
        }

    } else if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons)) {
        if (Snake.direction == 0) {
            if (Snake.length < 2) {
                Snake.direction = 2;
            }
        } else {
            Snake.direction = 2;
        }

    } else if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons)) {
        if (Snake.direction == 1) {
            if (Snake.length < 2) {
                Snake.direction = 3;
            }
        } else {
            Snake.direction = 3;
        }
    }
}

int updatePosition(void) {
    switch (Snake.direction) {
      case 0:
        Snake.tailDirection[Snake.x][Snake.y] = 0;
        Snake.y--;
        break;

      case 1:
        Snake.tailDirection[Snake.x][Snake.y] = 1;
        Snake.x++;
        break;

      case 2:
        Snake.tailDirection[Snake.x][Snake.y] = 2;
        Snake.y++;
        break;

      case 3:
        Snake.tailDirection[Snake.x][Snake.y] = 3;
        Snake.x--;
        break;
  }
  if (Snake.x == Snake.xTarget && Snake.y == Snake.yTarget) {
      newTarget();
      Snake.targetHit = 1;
      Snake.length++;
      if (Snake.length == 2400) {
          return 2;
      }
      if (Snake.length % 5 == 0 && Snake.speed > 15) {
          Snake.speed--;
      }
  }
  if (Snake.body[Snake.x][Snake.y] == 1) {
      return 1;
  } else {
      Snake.body[Snake.x][Snake.y] = 1;
  }
  if (Snake.x < 1 || Snake.x >= 59 || Snake.y < 1 || Snake.y >= 39) {
      return 1;
  } else {
      return 0;
  }
}

void drawSnake(void) {
    drawRectDMA(Snake.y * 4, Snake.x * 4, 4, 4, GREEN);
    drawRectDMA(Snake.y * 4 + 1, Snake.x * 4 + 1, 2, 2, YELLOW);
    //setPixel(Snake.y, Snake.x, WHITE);
    if(Snake.targetHit == 0) {
        drawRectDMA(Snake.yTail * 4, Snake.xTail * 4, 4, 4, GRAY);
        Snake.body[Snake.xTail][Snake.yTail] = 0;
        switch (Snake.tailDirection[Snake.xTail][Snake.yTail]) {
            case 0:
            Snake.yTail--;
            break;

            case 1:
            Snake.xTail++;
            break;

            case 2:
            Snake.yTail++;
            break;

            case 3:
            Snake.xTail--;
            break;
        }
    } else {
        Snake.targetHit = 0;
        switch (Snake.tailDirection[Snake.x][Snake.y]) {
            case 0:
            Snake.yTail = Snake.yTail;
            Snake.xTail = Snake.xTail;
            break;

            case 1:
            Snake.yTail = Snake.yTail;
            Snake.xTail = Snake.xTail;
            break;

            case 2:
            Snake.yTail = Snake.yTail;
            Snake.xTail = Snake.xTail;
            break;

            case 3:
            Snake.yTail = Snake.yTail;
            Snake.xTail = Snake.xTail;
            break;
        }

    }
}

void drawTarget(void) {
    drawImageDMA(Snake.yTarget * 4, Snake.xTarget * 4, 4, 4, target);
}

void newTarget(void) {
    do {
        Snake.xTarget = randint(1, 59);
        Snake.yTarget = randint(1, 39);
    } while (Snake.body[Snake.xTarget][Snake.yTarget] == 1);
}

