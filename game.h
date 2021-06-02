/**
    @file game.h
    @author Andrea Covre
    @version 1.3 (March 22nd, 2020)
*/

#ifndef GAME_H
#define GAME_H

#include "gba.h"

typedef struct snake {
    int direction; //0 = UP, 1 = RIGHT, 2 = DOWN, 3 = LEFT,
    int length;
    int x;
    int y;
    int body[60][40];
    int xTail;
    int yTail;
    int tailDirection[60][40];
    int speed;
    int xTarget;
    int yTarget;
    int targetHit;
} Sanke;

#endif
