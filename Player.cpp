#include "Player.h"
#include "TextureManager.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle,WorldTransform worldTransform) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_ = worldTransform;
}

void Player::Update() { worldTransform_.TransferMatrix(); }

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
