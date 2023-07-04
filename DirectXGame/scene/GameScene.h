#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Enemy.h"

#include "Player.h"
#include <cassert>
#include "RailCamera.h"
#include "Skydome.h"
#include<sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数

	std::list<EnemyBullet*> bullets_;
	const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

	std::list<Enemy*> enemys_;
	const std::list<Enemy*>& EnemyGetBullets() { return enemys_; }
	static const int kFireInterval = 60;

	std::stringstream enemyPopCommands;


	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void SetEnemy(const Vector3 position);

	void CheckAllCollisions();

	void AddEnemyBullet(EnemyBullet* enemyBullet);

	void Fire();

	void ApproachInitialize();

	void ApproachUpdate();

	void LoadEnemyPopDate();

	void UpdateEnemyPopCommands();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t textureHandle_ = 0;
	uint32_t enemyTextureHandle_ = 0;
	uint32_t skydomeTextureHandle_ = 0;

	Model* model_ = nullptr;
	Model* enemyModel_ = nullptr;
	Model* skydomeModel_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	DebugCamera* debugCamera_ = nullptr;
	int isDebugCameraActive_ = 0;

	int32_t fire_timer = 0;
	Vector3 velocity_;
	Vector3 enemy_pos;

	Player* player_ = nullptr;
	Skydome* skydome_ = nullptr;
	RailCamera* railCamera_ = nullptr;

	int waitCount = 0;
	int waitFlg = true;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
