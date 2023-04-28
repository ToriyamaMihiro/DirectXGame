#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include <cassert>"
#include "WorldTransform.h"

class PlayerBullet {
public:
	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// –ˆƒtƒŒ[ƒ€ˆ—
	/// </summary>
	void Update();

	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw(const ViewProjection& viewProjection);
	;

private:
	uint32_t textureHandle_ = 0u;
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
};
