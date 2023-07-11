/**
 * @file
 * @brief Utilites for collision checking.
 */

#pragma once
#include <vector>
#include <stdio.h>

/// @brief Axis-Aligned Bounding-Box
typedef struct {
  double x;      ///< X position of top left
  double y;      ///< Y position of top left
  double width;  ///< Width of AABB
  double height; ///< Height of AABB
} aabb;

/**
 * Determine if two AABBs collide.
 *
 * @param other The other AABB to check.
 * @param me    The AABB you are checking from.
 * @return      Whether they collided.
 */
bool aabb_collision(aabb other, aabb me);

/**
 * Determine if a AABB collides with any of multiple AABBs
 *
 * @param others The list of other AABBs to check.
 * @param me     The AABB you are checking from.
 * @return       Whether there was any collisions.
 */
bool aabb_collision(std::vector<aabb> others, aabb me);

/**
 * Print an AABB
 *
 * @param bb The AABB to print (bounding-box)
 * @param fd the file to output to.
 */
void print_aabb(aabb bb, FILE *fd = stdout);
