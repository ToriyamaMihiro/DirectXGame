#include "RailCamera.h"
#include "Affine.h"
#include "ImGuiManager.h"
#include "WorldTransform.h"

void RailCamera::Initialize(WorldTransform worldTransform, Vector3 rotation) {
	worldTransform_ = worldTransform;	

	worldTransform_.rotation_ = rotation;
	viewProjection_.Initialize();
	worldTransform_.Initialize();

	worldTransform_.translation_ = {0, 0, -20};
}

void RailCamera::Update() {

	// 移動
	Vector3 move = {0, 0, 0}; // 移動ベクトル

	const float kCharactorSpeed = 0.01f;

	move.x += kCharactorSpeed;
	move.y += kCharactorSpeed;
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

	// 移動
	worldTransform_.translation_.z -= move.z;

	// 回転
	const float kRotSpeed = 0.01f;
	//worldTransform_.rotation_.y += kRotSpeed;
	

	//worldTransform_.translation_.x += 0.01f;

	worldTransform_.UpdateMatrix();

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	/*ImGui::Begin("Camera");
	ImGui::SliderFloat3("SliderFloat3", worldTransform_.translation_, 0.0f, 1.0f);
	ImGui::SliderAngle("SliderFloat3", worldTransform_.rotation_, 0.0f, 1.0f);
	ImGui::End();*/
}
