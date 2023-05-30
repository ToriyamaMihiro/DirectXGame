#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "PlayerBullet.h"
// #include "math.h"
#include <list>
class Player {

public:
	std::list<PlayerBullet*> bullets_;
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, WorldTransform worldTransform);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void Rotate();

	Vector3 GetWorldPosition();

	void OnCollision();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	void Attack();

private:
	uint32_t textureHandle_ = 0u;
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	Input* input_ = nullptr;
	
};
