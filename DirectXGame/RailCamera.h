#pragma once
#include "Input.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WorldTransform worldTransform, Vector3 rotation);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};
