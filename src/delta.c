// Befunge IP delta
// Author: xxc3nsoredxx

#include "delta.h"

void up (delta_t *delta) {
    delta->delta_x = 0;
    delta->delta_y = -1;
}

void down (delta_t *delta) {
    delta->delta_x = 0;
    delta->delta_y = 1;
}

void left (delta_t *delta) {
    delta->delta_x = -1;
    delta->delta_y = 0;
}

void right (delta_t *delta) {
    delta->delta_x = 1;
    delta->delta_y = 0;
}
