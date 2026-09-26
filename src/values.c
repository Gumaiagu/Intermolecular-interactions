#pragma once
#include <limits.h>
#include <errno.h>
#include <raylib.h> // Defines Vector2
#include <stddef.h> // Defines size_t

#define ON_ERROR(message)                                                      \
  do {                                                                         \
    if (errno != 0) {                                                          \
      perror(message);                                                         \
      return errno;                                                            \
    }                                                                          \
  } while (0)

int WW = 800;
int WH = 800;

float delta;

float change = 0.01;

int RADIUS = 5;

size_t nthreads = INT_MAX;

float MAX_START_SPEED = 50;

float G = 0;

float REPULTION = 1;

size_t MAX_FPS = 180;

double T = 0;

double MAX_COLOR = 1;

double MAX_MASS = 1;

int COLOR_DEFINITION = 1;

size_t NUMBER_OF_BALLS = 100;
struct Ball *balls;

struct Ball {
  Vector2 pos;
  Vector2 vel;
  double mass;
};

struct ThreadParam {
  size_t i;
  struct Ball *ball_list;
};

#define sixth(x) x * x * x * x * x *x
#define twelve(x) sixth(x) * sixth(x)
