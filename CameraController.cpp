#include <algorithm>
#include "CameraController.h"
#include "MathUtils.h"
#include "Player.h"

void CameraController::Initialize() { viewProjection_.Initialize(); }

void CameraController::Update() {
	// 追従対象のワールドトランスフォームを取得
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	const Vector3& targetVelocity = target_->GetVelocity();

	// 追従対象の座標、移動速度とオフセットで目標座標を計算
	destination_ = targetWorldTransform.translation_ + targetVelocity * kVelocityBias + targetOffset_;
	// 座標補間によりゆったり追従
	viewProjection_.translation_ = Lerp(viewProjection_.translation_, destination_, kInterpolationRate);
	// 追従対象が画面外に出ないように補正
	viewProjection_.translation_.x = std::clamp(viewProjection_.translation_.x, targetWorldTransform.translation_.x + targetMargin.left, targetWorldTransform.translation_.x + targetMargin.right);
	viewProjection_.translation_.y = std::clamp(viewProjection_.translation_.y, targetWorldTransform.translation_.y + targetMargin.bottom, targetWorldTransform.translation_.y + targetMargin.top);
	// 移動範囲制限
	viewProjection_.translation_.x = std::clamp(viewProjection_.translation_.x, movableArea_.left, movableArea_.right);
	viewProjection_.translation_.y = std::clamp(viewProjection_.translation_.y, movableArea_.bottom, movableArea_.top);

	viewProjection_.UpdateMatrix();
}

void CameraController::Reset() { // 追従対象のワールドトランスフォームを取得
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	viewProjection_.translation_ = targetWorldTransform.translation_ + targetOffset_;
}