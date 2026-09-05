#include "local/values.h"
#include "local/config.c"
#include "local/ball_phisics.c"

// Raylib
#include <raylib.h>
#include <raymath.h>

#include <pthread.h>  // Creates and joins threads

#include <stdlib.h>   // Functions malloc(3), free(3) and exit(3)

#include <unistd.h>   // Get number of threads in OS


void create_balls(struct Ball new_balls[NUMBER_OF_BALLS]) {
  for (size_t i=0; i<NUMBER_OF_BALLS; i++) {
    new_balls[i].pos.x = balls[i].pos.x = GetRandomValue(0, WW);
    new_balls[i].pos.y = balls[i].pos.y = GetRandomValue(0, WH);

    new_balls[i].vel.x = balls[i].vel.x = GetRandomValue(-MAX_START_SPEED, MAX_START_SPEED);
    new_balls[i].vel.y = balls[i].vel.y = GetRandomValue(-MAX_START_SPEED, MAX_START_SPEED);

    bool on_invalid_pos = true;
    for (size_t j=0; j<i && on_invalid_pos; j++) {
      if (Vector2Distance(balls[j].pos, balls[i].pos) <= 2*RADIUS) {
        i--;
        on_invalid_pos = false;
      }
    }
  }
}


void create_and_finish_threads(struct ThreadParam paramethers[nthreads], struct Ball new_balls[NUMBER_OF_BALLS], pthread_t threads[nthreads]) {
  for (size_t i=0; i<nthreads; i++) {
    paramethers[i].i = i;
    int balls_over_threads = NUMBER_OF_BALLS/nthreads;

    paramethers[i].ball_list = &(new_balls[balls_over_threads*i]);
    if (pthread_create(&(threads[i]), NULL, ball_phisics, &(paramethers[i])) != 0) {
      perror("Didn`t create thread");
      exit(2);
    }
  }
  for (size_t i=0; i<nthreads; i++) {
    pthread_join(threads[i], NULL);
  }
}


void draw_balls(struct Ball new_balls[NUMBER_OF_BALLS]) {
  BeginDrawing();
    ClearBackground(BLACK);
    for (size_t i=0; i<NUMBER_OF_BALLS; i++) {
      balls[i].vel.x = new_balls[i].vel.x;
      balls[i].vel.y = new_balls[i].vel.y;
      balls[i].pos.x = new_balls[i].pos.x;
      balls[i].pos.y = new_balls[i].pos.y;
      DrawCircle(balls[i].pos.x, balls[i].pos.y, RADIUS, RED);
    }
    DrawFPS(0, 0);
  EndDrawing();
}


int main() {
  read_config();
  InitWindow(WW, WH, "Simulation");
  SetTargetFPS(MAX_FPS);

  balls = calloc(NUMBER_OF_BALLS, sizeof(struct Ball));
  if (balls == NULL) {
    perror("Malloc failed");
    return 3;
  }
  struct Ball new_balls[NUMBER_OF_BALLS];

  nthreads = sysconf(_SC_NPROCESSORS_ONLN);
  struct ThreadParam paramethers[nthreads];

  pthread_t threads[nthreads];

	create_balls(new_balls);

  while (!WindowShouldClose()) {
    delta = GetFrameTime();

    create_and_finish_threads(paramethers, new_balls, threads);

    draw_balls(new_balls);
  }
  free(balls);

  return 0;
}
