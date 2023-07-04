#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "EnemyBullet.h"
#define _USE_MATH_DEFINES
#include <math.h>
// #include "math.h"
#include <list>

class Player;
class GameScene;

enum class Phase {
	Approach,
	Leave,
};

class Enemy {

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(
	    Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void SetPlayer(Player* player) { player_ = player; };

	Vector3 GetWorldPosition();

	void OnCollision();

	bool IsDead() const { return isDead_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; };

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

private:
	int32_t fire_timer = 0;
	uint32_t textureHandle_ = 0u;
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	Input* input_ = nullptr;
	Vector3 position_;
	Vector3 velocity_;
	bool isDead_ = false;

	Phase phase_ = Phase::Approach;
	Player* player_ = nullptr;
	GameScene* gameScene_ = nullptr;
};
