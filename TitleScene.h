#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Sprite.h"
class TitleScene {
public:
	~TitleScene();
	void Initialize();
	void Update();
	void Draw();
	bool IsFinished() const;

private:
	bool finished_ = false;
	float counter_ = 0.0f;
	static inline const float kTimeTitleMove = 2.0f;
	ViewProjection viewProjection_;
	WorldTransform worldTransformTitle_;
	WorldTransform worldTransformPlayer_;

	Model* modelPlayer_ = nullptr;
	Model* modelTitle_ = nullptr;
	
};
