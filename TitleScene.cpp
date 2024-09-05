#define NOMINMAX
#include "TitleScene.h"
#include "DirectXCommon.h"
#include "Input.h"
#include <cmath>
#include <numbers>
#include "TextureManager.h"
TitleScene::~TitleScene() {
	delete modelPlayer_;
	delete modelTitle_;
	/*delete backgroundSprite_;*/
}
void TitleScene::Initialize() { 
	modelTitle_ = Model::CreateFromOBJ("titleFont", true);
	modelPlayer_ = Model::CreateFromOBJ("player");
	viewProjection_.Initialize();
	const float kTitleScale = 10.0f;
	worldTransformTitle_.Initialize();
	worldTransformTitle_.scale_ = {kTitleScale, kTitleScale, kTitleScale};
	worldTransformTitle_.rotation_.x = std::numbers::pi_v<float> / 2.0f;
	worldTransformTitle_.rotation_.y = std::numbers::pi_v<float>;
	worldTransformTitle_.translation_.x = -19.0f;
	const float kPlayerScale = 10.0f;
	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {kPlayerScale, kPlayerScale, kPlayerScale};
	worldTransformPlayer_.rotation_.y = 0.95f * std::numbers::pi_v<float>;
	worldTransformPlayer_.translation_.x = -2.0f;
	worldTransformPlayer_.translation_.y = -10.0f;
	/*backgroundTH_ = TextureManager::Load("titleBackground.png");
	backgroundSprite_ = Sprite::Create(backgroundTH_, {0, 0});*/
}
void TitleScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeTitleMove);
	float angle = counter_ / kTimeTitleMove * 2.0f * std::numbers::pi_v<float>;
	worldTransformTitle_.translation_.y = std::sin(angle) + 10.0f;
	viewProjection_.TransferMatrix();
	worldTransformTitle_.UpdateMatrix();
	worldTransformPlayer_.UpdateMatrix();
}
void TitleScene::Draw() {
	DirectXCommon* dxCommon_=DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	//// 背景スプライト描画前処理
	//Sprite::PreDraw(commandList);

	///// <summary>
	///// ここに背景スプライトの描画処理を追加できる
	///// </summary>
	//
	//// スプライト描画後処理
	//Sprite::PostDraw();
	//// 深度バッファクリア
	//dxCommon_->ClearDepthBuffer();
	Model::PreDraw(commandList);
	modelTitle_->Draw(worldTransformTitle_, viewProjection_);
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_);
	Model::PostDraw();
}
bool TitleScene::IsFinished() const {
	return finished_;
}