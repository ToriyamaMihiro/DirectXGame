#include "Enemy.h"
#include "Affine.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>

Enemy::~Enemy() {}

void Enemy::Initialize(Model* model, uint32_t textureHandle, WorldTransform worldTransform) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_ = worldTransform;
	input_ = Input::GetInstance();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
}

void Enemy::Update() {

	Vector3 move = {0, 0, 0}; // 移動ベクトル

	const float kCharactorSpeed = -0.2f;

	move.z += kCharactorSpeed;

	MakeScaleMatrix(worldTransform_.scale_);

	MakeRatateXMatrix(worldTransform_.rotation_.x);
	MakeRatateYMatrix(worldTransform_.rotation_.y);
	MakeRatateZMatrix(worldTransform_.rotation_.z);
	MakeRatateXYZMatrix(worldTransform_.rotation_);

	MakeTranslateMatrix(worldTransform_.translation_);

	worldTransform_.matWorld_ = MakeAffinMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.UpdateMatrix();

	worldTransform_.translation_.z += move.z; // プレイworldTransform_.translation_.y += move.y;

	ImGui::Begin("Debug1");
	ImGui::Text("%f", worldTransform_.translation_.x);
	ImGui::Text("%f", worldTransform_.translation_.y);
	ImGui::Text("%f", worldTransform_.translation_.z);

	ImGui::End();

	const float kMoveLimitX = 34;
	const float kMoveLimitY = 19;
	// 移動範囲の指定
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}