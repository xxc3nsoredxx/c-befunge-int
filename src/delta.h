#ifndef DELTA_H_20160923_142135
#define DELTA_H_20160923_142135

typedef struct delta_s {
    int delta_x;
    int delta_y;
} delta_t;

void up (delta_t*);
void down (delta_t*);
void left (delta_t*);
void right (delta_t*);

#endif
