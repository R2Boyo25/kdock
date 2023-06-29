#pragma once
#include <vector>

// axis-aligned bounding box
typedef struct {
  double x;
  double y;
  double width;
  double height;
} aabb;

bool aabb_collision(aabb other, aabb me);
bool aabb_collision(std::vector<aabb> others, aabb me);
void print_aabb(aabb bb);
