#include "Enemy.h"
#include "Affine.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include"Player.h"

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, uint32_t textureHandle, WorldTransform worldTransform) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_ = worldTransform;
	input_ = Input::GetInstance();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	// Fire();
	ApproachInitialize();
}

void Enemy::Update() {

	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
	});

	// �ړ�
	Vector3 move = {0, 0, 0}; // �ړ��x�N�g��

	const float kCharactorSpeed = +0.1f;

	move.x += kCharactorSpeed;

	MakeScaleMatrix(worldTransform_.scale_);

	MakeRatateXMatrix(worldTransform_.rotation_.x);
	MakeRatateYMatrix(worldTransform_.rotation_.y);
	MakeRatateZMatrix(worldTransform_.rotation_.z);
	MakeRatateXYZMatrix(worldTransform_.rotation_);

	MakeTranslateMatrix(worldTransform_.translation_);

	worldTransform_.matWorld_ = MakeAffinMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.UpdateMatrix();

	worldTransform_.translation_.x += move.x; // �v���CworldTransform_.translation_.y += move.y;

	ImGui::Begin("Debug1");
	ImGui::Text("%f", fire_timer);

	ImGui::End();

	const float kMoveLimitX = 34;
	const float kMoveLimitY = 19;
	//// �ړ��͈͂̎w��
	// worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	// worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	// worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	// worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// �s���p�^�[��
	/*switch (phase_) {
	case Phase::Approach:
	    move.z += kCharactorSpeed;
	    worldTransform_.translation_.z += move.z;
	    if (worldTransform_.translation_.z < 0.0f) {
	        phase_ = Phase::Leave;
	    }
	    break;
	case Phase::Leave:
	    move.x += kCharactorSpeed;
	    move.y += kCharactorSpeed;
	    worldTransform_.translation_.x += move.x;
	    worldTransform_.translation_.y -= move.y;
	    break;
	default:
	    break;
	}*/
	ApproachUpdate();
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Enemy::Fire() {
	assert(player_);

	const float kBulletSpeed = 1.0f;

	Vector3 player_worldPos = player_->GetWorldPosition();
	Vector3 enemy_worldPos = GetWorldPosition();
	Vector3 subtraction = {
	    player_worldPos.x - enemy_worldPos.x, player_worldPos.y - enemy_worldPos.y,
	    player_worldPos.z - enemy_worldPos.z};

	//���K��
	float length = sqrt(
	    subtraction.x * subtraction.x + subtraction.y * subtraction.y +
	    subtraction.z * subtraction.z);

	Vector3 newSubtraction = {subtraction.x, subtraction.y, subtraction.z};

	if (length != 0.0f) {

		newSubtraction = {subtraction.x / length, subtraction.y / length, subtraction.z / length};
	}

	newSubtraction.x *= kBulletSpeed;
	newSubtraction.y *= kBulletSpeed;
	newSubtraction.z *= kBulletSpeed;

	Vector3 velocity(newSubtraction);

	// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]����
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// �e�𐶐����ď�����
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	bullets_.push_back(newBullet);
}

void Enemy::ApproachInitialize() { fire_timer = kFireInterval; }

void Enemy::ApproachUpdate() {
	fire_timer--;
	if (fire_timer == 0) {
		Fire();
		fire_timer = kFireInterval;
	}
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

void Enemy::OnCollision() {}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}