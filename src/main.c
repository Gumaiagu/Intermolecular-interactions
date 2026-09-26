#include "ball_phisics.c"
#include "config.c"
#include "values.c"

#include <pthread.h> // Creates and joins threads

// Get number of threads in OS
#ifdef __unix__
#include <unistd.h>
#else
#include <sysinfoapi.h>
#endif

void create_balls(struct Ball new_balls[NUMBER_OF_BALLS]) {
  for (size_t i = 0; i < NUMBER_OF_BALLS; i++) {
    new_balls[i].pos.x = balls[i].pos.x = GetRandomValue(0, WW);
    new_balls[i].pos.y = balls[i].pos.y = GetRandomValue(0, WH);

    new_balls[i].vel.x = balls[i].vel.x =
        GetRandomValue(-MAX_START_SPEED, MAX_START_SPEED);
    new_balls[i].vel.y = balls[i].vel.y =
        GetRandomValue(-MAX_START_SPEED, MAX_START_SPEED);
    
    new_balls[i].mass = balls[i].mass = GetRandomValue(1, MAX_MASS);

    bool on_invalid_pos = true;
    for (size_t j = 0; j < i && on_invalid_pos; j++) {
      if (Vector2Distance(balls[j].pos, balls[i].pos) <= 2 * RADIUS) {
        i--;
        on_invalid_pos = false;
      }
    }
  }
}

int create_and_finish_threads(struct ThreadParam paramethers[nthreads],
                              struct Ball new_balls[NUMBER_OF_BALLS],
                              pthread_t threads[nthreads]) {
  for (size_t i = 0; i < nthreads; i++) {
    paramethers[i].i = i;
    int balls_over_threads = NUMBER_OF_BALLS / nthreads;

    paramethers[i].ball_list = &(new_balls[balls_over_threads * i]);
    pthread_create(&(threads[i]), NULL, ball_phisics, &(paramethers[i]));
    ON_ERROR("Could not create threads");
  }
  for (size_t i = 0; i < nthreads; i++) {
    pthread_join(threads[i], NULL);
  }
  ON_ERROR("An error occured");
  return 0;
}

double decimal_part(double x) { return x - floor(x); }
double absolute(double x) {
  if (x >= 0) {
    return x;
  } else {
    return -x;
  }
}

Color create_ball_color(struct Ball ball) {
  Color a;
  if (COLOR_DEFINITION == 0) {
    double speed = Vector2Distance(Vector2Zero(), ball.vel);
    if (speed <= MAX_COLOR) {
      a.r = floor(speed*(255/MAX_COLOR));
      a.b = floor(255 - speed*(255/MAX_COLOR));
    }
    else {
      a.r = 255;
      a.b = 0;
    }
  }
  else {
    a.r = floor(ball.mass*(255/MAX_MASS));       
    a.b = floor(255 - ball.mass*(255/MAX_MASS)); 
  }
  a.g = 0;
  a.a = 255;
  return a;
}

void draw_balls(struct Ball new_balls[NUMBER_OF_BALLS]) {
  BeginDrawing();
  ClearBackground(BLACK);

  bool change_t = absolute(decimal_part(GetTime())) >= .9;
  if (change_t) T = 0;

  for (size_t i = 0; i < NUMBER_OF_BALLS; i++) {
    if (IsKeyDown(KEY_O))
      new_balls[i].vel = Vector2Scale(new_balls[i].vel, 1 - change);
    else if (IsKeyDown(KEY_P))
      new_balls[i].vel = Vector2Scale(new_balls[i].vel, 1 + change);

    // Clone balls
    balls[i].vel.x = new_balls[i].vel.x;
    balls[i].vel.y = new_balls[i].vel.y;
    balls[i].pos.x = new_balls[i].pos.x;
    balls[i].pos.y = new_balls[i].pos.y;

#ifdef __unix__
    if (change_t)
      T += Vector2Distance(balls[i].vel, Vector2Zero());
#else
    if (change_t)
      T += Vector2Distance(balls[i].vel, Vector2Zero()) / NUMBER_OF_BALLS;
#endif

    DrawCircle(balls[i].pos.x, balls[i].pos.y, RADIUS, create_ball_color(balls[i]));
  }

  char my_string[9];
#ifdef __unix__
  snprintf(my_string, 9, "%.*lf", 3, T / NUMBER_OF_BALLS);
#else
  snprintf(my_string, 9, "%.*lf", 3, T);
#endif
  DrawText(my_string, WW / 2 - 10, 0, 20, WHITE);
  DrawFPS(0, 0);
  EndDrawing();
}

int main() {
  read_config();
  ON_ERROR("Something whent wrong while reading config file");

  if (nthreads == INT_MAX) {
#ifdef __unix__
    nthreads = sysconf(_SC_NPROCESSORS_ONLN);
#else
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    nthreads = sysInfo.dwNumberOfProcessors;
#endif
  }

  if (NUMBER_OF_BALLS < nthreads)
    nthreads = 1;
#ifdef __unix__
  printf("Using %ld threads.\n", nthreads);
#else
  printf("Using %lld threads.\n", nthreads);
#endif
  struct ThreadParam paramethers[nthreads];

  pthread_t threads[nthreads];

  InitWindow(WW, WH, "Simulation");
  SetTargetFPS(MAX_FPS);
  errno = 0; // Raylib creates non critical errors.

  balls = calloc(NUMBER_OF_BALLS, sizeof(struct Ball));
  ON_ERROR("Could not create balls");
  struct Ball new_balls[NUMBER_OF_BALLS];

  create_balls(new_balls);

  while (!WindowShouldClose()) {
    delta = GetFrameTime();

    create_and_finish_threads(paramethers, new_balls, threads);
    ON_ERROR("Could not create threads properly");

    draw_balls(new_balls);
    errno = 0; // Raylib creates non critical errors.
  }
  free(balls);
  CloseWindow();

  return 0;
}
