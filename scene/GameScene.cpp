#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>


GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete enemy_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("sora_kh.png");
	enemyTextureHandle_ = TextureManager::Load("syadow_kh.png");
	model_ = Model::Create();
	enemyModel_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	enemyWorldTransform_.Initialize();
	enemyViewProjection_.Initialize();

	debugCamera_ = new DebugCamera(1280, 720);

	player_ = new Player();
	player_->Initialize(model_, textureHandle_, worldTransform_);

	enemyWorldTransform_.translation_.y = 1;
	enemyWorldTransform_.translation_.z = 20;
	enemy_ = new Enemy();
	enemy_->Initialize(enemyModel_, enemyTextureHandle_, enemyWorldTransform_);
	
}

void GameScene::Update() {
	player_->Update();
	player_->Rotate();
	enemy_->Update();
	debugCamera_->Update();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = 1;
	}
#endif //  _DEBUG

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection; 
	viewProjection_.TransferMatrix();

	} else {
	viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	player_->Draw(viewProjection_);
	enemy_->Draw(enemyViewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
