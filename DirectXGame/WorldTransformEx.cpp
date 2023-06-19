#include "WorldTransform.h"
#include "Affine.h"

void WorldTransform::UpdateMatrix() {
	matWorld_ = MakeAffinMatrix(scale_, rotation_, translation_);
	//親があれば親のワールド行列を掛ける
	if (parent_) {
		matWorld_ = Multiply( parent_->matWorld_,matWorld_);
	}
	TransferMatrix();
}