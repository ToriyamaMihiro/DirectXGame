#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include <cassert>"
#include "WorldTransform.h"

class PlayerBullet {
private:
	uint32_t textureHandle_ = 0u;
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	Vector3 velocity_;
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
	
public:
	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// –ˆƒtƒŒ[ƒ€ˆ—
	/// </summary>
	void Update();

	bool IsDead() const { return isDead_; }

	void OnCollision();

	Vector3 GetWorldPosition();
	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw(const ViewProjection& viewProjection);


};
