#include "Skydome.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>

void Skydome::Initialize(Model* model, WorldTransform worldTransform) {
	assert(model);
	model_ = model;
	worldTransform_ = worldTransform;
	input_ = Input::GetInstance();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
}

void Skydome::Update() {}

void Skydome::Draw( ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
