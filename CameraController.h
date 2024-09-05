#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"

class Player;

// カメラ制御
class CameraController {
public:
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	void Initialize();

	void Update();

	// setter
	void Reset();
	void SetTarget(Player* target) { target_ = target; }
	void SetMovableArea(const Rect& area) { movableArea_ = area; }

	// getter
	const ViewProjection& GetViewProjection() const { return viewProjection_; }

private:
	ViewProjection viewProjection_;
	Player* target_ = nullptr;
	Rect movableArea_ = {0, 100, 0, 100};
	Vector3 destination_;
	Vector3 targetOffset_ = {0, -15.0f, -15.0f};
	static inline const Rect targetMargin = {-9.0f, 9.0f, -5.0f, 5.0f};

	// 座標補間割合
	static inline const float kInterpolationRate = 0.1f;
	// 速度掛け率
	static inline const float kVelocityBias = 30.0f;
};