#include "Enemy.h"
#include "MathUtils.h"
#include <numbers>
void Enemy::Initialize(const Vector3& position, ViewProjection* viewProjection) {
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	viewProjection_ = viewProjection;

	worldTransform_.rotation_.y = -(std::numbers::pi_v<float> / 2.0f);

	// 引数の内容をメンバ変数に記録
	model_ = Model::CreateFromOBJ("enemy", true);
	
	velocity_ = {-kWalkSpeed, 0, 0};
	walkTimer_ = 0.0f;

}
void Enemy::Update() {
	walkTimer_ += 1.0f / 60.0f;
	float param = std::sin(2 * std::numbers::pi_v<float> * walkTimer_ / kWalkMotionTime);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x =radian;
	worldTransform_.UpdateMatrix();
	worldTransform_.translation_ += velocity_;

}
void Enemy::Draw() {
	model_->Draw(worldTransform_, *viewProjection_);
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

 void Enemy::SetWorldPositionX(float pos) {
	worldTransform_.translation_.x = pos;
}

AABB Enemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}

void Enemy::OnCollision(const Player* player) {
	(void)player;
}