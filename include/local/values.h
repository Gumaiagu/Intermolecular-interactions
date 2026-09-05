#pragma once
#include <raylib.h>   // Defines Vector2
#include <stddef.h>

int WW = 800;
int WH = 500;

float delta;

int RADIUS = 5;

size_t nthreads;

float MAX_START_SPEED = 50;

float G = 0;

float REPULTION = 1;

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


