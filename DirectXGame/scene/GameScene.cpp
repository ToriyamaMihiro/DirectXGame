#include "GameScene.h"
#include "Affine.h"
#include "TextureManager.h"
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete skydome_;
	delete debugCamera_;
	delete railCamera_;

	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("sora_kh.png");
	model_ = Model::Create();
	enemyModel_ = Model::Create();
	skydomeModel_ = Model::CreateFromOBJ("tenkyu", true);
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	debugCamera_ = new DebugCamera(1280, 720);

	railCamera_ = new RailCamera;
	railCamera_->Initialize(worldTransform_, worldTransform_.rotation_);

	player_ = new Player();
	Vector3 playerPosition(0, 0, 20);
	
	player_->Initialize(model_, textureHandle_, playerPosition);
	player_->SetParent(&railCamera_->GetWorldTransform());

	skydome_ = new Skydome();
	skydome_->Initialize(skydomeModel_, worldTransform_);

	LoadEnemyPopDate();
}

void GameScene::Update() {

	player_->Update();
	player_->Rotate();
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});
	ApproachUpdate();
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	skydome_->Update();
	debugCamera_->Update();
	railCamera_->Update();

	CheckAllCollisions();
	viewProjection_.UpdateMatrix();

	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

	viewProjection_.TransferMatrix();
	UpdateEnemyPopCommands();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_Z)) {
		isDebugCameraActive_ = 1;
	}
#endif //  _DEBUG

	if (isDebugCameraActive_) {
		debugCamera_->Update();

		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
}

void GameScene::SetEnemy(const Vector3 position) {

	Enemy* enemy = new Enemy();
	velocity_ = Vector3(0, 0, 0);
	enemy->Initialize(enemyModel_, position, velocity_);
	enemys_.push_back(enemy);

	ApproachInitialize();
	enemy->SetPlayer(player_);
	enemy->SetGameScene(this);
}

void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;
	Vector3 posC, posD;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = GetBullets();
	// 敵リストの取得
	const std::list<Enemy*>& enemys = EnemyGetBullets();

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
	// 敵とプレイヤーの弾の当たり判定
	for (Enemy* enemy : enemys) {
		for (PlayerBullet* bullet : playerBullets) {
			posC = enemy->GetWorldPosition();
			posD = bullet->GetWorldPosition();
			int radiusC = 2;
			int radiusD = 2;

			Vector3 CtD = {posD.x - posC.x, posD.y - posC.y, posD.z - posC.z};

			if (CtD.x * CtD.x + CtD.y * CtD.y + CtD.z * CtD.z <=
			    (radiusC + radiusD) * (radiusC + radiusD)) {
				enemy->OnCollision();
				bullet->OnCollision();
			}
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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { bullets_.push_back(enemyBullet); }

void GameScene::Fire() {
	assert(player_);

	const float kBulletSpeed = 1.0f;
	for (Enemy* enemy : enemys_) {
		Vector3 player_worldPos = player_->GetWorldPosition();
		Vector3 enemy_worldPos = enemy->GetWorldPosition();

		Vector3 subtraction = {
		    player_worldPos.x - enemy_worldPos.x, player_worldPos.y - enemy_worldPos.y,
		    player_worldPos.z - enemy_worldPos.z};

		// 正規化
		float length = sqrt(
		    subtraction.x * subtraction.x + subtraction.y * subtraction.y +
		    subtraction.z * subtraction.z);

		Vector3 newSubtraction = {subtraction.x, subtraction.y, subtraction.z};

		if (length != 0.0f) {

			newSubtraction = {
			    subtraction.x / length, subtraction.y / length, subtraction.z / length};
		}

		newSubtraction.x *= kBulletSpeed;
		newSubtraction.y *= kBulletSpeed;
		newSubtraction.z *= kBulletSpeed;

		Vector3 velocity(newSubtraction);

		// 速度ベクトルを自機の向きに合わせて回転する
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成して初期化
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, enemy_worldPos, velocity);

		bullets_.push_back(newBullet);
	}
}

void GameScene::ApproachInitialize() { fire_timer = kFireInterval; }

void GameScene::ApproachUpdate() {
	fire_timer--;
	if (fire_timer == 0) {
		Fire();
		fire_timer = kFireInterval;
	}
}

void GameScene::LoadEnemyPopDate() {
	std::ifstream file;
	file.open("Resources/enemyPos.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	if (waitFlg) {
		waitCount--;
		if (waitCount <= 0) {
			waitFlg = false;
		}
		return;
	}

	std::string line;
	while (getline(enemyPopCommands, line)) {
		std::istringstream line_stream(line);

		std::string word;
		getline(line_stream, word, ',');
		if (word.find("//") == 0) {
			continue;
		}
		if (word.find("POP") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			SetEnemy(Vector3(x, y, z));

		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());
			waitFlg = true;
			waitCount = waitTime;
			break;
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

	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	for (EnemyBullet* bullet : bullets_) {

		bullet->Draw(viewProjection_);
	}

	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	player_->DrawUI();

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
