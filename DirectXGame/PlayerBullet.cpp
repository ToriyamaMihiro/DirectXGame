#include "PlayerBullet.h"



void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("bullet.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;
}

void PlayerBullet::Update() {
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
	worldTransform_.UpdateMatrix();
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::OnCollision() { isDead_ = 1; }

Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	worldTransform_.translation_ = GetWorldPosition();
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
