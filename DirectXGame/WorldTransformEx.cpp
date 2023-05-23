#include "WorldTransform.h"
#include "Affine.h"

void WorldTransform::UpdateMatrix() {
	matWorld_ = MakeAffinMatrix(scale_, rotation_, translation_);
	TransferMatrix();
}