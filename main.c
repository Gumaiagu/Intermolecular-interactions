#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include <stdio.h>

#define WW 800
#define WH 800

#define RADIUS 5

#define MAX_START_SPEED 50

#define G 9.8

#define REPULTION 1000

#define MAX_FPS 180

const size_t NUMBER_OF_BALLS = 100;

struct Ball {
  Vector2 pos;
  Vector2 vel;
};


#define sixth(x) x*x*x*x*x*x
#define twelve(x) sixth(x)*sixth(x)


void create_balls(struct Ball balls[NUMBER_OF_BALLS]) {
  for (size_t i=0; i<NUMBER_OF_BALLS; i++) {
    balls[i].pos.x = GetRandomValue(1, WW);
    balls[i].pos.y = GetRandomValue(0, WH);

    balls[i].vel.x = GetRandomValue(-MAX_START_SPEED, MAX_START_SPEED);
    balls[i].vel.y = GetRandomValue(-MAX_START_SPEED, MAX_START_SPEED);
    bool on_invalid_pos = true;
    for (size_t j=0; j<i && on_invalid_pos; j++) {
      if (Vector2Distance(balls[j].pos, balls[i].pos) <= 2*RADIUS) {
        i--;
        on_invalid_pos = false;
      }
    }
  }
}


void collide(size_t i, struct Ball balls[NUMBER_OF_BALLS], float delta) {
  for (size_t j=i+1; j<NUMBER_OF_BALLS; j++) {
    float d = Vector2Distance(balls[i].pos, balls[j].pos);
    balls[i].vel = Vector2Add(balls[i].vel, Vector2Scale(Vector2Normalize(Vector2Subtract(balls[i].pos, balls[j].pos)), delta*2*REPULTION*(twelve(2*RADIUS/d) - sixth(2*RADIUS/d))));

    balls[j].vel = Vector2Add(balls[j].vel, Vector2Scale(Vector2Normalize(Vector2Subtract(balls[j].pos, balls[i].pos)), delta*2*REPULTION*(twelve(2*RADIUS/d) - sixth(2*RADIUS/d))));
  }
  balls[i].vel.y += delta*G/2;

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

	create_balls(balls);

  while (!WindowShouldClose()) {
    BeginDrawing();
      ClearBackground(BLACK);
      float delta = GetFrameTime();
      for (size_t i=0; i<NUMBER_OF_BALLS; i++) {
        balls[i].pos.x += delta*balls[i].vel.x;
        balls[i].pos.y += delta*balls[i].vel.y;
        
        collide(i, balls, delta);

        DrawCircle(balls[i].pos.x, balls[i].pos.y, RADIUS, RED);
      }
      DrawFPS(0, 0);
      // TODO: Implement csv recording
    EndDrawing();
  }

  return 0;
}
