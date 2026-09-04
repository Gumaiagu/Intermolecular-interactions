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
pthread_mutex_t lock;

float MAX_START_SPEED = 50;

float G = 9.8;

float REPULTION = 1000;

size_t MAX_FPS = 180;

size_t NUMBER_OF_BALLS = 100;
struct Ball* balls;

struct Ball* new_balls;

struct Ball {
  Vector2 pos;
  Vector2 vel;
};


struct ThreadParam {
  size_t i;
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
  size_t i = ((struct ThreadParam*) param)->i;

  int balls_over_threads = NUMBER_OF_BALLS/nthreads;

  size_t ball_end;

  size_t ball_start = i * balls_over_threads;
  if (i+1 == nthreads) {
    balls_over_threads = NUMBER_OF_BALLS % nthreads;
    ball_end = NUMBER_OF_BALLS;
  }
  else{
    ball_end = (i+1) * balls_over_threads;
  }

  for (size_t k=ball_start; k<=ball_end; k++) {
    for (size_t j=0; j<NUMBER_OF_BALLS; j++) {
      if (j != k) {
        float d = Vector2Distance(new_balls[k].pos, balls[j].pos);
        Vector2 t = Vector2Scale(Vector2Normalize(Vector2Subtract(new_balls[k].pos, balls[j].pos)), delta*REPULTION*(twelve(2*RADIUS/d) - sixth(2*RADIUS/d))/d);
        new_balls[k].vel = Vector2Add(new_balls[k].vel, t);
      }
    }
    new_balls[k].pos.x += delta*balls[k].vel.x;
    new_balls[k].pos.y += delta*balls[k].vel.y;
        
    new_balls[k].vel.y += delta*G/2;

    if (balls[k].pos.x > WW - RADIUS) {
      new_balls[k].pos.x = WW - RADIUS;
      new_balls[k].vel.x = -new_balls[k].vel.x;
    }
    if (balls[k].pos.x < RADIUS) {
      new_balls[k].pos.x = RADIUS;
      new_balls[k].vel.x = -new_balls[k].vel.x;
    }

    if (balls[k].pos.y > WH - RADIUS) {
      new_balls[k].vel.y = -new_balls[k].vel.y;
      new_balls[k].pos.y = WH - RADIUS;
    }
    if (balls[k].pos.y < RADIUS) {
      new_balls[k].vel.y = -new_balls[k].vel.y;
      new_balls[k].pos.y = RADIUS;
    }
  }

  pthread_mutex_lock(&lock);
    for (size_t k=ball_start; k<ball_end; k++) {
      balls[k].vel.x = new_balls[k].vel.x;
      balls[k].vel.y = new_balls[k].vel.y;
      balls[k].pos.x = new_balls[k].pos.x;
      balls[k].pos.y = new_balls[k].pos.y;
    }
  pthread_mutex_unlock(&lock);
  return NULL;
}


int main() {
  read_config();
  InitWindow(WW, WH, "Simulation");
  balls = malloc(NUMBER_OF_BALLS * sizeof(struct Ball));
  new_balls = malloc(NUMBER_OF_BALLS * sizeof(struct Ball));
  SetTargetFPS(MAX_FPS);
  nthreads = sysconf(_SC_NPROCESSORS_ONLN);
  struct ThreadParam paramethers[nthreads];

  pthread_t threads[nthreads];
  pthread_mutex_init(&lock, NULL);

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
      paramethers[i] = (struct ThreadParam) {i};
      if (pthread_create(&(threads[i]), NULL, collide, paramethers + i) != 0) {
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
        DrawCircle(balls[i].pos.x, balls[i].pos.y, RADIUS, RED);
        new_balls[i].vel.x = balls[i].vel.x;
        new_balls[i].vel.y = balls[i].vel.y;
        new_balls[i].pos.x = balls[i].pos.x;
        new_balls[i].pos.y = balls[i].pos.y;
      }
      DrawFPS(0, 0);
    EndDrawing();
  }
  pthread_mutex_destroy(&lock);
  free(balls);

  return 0;
}
