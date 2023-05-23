#include "Matrix4x4.h"
#include<assert.h>

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 MakeTranslateMatrix;
	// 3行1列に格納
	MakeTranslateMatrix.m[0][0] = 1;
	MakeTranslateMatrix.m[0][1] = 0;
	MakeTranslateMatrix.m[0][2] = 0;
	MakeTranslateMatrix.m[0][3] = translate.x;
	MakeTranslateMatrix.m[1][0] = 0;
	MakeTranslateMatrix.m[1][1] = 1;
	MakeTranslateMatrix.m[1][2] = 0;
	MakeTranslateMatrix.m[1][3] = translate.y;
	MakeTranslateMatrix.m[2][0] = 0;
	MakeTranslateMatrix.m[2][1] = 0;
	MakeTranslateMatrix.m[2][2] = 0;
	MakeTranslateMatrix.m[2][3] = translate.z;
	MakeTranslateMatrix.m[3][0] = 0;
	MakeTranslateMatrix.m[3][1] = 0;
	MakeTranslateMatrix.m[3][2] = 0;
	MakeTranslateMatrix.m[3][3] = 1;
	return MakeTranslateMatrix;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 MakeScaleMatrix;
	return MakeScaleMatrix; 
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) { 
	Vector3 Transform;
	Transform.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	Transform.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f); // wは0にならない
	// 0ではないwの値で割るので0ではない
	Transform.x /= w;
	Transform.y /= w;
	return Transform;
}
