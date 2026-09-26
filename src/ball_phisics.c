// My header
#include "values.c"

//Raylib
#include <raylib.h>
#include <raymath.h>


double formula(double d, double mass) {
    return delta*12*REPULTION*(2*twelve(2*RADIUS/d) - sixth(2*RADIUS/d))/(d*mass);
}


void intermolecular_forces(struct Ball* ball) {
  for (size_t j=0; j<NUMBER_OF_BALLS; j++) {
    double d = Vector2Distance(ball->pos, balls[j].pos);
    if (d != 0) {
      ball->vel = Vector2Add(ball->vel, Vector2Scale(Vector2Normalize(Vector2Subtract(ball->pos, balls[j].pos)), formula(d, ball->mass)));
    }
  }
}


void border_collision(struct Ball* ball) {
  if (ball->pos.x > WW - RADIUS) {
    ball->pos.x = WW - RADIUS;
    ball->vel.x = -ball->vel.x;
  }
  if (ball->pos.x < RADIUS) {
    ball->pos.x = RADIUS;
    ball->vel.x = -ball->vel.x;
  }

  if (ball->pos.y > WH - RADIUS) {
    ball->vel.y = -ball->vel.y;
    ball->pos.y = WH - RADIUS;
  }
  if (ball->pos.y < RADIUS) {
    ball->vel.y = -ball->vel.y;
    ball->pos.y = RADIUS;
  }
}


void* ball_phisics(void* param) {
  size_t index = ((struct ThreadParam*) param)->i;
  struct Ball* list = ((struct ThreadParam*) param)->ball_list;

  size_t balls_over_threads = NUMBER_OF_BALLS/nthreads;

  if (index+1 == nthreads) {
    balls_over_threads += NUMBER_OF_BALLS%nthreads;
  }

  for (size_t i=0; i<balls_over_threads; i++) {
    list[i].vel.y += delta*G/2;
    intermolecular_forces(&(list[i]));
    border_collision(&(list[i]));

    list[i].pos.x += delta*list[i].vel.x;
    list[i].pos.y += delta*list[i].vel.y;
  }

  return NULL;
}
