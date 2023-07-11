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
	void Initialize(Model* model, uint32_t textureHandle, Vector3 position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void GetPointer();

	void Rotate();

	Vector3 GetWorldPosition();

	Vector3 Get3DWorldPosition();

	void OnCollision();

	// 親となるワールドトランスフォームをセット
	void SetParent(const WorldTransform* parent);

	WorldTransform worldTransform3DReticle_;

	void Attack();

	void DrawUI();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);


private:
	Sprite* sprite2DReiticle_ = nullptr;
	uint32_t textureHandle_ = 0u;
	uint32_t textureReticle = 0u;
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	Vector3 position_;
	Input* input_ = nullptr;

	
};
