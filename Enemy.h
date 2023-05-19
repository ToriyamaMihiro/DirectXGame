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

enum class Phase {
	Approach,
	Leave,
};

class Enemy {

public:
	std::list<EnemyBullet*> bullets_;
	const std::list<EnemyBullet*>& GetBullets() { return bullets_; }
	static const int kFireInterval = 60;
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Enemy();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, WorldTransform worldTransform);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	void Fire();

	void ApproachInitialize();

	void ApproachUpdate();

	void SetPlayer(Player* player) { player_ = player; };

	Vector3 GetWorldPosition();

	void OnCollision();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);

private:
	uint32_t textureHandle_ = 0u;
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	Input* input_ = nullptr;
	int32_t fire_timer = 0;
	Phase phase_ = Phase::Approach;
	Player* player_ = nullptr;
};
