#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include <stdio.h>

#define WW 800
#define WH 800

#define RADIUS 5

#define MAX_SPEED 20
#define Dx 5

#define MAX_FPS 180

const size_t NUMBER_OF_BALLS = 1000;

struct Ball {
  bool collided;
  Vector2 pos;
  Vector2 vel;
};


void create_balls(struct Ball balls[NUMBER_OF_BALLS], size_t t, float min_x, float max_x) {
  for (size_t i=0; i<t; i++) {
    balls[i].collided = false;
    balls[i].pos.x = GetRandomValue(min_x, max_x);
    balls[i].pos.y = GetRandomValue(1, WH);

    balls[i].vel.x = GetRandomValue(-MAX_SPEED, MAX_SPEED);
    balls[i].vel.y = GetRandomValue(-MAX_SPEED, MAX_SPEED);
  }
}


void collide(size_t i, struct Ball balls[NUMBER_OF_BALLS]) {
  bool hasnt_collided = true;
  for (size_t j=i+1; j<NUMBER_OF_BALLS; j++) {
    if (Vector2Distance(balls[i].pos, balls[j].pos) <= 2*RADIUS) {
      hasnt_collided = false;

      if (!balls[i].collided && !balls[j].collided) {
        balls[i].collided = true;
        balls[j].collided = true;

        Vector2 temp = balls[i].vel;

        balls[i].vel = balls[j].vel;
        balls[j].vel = temp;
      }
    }
  }
  if (hasnt_collided) {
    balls[i].collided = false;
  }

  if (balls[i].pos.x > WW - RADIUS) {
    balls[i].pos.x = WW - RADIUS;
    balls[i].vel.x = -balls[i].vel.x;
  }
  if (balls[i].pos.x < RADIUS) {
    balls[i].pos.x = RADIUS;
    balls[i].vel.x = -balls[i].vel.x;
  }

  if (balls[i].pos.y > WH - RADIUS) {
    balls[i].vel.y = -balls[i].vel.y;
    balls[i].pos.y = WH - RADIUS;
  }
  if (balls[i].pos.y < RADIUS) {
    balls[i].vel.y = -balls[i].vel.y;
    balls[i].pos.y = RADIUS;
  }
}


int main() {
  InitWindow(WW, WH, "Simulation");
  SetTargetFPS(MAX_FPS);
  struct Ball balls[NUMBER_OF_BALLS];

  create_balls(balls, 2*NUMBER_OF_BALLS/3, 1, 100);
  create_balls(balls, NUMBER_OF_BALLS/3, 1, WW);

  while (!WindowShouldClose()) {
    BeginDrawing();
      ClearBackground(BLACK);
      for (size_t i=0; i<NUMBER_OF_BALLS; i++) {
        balls[i].pos.x += Dx*balls[i].vel.x/MAX_FPS;
        balls[i].pos.y += Dx*balls[i].vel.y/MAX_FPS;
        
        collide(i, balls);

        DrawCircle(balls[i].pos.x, balls[i].pos.y, RADIUS, RED);
      }
      DrawFPS(0, 0);
      // TODO: Implement csv recording
    EndDrawing();
  }

  return 0;
}
