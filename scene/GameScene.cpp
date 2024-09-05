#include "GameScene.h"
#include "TextureManager.h"
#include "MathUtils.h"
#include "CameraController.h"
#include "AABB.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}

	worldTransformBlocks_.clear();

	for (std::vector<WorldTransform*>& worldTransformGoalBlockLine : worldTransformGoalBlocks_) {
		for (WorldTransform* worldTransformGoalBlock : worldTransformGoalBlockLine) {
			delete worldTransformGoalBlock;
		}
	}

	worldTransformGoalBlocks_.clear();

	for (std::vector<WorldTransform*>& worldTransformSaveBlockLine : worldTransformSaveBlocks_) {
		for (WorldTransform* worldTransformSaveBlock : worldTransformSaveBlockLine) {
			delete worldTransformSaveBlock;
		}
	}

	worldTransformGoalBlocks_.clear();

	delete debugCamera_;

	delete modelSkydome_;

	// マップチップフィールドの解放
	delete mapChipField_;

	delete cameraController;

	delete player_;

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear();

	delete deathParticles_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("block.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();
	modelBlock_ = Model::CreateFromOBJ("block");
	modelGoal_ = Model::CreateFromOBJ("goalBlock");
	modelSave_ = Model::CreateFromOBJ("saveBlock");
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(3,13);
	player_->Initialize(playerPosition,&viewProjection_);

	// 天球の生成
	skydome_ = new Skydome();
	// 天球3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("sphere", true);
	// 天球の初期化
	skydome_->Initialize(modelSkydome_,&viewProjection_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280,720);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	player_->SetMapChipField(mapChipField_);

	GenerateBlocks();
	GenerateGoalBlocks();
	GenerateSaveBlocks();

	cameraController = new CameraController;
	cameraController->Initialize();
	cameraController->SetTarget(player_);
	cameraController->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 15.0f};
	cameraController->SetMovableArea(cameraArea);

	for (int32_t i = 0; i < 3; i++) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(15, 18-i);
		newEnemy->Initialize(enemyPosition, &viewProjection_);
		enemies_.push_back(newEnemy);
	}
	for (int32_t i = 0; i < 3; i++) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(48, 18 - i);
		newEnemy->Initialize(enemyPosition, &viewProjection_);
		enemies_.push_back(newEnemy);
	}
	for (int32_t i = 0; i < 3; i++) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(81, 18 - i);
		newEnemy->Initialize(enemyPosition, &viewProjection_);
		enemies_.push_back(newEnemy);
	}
	phase_ = Phase::kPlay;
}

void GameScene::GenerateBlocks() {

	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
	
	// 要素数を変更する
	worldTransformBlocks_.resize(numBlockVirtical);

	// キューブの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::GenerateGoalBlocks() {

	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	worldTransformGoalBlocks_.resize(numBlockVirtical);

	// キューブの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformGoalBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kGoalBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformGoalBlocks_[i][j] = worldTransform;
				worldTransformGoalBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::GenerateSaveBlocks() {

	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	worldTransformSaveBlocks_.resize(numBlockVirtical);

	// キューブの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformSaveBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kSaveBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformSaveBlocks_[i][j] = worldTransform;
				worldTransformSaveBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions() {
	AABB aabb1,aabb2;
#pragma region
	{ 
		aabb1 = player_->GetAABB();
		for (Enemy* enemy : enemies_) {
			aabb2 = enemy->GetAABB();
			if (IsCollision(aabb1, aabb2)) {
				player_->OnCollision(enemy);
				enemy->OnCollision(player_);
			}
		}
	}
}

void GameScene::Update() {

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		if (isDebugCameraActive_ == true)
			isDebugCameraActive_ = false;
		else
			isDebugCameraActive_ = true;
	}
#endif

	ChangePhase();
	switch (phase_) {
	case Phase::kPlay:
		// 天球の更新
		skydome_->Update();
		// 自キャラの更新
		player_->Update();
		for (Enemy* enemy : enemies_) {
			if (enemy != nullptr) {
				enemy->Update();
			}
			if (enemy->GetWorldPosition().x < mapChipField_->GetMapChipPositionByIndex(0, 18).x) {
				enemy->SetWorldPositionX(mapChipField_->GetMapChipPositionByIndex(100, 0).x);
			}
		}

		cameraController->Update();

		// カメラ処理
		if (isDebugCameraActive_) {
			// デバッグカメラの更新
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		} else {
			// ビュープロジェクション行列の更新と転送
			//	        	viewProjection_.UpdateMatrix();
			viewProjection_.matView = cameraController->GetViewProjection().matView;
			viewProjection_.matProjection = cameraController->GetViewProjection().matProjection;
			// ビュープロジェクションの転送
			viewProjection_.TransferMatrix();
		}
		// 縦横ブロック更新
		for (std::vector<WorldTransform*> worldTransformBlockTate : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlockYoko : worldTransformBlockTate) {
				if (!worldTransformBlockYoko)
					continue;

				// アフィン変換行列の作成
				//(MakeAffineMatrix：自分で作った数学系関数)
				worldTransformBlockYoko->matWorld_ = MakeAffineMatrix(worldTransformBlockYoko->scale_, worldTransformBlockYoko->rotation_, worldTransformBlockYoko->translation_);

				// 定数バッファに転送
				worldTransformBlockYoko->TransferMatrix();
			}
		}

		for (std::vector<WorldTransform*> worldTransformGoalBlockTate : worldTransformGoalBlocks_) {
			for (WorldTransform* worldTransformGoalBlockYoko : worldTransformGoalBlockTate) {
				if (!worldTransformGoalBlockYoko)
					continue;

				// アフィン変換行列の作成
				//(MakeAffineMatrix：自分で作った数学系関数)
				worldTransformGoalBlockYoko->matWorld_ = MakeAffineMatrix(worldTransformGoalBlockYoko->scale_, worldTransformGoalBlockYoko->rotation_, worldTransformGoalBlockYoko->translation_);

				// 定数バッファに転送
				worldTransformGoalBlockYoko->TransferMatrix();
			}
		}

		for (std::vector<WorldTransform*> worldTransformSaveBlockTate : worldTransformSaveBlocks_) {
			for (WorldTransform* worldTransformSaveBlockYoko : worldTransformSaveBlockTate) {
				if (!worldTransformSaveBlockYoko)
					continue;

				// アフィン変換行列の作成
				//(MakeAffineMatrix：自分で作った数学系関数)
				worldTransformSaveBlockYoko->matWorld_ = MakeAffineMatrix(worldTransformSaveBlockYoko->scale_, worldTransformSaveBlockYoko->rotation_, worldTransformSaveBlockYoko->translation_);

				// 定数バッファに転送
				worldTransformSaveBlockYoko->TransferMatrix();
			}
		}
		CheckAllCollisions();
		break;
	case Phase::kDeath:
		// 天球の更新
		skydome_->Update();
		for (Enemy* enemy : enemies_) {
			if (enemy != nullptr) {
				enemy->Update();
			}
		}
		deathParticles_->Update();
		// カメラ処理
		if (isDebugCameraActive_) {
			// デバッグカメラの更新
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		} else {
			// ビュープロジェクション行列の更新と転送
			//	        	viewProjection_.UpdateMatrix();
			viewProjection_.matView = cameraController->GetViewProjection().matView;
			viewProjection_.matProjection = cameraController->GetViewProjection().matProjection;
			// ビュープロジェクションの転送
			viewProjection_.TransferMatrix();
		}
		// 縦横ブロック更新
		for (std::vector<WorldTransform*> worldTransformBlockTate : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlockYoko : worldTransformBlockTate) {
				if (!worldTransformBlockYoko)
					continue;

				// アフィン変換行列の作成
				//(MakeAffineMatrix：自分で作った数学系関数)
				worldTransformBlockYoko->matWorld_ = MakeAffineMatrix(worldTransformBlockYoko->scale_, worldTransformBlockYoko->rotation_, worldTransformBlockYoko->translation_);

				// 定数バッファに転送
				worldTransformBlockYoko->TransferMatrix();
			}
		}
		break;
	}
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kPlay:
		   if (player_->IsDead()) {
			    phase_ = Phase::kDeath;
			    const Vector3& deathParticlesPosition = player_->GetWorldPosition();
	            deathParticles_ = new DeathParticles;
			    deathParticles_->Initialize(deathParticlesPosition, &viewProjection_);
           }
		   if (player_->GetHitGoal()) {
			    finished_ = true;
		   }
		    break;
	case Phase::kDeath:
		    if (deathParticles_ && deathParticles_->IsFinished()) {
			    if (player_->GetSpawn() == 0) {
				    finished_ = true;
			    } else if (player_->GetSpawn() == 1) {
				phase_ = Phase::kPlay;
				
				player_->Respawn(mapChipField_->GetMapChipPositionByIndex(52, 10));

			    }
			}
		    break;
			case Phase::kGoal:///ここ分からぬ
		if (deathParticles_ && deathParticles_->IsFinished()) {
			finished_ = true;
		}


		if (deathParticles_) {
			deathParticles_->Update();
		}

		break;

	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 3Dモデル描画
//	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	// 自キャラの描画
	if (!player_->IsDead()) {
		player_->Draw();
    }
	
	for (Enemy* enemy : enemies_) {
	    if (enemy != nullptr) {
		    enemy->Draw();
	    }

	}
	
	if (deathParticles_) {
		deathParticles_->Draw();
	}
	// 天球の描画
	skydome_->Draw();


	//縦横ブロック描画
	for (std::vector<WorldTransform*> worldTransformBlockTate : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlockYoko : worldTransformBlockTate) {
			if (!worldTransformBlockYoko)
				continue;
			
			modelBlock_->Draw(*worldTransformBlockYoko, viewProjection_);
		}
	}
	for (std::vector<WorldTransform*> worldTransformGoalBlockTate : worldTransformGoalBlocks_) {
		for (WorldTransform* worldTransformGoalBlockYoko : worldTransformGoalBlockTate) {
			if (!worldTransformGoalBlockYoko)
				continue;

			modelGoal_->Draw(*worldTransformGoalBlockYoko, viewProjection_);
		}
	}
	for (std::vector<WorldTransform*> worldTransformSaveBlockTate : worldTransformSaveBlocks_) {
		for (WorldTransform* worldTransformSaveBlockYoko : worldTransformSaveBlockTate) {
			if (!worldTransformSaveBlockYoko)
				continue;

			modelSave_->Draw(*worldTransformSaveBlockYoko, viewProjection_);
		}
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
