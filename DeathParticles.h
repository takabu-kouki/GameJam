#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <array>
#include <numbers>
#include "ObjectColor.h"
class DeathParticles {
public:
	void Initialize(const Vector3& position, ViewProjection* viewProjection);
	void Update();
	void Draw();
	bool IsFinished() const;

private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	static inline const uint32_t kNumParticles = 8;
	static inline const float kDuration = 1.0f;
	static inline const float kSpeed = 0.1f;
	static inline const float kAngleUnit = 2 * std::numbers::pi_v<float> / kNumParticles;
	std::array<WorldTransform, kNumParticles> worldTransforms_;
	bool isFinished_ = false;
	float counter_ = 0.0f;
	ObjectColor objectColor_;
	Vector4 color_;
};
