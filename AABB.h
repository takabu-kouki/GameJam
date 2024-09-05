#pragma once
#include "Vector3.h"
struct AABB {
	Vector3 min;
	Vector3 max;
};
bool IsCollision(AABB aabb1, AABB aabb2);