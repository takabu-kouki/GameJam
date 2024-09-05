#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "AABB.h"
#include <numbers>

class Player;

class Enemy {
public:
	void Initialize(const Vector3& position, ViewProjection* viewProjection);
	void Update();
	void Draw();
	void OnCollision(const Player* player);
	Vector3 GetWorldPosition();
	void SetWorldPositionX(float pos);
	AABB GetAABB();
	
private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	Model* model_ = nullptr;
	Vector3 velocity_ = {};
	float walkTimer_ = 0.0f;

	static inline const float kWalkSpeed = 0.05f;
	static inline const float kWalkMotionAngleStart = std::numbers::pi_v<float>/6.0f;
	static inline const float kWalkMotionAngleEnd = -(std::numbers::pi_v<float> / 4.0f);
	static inline const float kWalkMotionTime = 1.0f;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
};
