#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

int WW = 800;
int WH = 500;

float delta;

int RADIUS = 5;

size_t nthreads;

float MAX_START_SPEED = 50;

float G = 9.8;

float REPULTION = 1000;

size_t MAX_FPS = 180;

size_t NUMBER_OF_BALLS = 100;
struct Ball* balls;

struct Ball {
  Vector2 pos;
  Vector2 vel;
};


struct ThreadParam {
  size_t i;
  struct Ball* ball_list;
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


void create_balls() {
  for (size_t i=0; i<NUMBER_OF_BALLS; i++) {
    balls[i].pos.x = GetRandomValue(0, WW);
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


void* collide(void* param) {
  size_t index = ((struct ThreadParam*) param)->i;
  struct Ball* list = ((struct ThreadParam*) param)->ball_list;

  size_t balls_over_threads = NUMBER_OF_BALLS/nthreads;

  if (index+1 == nthreads) {
    balls_over_threads = NUMBER_OF_BALLS % nthreads;
  }
  else{
    balls_over_threads = NUMBER_OF_BALLS/nthreads;
  }

  for (size_t i=0; i<balls_over_threads; i++) {
    for (size_t j=0; j<NUMBER_OF_BALLS; j++) {
      float d = Vector2Distance(list[i].pos, balls[j].pos);
      if (d != 0) {
        Vector2 t = Vector2Scale(Vector2Normalize(Vector2Subtract(list[i].pos, balls[j].pos)), delta*REPULTION*(twelve(2*RADIUS/d) - sixth(2*RADIUS/d))/d);
        list[i].vel = Vector2Add(list[i].vel, t);
      }
    }
    list[i].pos.x += delta*list[i].vel.x;
    list[i].pos.y += delta*list[i].vel.y;
        
    list[i].vel.y += delta*G/2;

    if (list[i].pos.x > WW - RADIUS) {
      list[i].pos.x = WW - RADIUS;
      list[i].vel.x = -list[i].vel.x;
    }
    if (list[i].pos.x < RADIUS) {
      list[i].pos.x = RADIUS;
      list[i].vel.x = -list[i].vel.x;
    }

    if (list[i].pos.y > WH - RADIUS) {
      list[i].vel.y = -list[i].vel.y;
      list[i].pos.y = WH - RADIUS;
    }
    if (list[i].pos.y < RADIUS) {
      list[i].vel.y = -list[i].vel.y;
      list[i].pos.y = RADIUS;
    }
  }

  return NULL;
}


int main() {
  read_config();
  InitWindow(WW, WH, "Simulation");
  SetTargetFPS(MAX_FPS);

  balls = malloc(NUMBER_OF_BALLS * sizeof(struct Ball));
  struct Ball new_balls[NUMBER_OF_BALLS];

  nthreads = sysconf(_SC_NPROCESSORS_ONLN);
  struct ThreadParam paramethers[nthreads];

  pthread_t threads[nthreads];

	create_balls();
  for (size_t i=0; i<NUMBER_OF_BALLS; i++) {
    new_balls[i].vel.x = balls[i].vel.x;
    new_balls[i].vel.y = balls[i].vel.y;
    new_balls[i].pos.x = balls[i].pos.x;
    new_balls[i].pos.y = balls[i].pos.y;
  }

  while (!WindowShouldClose()) {
    delta = GetFrameTime();

    for (size_t i=0; i<nthreads; i++) {
      paramethers[i].i = i;
      int balls_over_threads;
      if (i == nthreads) balls_over_threads = NUMBER_OF_BALLS%nthreads;
      else balls_over_threads = NUMBER_OF_BALLS/nthreads;

      paramethers[i].ball_list = &(new_balls[balls_over_threads*i]);
      if (pthread_create(&(threads[i]), NULL, collide, &(paramethers[i])) != 0) {
        perror("Didn`t create thread");
        return 1;
      }
    }
    for (size_t i=0; i<nthreads; i++) {
      pthread_join(threads[i], NULL);
    }

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
  free(balls);

  return 0;
}
