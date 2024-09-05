#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
//	Vector3 operator+=(Vector3& other);

	float x;
	float y;
	float z;
};

/*
Vector3 Vector3::operator+=(Vector3& other) {
    return Vector3(this->x += other.x, this->y += other.y, this->z += other.z);
}
*/

