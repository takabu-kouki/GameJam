#pragma once
#include "AABB.h"
bool IsCollision(AABB aabb1, AABB aabb2) {
	if (aabb1.max.x < aabb2.min.x || aabb1.min.x > aabb2.max.x)
		return false;

	if (aabb1.max.y < aabb2.min.y || aabb1.min.y > aabb2.max.y)
		return false;

	if (aabb1.max.z < aabb2.min.z || aabb1.min.z > aabb2.max.z)
		return false;

	return true;
}