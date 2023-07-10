#include "Player.h"
#include "Affine.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReiticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 position) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	position_ = position;
	input_ = Input::GetInstance();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	textureReticle = TextureManager::Load("pointer.png");

	worldTransform_.translation_ = position_;
	worldTransform3DReticle_.Initialize();
	sprite2DReiticle_ =
	    Sprite::Create(textureReticle, Vector2(700, 700), Vector4(1, 1, 1, 1), Vector2(0.5f, 0.5f));
	sprite2DReiticle_->SetSize(Vector2(200, 200));
}

void Player::Update() {

	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	Vector3 move = {0, 0, 0}; // 移動ベクトル

	const float kCharactorSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharactorSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharactorSpeed;
	} else if (input_->PushKey(DIK_UP)) {
		move.y += kCharactorSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharactorSpeed;
	}

	// 左右上下移動
	MakeScaleMatrix(worldTransform_.scale_);

	MakeRatateXMatrix(worldTransform_.rotation_.x);
	MakeRatateYMatrix(worldTransform_.rotation_.y);
	MakeRatateZMatrix(worldTransform_.rotation_.z);
	MakeRatateXYZMatrix(worldTransform_.rotation_);

	MakeTranslateMatrix(worldTransform_.translation_);

	worldTransform_.matWorld_ = MakeAffinMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.UpdateMatrix();

	worldTransform_.translation_.x += move.x; // プレイヤーの座標 + 移動ベクトル
	worldTransform_.translation_.y += move.y;

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

	// 弾の発射
	Attack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	const float kDistancePlayerTo3DReticle = 20.0f;

	Vector3 offset = {0, 0, 1.0f};

	Vector3 worldPosition = GetWorldPosition();

	offset = TransformNormal(offset, worldTransform_.matWorld_);
	offset = Scaler(kDistancePlayerTo3DReticle, Normalize(offset));
	worldTransform3DReticle_.translation_ = Add(worldPosition, offset);

	worldTransform3DReticle_.UpdateMatrix();

	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle = Get3DWorldPosition();
	// ビューポート行列
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュー後y列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
	    Multiply(Multiply(viewProjection_.matView, viewProjection_.matProjection) , matViewport);
	positionReticle = Transform(positionReticle, matViewProjectionViewport);
	sprite2DReiticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
}

void Player::Rotate() {
	const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

Vector3 Player::Get3DWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];
	return worldPos;
}

void Player::OnCollision() {}

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転する
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		Vector3 world3DPos = Get3DWorldPosition();
		Vector3 worldPosition;

		// 弾を生成して初期化
		worldPosition = GetWorldPosition();

		velocity = Subtract(world3DPos, worldPosition);
		velocity = Scaler(kBulletSpeed, Normalize(velocity));

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldPosition, velocity);

		bullets_.push_back(newBullet);
	}
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}
void Player::DrawUI() { sprite2DReiticle_->Draw(); }