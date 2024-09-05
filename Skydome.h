#include "Model.h"
#include "WorldTransform.h"
#include "Viewprojection.h"

class Skydome{
public:
	void Initialize(Model* model, ViewProjection* viewProjection); 

	void Update();

	void Draw();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	// モデル
	Model* model_ = nullptr;
};