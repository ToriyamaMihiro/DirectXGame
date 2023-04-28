#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include <cassert>"
#include "WorldTransform.h"

class PlayerBullet {
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection);
	;

private:
	uint32_t textureHandle_ = 0u;
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
};
