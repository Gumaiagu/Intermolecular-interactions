#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include <stdio.h>

#define WW 800
#define WH 800

#define RADIUS 5

#define MAX_SPEED 20
#define Dx 5

#define REPULTION 10

#define MAX_FPS 180

const size_t NUMBER_OF_BALLS = 100;

struct Ball {
  Vector2 pos;
  Vector2 vel;
};


void create_balls(struct Ball balls[NUMBER_OF_BALLS], size_t t, float min_x, float max_x) {
  for (size_t i=0; i<t; i++) {
    balls[i].pos.x = GetRandomValue(min_x, max_x);
    balls[i].pos.y = GetRandomValue(1, WH);

    balls[i].vel.x = GetRandomValue(-MAX_SPEED, MAX_SPEED);
    balls[i].vel.y = GetRandomValue(-MAX_SPEED, MAX_SPEED);
  }
}


void collide(size_t i, struct Ball balls[NUMBER_OF_BALLS]) {
  for (size_t j=i+1; j<NUMBER_OF_BALLS; j++) {
    balls[i].vel = Vector2Subtract(balls[i].vel, Vector2Scale(Vector2Normalize(Vector2Subtract(balls[j].pos, balls[i].pos)), REPULTION/Vector2DistanceSqr(balls[j].pos, balls[i].pos)));

    balls[j].vel = Vector2Subtract(balls[j].vel, Vector2Scale(Vector2Normalize(Vector2Subtract(balls[i].pos, balls[j].pos)), REPULTION/Vector2DistanceSqr(balls[i].pos, balls[j].pos)));
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

  create_balls(balls, NUMBER_OF_BALLS, 1, 100);

//create_balls(balls, 2*NUMBER_OF_BALLS/3, 1, 100);
//create_balls(balls, NUMBER_OF_BALLS/3, 1, WW);

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
