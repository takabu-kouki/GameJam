#define NOMINMAX
#include "DeathParticles.h"
#include "MathUtils.h"
void DeathParticles::Initialize(const Vector3& position, ViewProjection* viewProjection) {
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
	viewProjection_ = viewProjection;
	model_ = Model::CreateFromOBJ("deathParticle", true);
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void DeathParticles::Update() {
	if (isFinished_) {
		return;
	}
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();

	}
	for (uint32_t i = 0; i < kNumParticles; i++) {
		Vector3 velocity = {kSpeed, 0, 0};
		float angle = kAngleUnit * i;
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		velocity = Transform(velocity, matrixRotation);
		worldTransforms_[i].translation_ += velocity;
	}
	counter_ += 1.0f / 60.0f;
	if (counter_ >= kDuration) {
        counter_ = kDuration;
		isFinished_ = true;
	}
	color_.w = std::max(0.0f, 1.0f - counter_ / kDuration);
	objectColor_.SetColor(color_);
	objectColor_.TransferMatrix();
}

void DeathParticles::Draw() {
	if (isFinished_) {
		return;
	}
	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *viewProjection_, &objectColor_);
	}
}

bool DeathParticles::IsFinished() const {
	return isFinished_;
}