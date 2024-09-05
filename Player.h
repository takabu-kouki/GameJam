#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "AABB.h"
class MapChipField;
class Enemy;
/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};
	// 角
	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,

		kNumCorner
	};

	/// 初期化
	void Initialize(const Vector3& position, ViewProjection *viewProjection);

	/// 更新
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const Vector3& GetVelocity() const { return velocity_; }
	Vector3 GetWorldPosition();
	AABB GetAABB();
	bool IsDead() const;
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	//入力
	void InputMove();

	Vector3 CornerPosition(const Vector3& center, Corner corner);
	void AnimateTurn();
	void OnCollision(const Enemy* enemy);

    int GetSpawn() { return spawn_; }
	bool GetHitGoal() { return hitGoal_; }
	void Respawn(const Vector3& position);

private:
	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		Vector3 move;
	};

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	Vector3 velocity_ = {};

	bool isDead_ = false;

	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.09f;
	static inline const float kLimitRunSpeed = 0.3f;

	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;

	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;

	// 接地状態フラグ
	bool onGround_ = true;
	// 着地フラグ
	bool landing = false;

	// 重力加速度（下方向）
	static inline const float kGravityAcceleration = 0.98f;
	// 最大落下速度（下方向）
	static inline const float kLimitFallSpeed = 0.5f;
	// ジャンプ初速（上方向）
	static inline const float kJumpAcceleration = 20.f;

	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kBlank = 0.04f;
	static inline const float kGroundSearchHeight = 0.06f;
	static inline const float kAttenuationWall = 0.2f;
	static inline const float kAttenuationLanding = 0.0f;

	void CheckMapCollision(CollisionMapInfo& info);
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);
    void CheckMapCollisionRight(CollisionMapInfo& info);
	void UpdateOnGround(const CollisionMapInfo& info);
	
	
	int spawn_ = 0;
	bool hitGoal_ = false;

};

