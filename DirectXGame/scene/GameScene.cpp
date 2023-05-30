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
	enemy_->SetPlayer(player_);
}

void GameScene::Update() {
	player_->Update();
	player_->Rotate();
	enemy_->Update();
	debugCamera_->Update();
	CheckAllCollisions();
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

void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;
	Vector3 posC, posD;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	// 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();
	for (EnemyBullet* bullet : enemyBullets) {
		posB = bullet->GetWorldPosition();
		int radiusA = 2;
		int radiusB = 2;

		Vector3 AtB = {posB.x - posA.x, posB.y - posA.y, posB.z - posA.z};

		if (AtB.x * AtB.x + AtB.y * AtB.y + AtB.z * AtB.z <=
		    (radiusA + radiusB) * (radiusA + radiusB)) {
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
	posC = enemy_->GetWorldPosition();
	for (PlayerBullet* bullet : playerBullets) {
		posD = bullet->GetWorldPosition();
		int radiusC = 2;
		int radiusD = 2;

		Vector3 CtD = {posD.x - posC.x, posD.y - posC.y, posD.z - posC.z};

		if (CtD.x * CtD.x + CtD.y * CtD.y + CtD.z * CtD.z <=
		    (radiusC + radiusD) * (radiusC + radiusD)) {
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}
	for (EnemyBullet* enemy_bullet : enemyBullets) {
		for (PlayerBullet* player_bullet : playerBullets) {
			posB = enemy_bullet->GetWorldPosition();
			posD = player_bullet->GetWorldPosition();
			int radiusB = 2;
			int radiusD = 2;
			Vector3 BtD = {posD.x - posB.x, posD.y - posB.y, posD.z - posB.z};
			if (BtD.x * BtD.x + BtD.y * BtD.y + BtD.z * BtD.z <=
			    (radiusB + radiusD) * (radiusB + radiusD)) {
				enemy_bullet->OnCollision();
				player_bullet->OnCollision();
			}

		}
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
