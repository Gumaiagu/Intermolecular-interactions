#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int WW = 800;
int WH = 500;

int RADIUS = 5;

float MAX_START_SPEED = 50;

float G = 9.8;

float REPULTION = 1000;

size_t MAX_FPS = 180;

size_t NUMBER_OF_BALLS = 100;

struct Ball {
  Vector2 pos;
  Vector2 vel;
};


#define sixth(x) x*x*x*x*x*x
#define twelve(x) sixth(x)*sixth(x)


void read_config() {
  FILE* config = fopen("config", "r");
  if (config == NULL) {
    perror("Could not read file");
    exit(1);
  }
  size_t len;
  char* line = NULL;
  int read;
  read = getline(&line, &len, config);
  line[read--] = '\0';
  while (read > 0) {
    line[2] = '\0';
    if (strcmp(line, "WW") == 0) {
      WW = atoi(line+4);   
    }
    else if (strcmp(line, "WH") == 0) {
      WH = atoi(line+4);
    }
    else if (strcmp(line, "GR") == 0) {
      G = atof(line+4);
    }
    else if (strcmp(line, "NB") == 0) {
      NUMBER_OF_BALLS = atoi(line+4);
    }
    else if (strcmp(line, "RE") == 0) {
      REPULTION = atoi(line+4);
    }
    else if (strcmp(line, "MS") == 0) {
      MAX_START_SPEED = atoi(line+4);
    }
    read = getline(&line, &len, config);
  }
  fclose(config);
  free(line);
}


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
  read_config();
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
        
        // TODO: Implement threding
        collide(i, balls, delta);

        DrawCircle(balls[i].pos.x, balls[i].pos.y, RADIUS, RED);
      }
      DrawFPS(0, 0);
      // TODO: Implement csv recording
    EndDrawing();
  }

  return 0;
}
