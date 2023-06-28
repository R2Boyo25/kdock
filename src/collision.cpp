#include "collision.hpp"

bool aabb_collision(aabb other, aabb me) {
  return other.x < me.x + me.width &&
         other.x + other.width > me.x &&
         other.y < me.y + me.height &&
         other.y + other.height > me.y;
}

bool aabb_collision(std::vector<aabb> others, aabb me) {
  for (aabb& other : others) {
    if (aabb_collision(other, me)) {
      return true;
    }
  }
  
  return false;
}
