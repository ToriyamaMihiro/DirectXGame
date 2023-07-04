#include "Enemy.h"
#include "Affine.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include"Player.h"


Enemy::~Enemy() {
	
}

void Enemy::Initialize( Model* model, const Vector3& position, const Vector3& velocity ) {
	assert(model);
	model_ = model;
	input_ = Input::GetInstance();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	textureHandle_ = TextureManager::Load("syadow_kh.png");
	// Fire();
	
	worldTransform_.translation_ = position;
	velocity_ = velocity;

}

void Enemy::Update() {
		
	// 移動
	Vector3 move = {0, 0, 0}; // 移動ベクトル

	const float kCharactorSpeed = -0.5f;

	//move.x += kCharactorSpeed;

	MakeScaleMatrix(worldTransform_.scale_);

	MakeRatateXMatrix(worldTransform_.rotation_.x);
	MakeRatateYMatrix(worldTransform_.rotation_.y);
	MakeRatateZMatrix(worldTransform_.rotation_.z);
	MakeRatateXYZMatrix(worldTransform_.rotation_);

	MakeTranslateMatrix(worldTransform_.translation_);

	worldTransform_.matWorld_ = MakeAffinMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.UpdateMatrix();

	worldTransform_.translation_.x += move.x; // プレイworldTransform_.translation_.y += move.y;
	
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 19;		
}



Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Enemy::OnCollision() { isDead_ = 1; }

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}