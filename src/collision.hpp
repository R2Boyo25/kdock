#pragma once
#include <vector>

// axis-aligned bounding box
typedef struct {
  int x;
  int y;
  int width;
  int height;
} aabb;

bool aabb_collision(aabb other, aabb me);
bool aabb_collision(std::vector<aabb> others, aabb me);
