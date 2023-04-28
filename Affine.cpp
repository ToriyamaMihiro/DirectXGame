#include "Affine.h"
#include <assert.h>

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 MakeTranslateMatrix;
	// 3çs1óÒÇ…äiî[
	MakeTranslateMatrix.m[0][0] = 1;
	MakeTranslateMatrix.m[0][1] = 0;
	MakeTranslateMatrix.m[0][2] = 0;
	MakeTranslateMatrix.m[0][3] = 0;
	MakeTranslateMatrix.m[1][0] = 0;
	MakeTranslateMatrix.m[1][1] = 1;
	MakeTranslateMatrix.m[1][2] = 0;
	MakeTranslateMatrix.m[1][3] = 0;
	MakeTranslateMatrix.m[2][0] = 0;
	MakeTranslateMatrix.m[2][1] = 0;
	MakeTranslateMatrix.m[2][2] = 1;
	MakeTranslateMatrix.m[2][3] = 0;
	MakeTranslateMatrix.m[3][0] = translate.x;
	MakeTranslateMatrix.m[3][1] = translate.y;
	MakeTranslateMatrix.m[3][2] = translate.z;
	MakeTranslateMatrix.m[3][3] = 1;
	return MakeTranslateMatrix;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 MakeScaleMatrix;
	MakeScaleMatrix.m[0][0] = scale.x;
	MakeScaleMatrix.m[0][1] = 0;
	MakeScaleMatrix.m[0][2] = 0;
	MakeScaleMatrix.m[0][3] = 0;
	MakeScaleMatrix.m[1][0] = 0;
	MakeScaleMatrix.m[1][1] = scale.y;
	MakeScaleMatrix.m[1][2] = 0;
	MakeScaleMatrix.m[1][3] = 0;
	MakeScaleMatrix.m[2][0] = 0;
	MakeScaleMatrix.m[2][1] = 0;
	MakeScaleMatrix.m[2][2] = scale.z;
	MakeScaleMatrix.m[2][3] = 0;
	MakeScaleMatrix.m[3][0] = 0;
	MakeScaleMatrix.m[3][1] = 0;
	MakeScaleMatrix.m[3][2] = 0;
	MakeScaleMatrix.m[3][3] = 1;

	return MakeScaleMatrix;
}

Vector3 Transform(Vector3& vector, Matrix4x4 matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
	           1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
	           1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
	           1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
	          1.0f * matrix.m[3][3];
	assert(w != 0.0f); // wÇÕ0Ç…Ç»ÇÁÇ»Ç¢
	// 0Ç≈ÇÕÇ»Ç¢wÇÃílÇ≈äÑÇÈÇÃÇ≈0Ç≈ÇÕÇ»Ç¢
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

Matrix4x4 MakeRatateXMatrix(float radian) {
	Matrix4x4 MakeRatateXMatrix;
	MakeRatateXMatrix.m[0][0] = 1;
	MakeRatateXMatrix.m[0][1] = 0;
	MakeRatateXMatrix.m[0][2] = 0;
	MakeRatateXMatrix.m[0][3] = 0;
	MakeRatateXMatrix.m[1][0] = 0;
	MakeRatateXMatrix.m[1][1] = std::cos(radian);
	MakeRatateXMatrix.m[1][2] = std::sin(radian);
	MakeRatateXMatrix.m[1][3] = 0;
	MakeRatateXMatrix.m[2][0] = 0;
	MakeRatateXMatrix.m[2][1] = -std::sin(radian);
	MakeRatateXMatrix.m[2][2] = std::cos(radian);
	MakeRatateXMatrix.m[2][3] = 0;
	MakeRatateXMatrix.m[3][0] = 0;
	MakeRatateXMatrix.m[3][1] = 0;
	MakeRatateXMatrix.m[3][2] = 0;
	MakeRatateXMatrix.m[3][3] = 1;

	return MakeRatateXMatrix;
}

Matrix4x4 MakeRatateYMatrix(float radian) {
	Matrix4x4 MakeRatateYMatrix;
	MakeRatateYMatrix.m[0][0] = std::cos(radian);
	MakeRatateYMatrix.m[0][1] = 0;
	MakeRatateYMatrix.m[0][2] = -std::sin(radian);
	MakeRatateYMatrix.m[0][3] = 0;
	MakeRatateYMatrix.m[1][0] = 0;
	MakeRatateYMatrix.m[1][1] = 1;
	MakeRatateYMatrix.m[1][2] = 0;
	MakeRatateYMatrix.m[1][3] = 0;
	MakeRatateYMatrix.m[2][0] = std::sin(radian);
	MakeRatateYMatrix.m[2][1] = 0;
	MakeRatateYMatrix.m[2][2] = std::cos(radian);
	MakeRatateYMatrix.m[2][3] = 0;
	MakeRatateYMatrix.m[3][0] = 0;
	MakeRatateYMatrix.m[3][1] = 0;
	MakeRatateYMatrix.m[3][2] = 0;
	MakeRatateYMatrix.m[3][3] = 1;

	return MakeRatateYMatrix;
}

Matrix4x4 MakeRatateZMatrix(float radian) {
	Matrix4x4 MakeRatateZMatrix;
	MakeRatateZMatrix.m[0][0] = std::cos(radian);
	MakeRatateZMatrix.m[0][1] = std::sin(radian);
	MakeRatateZMatrix.m[0][2] = 0;
	MakeRatateZMatrix.m[0][3] = 0;
	MakeRatateZMatrix.m[1][0] = -std::sin(radian);
	MakeRatateZMatrix.m[1][1] = std::cos(radian);
	MakeRatateZMatrix.m[1][2] = 0;
	MakeRatateZMatrix.m[1][3] = 0;
	MakeRatateZMatrix.m[2][0] = 0;
	MakeRatateZMatrix.m[2][1] = 0;
	MakeRatateZMatrix.m[2][2] = 1;
	MakeRatateZMatrix.m[2][3] = 0;
	MakeRatateZMatrix.m[3][0] = 0;
	MakeRatateZMatrix.m[3][1] = 0;
	MakeRatateZMatrix.m[3][2] = 0;
	MakeRatateZMatrix.m[3][3] = 1;

	return MakeRatateZMatrix;
}

Matrix4x4 MakeRatateXYZMatrix(const Vector3& rotate) {

	Matrix4x4 rotateXYZMatrix = Multiply(
	    MakeRatateXMatrix(rotate.x),
	    Multiply(MakeRatateYMatrix(rotate.y), MakeRatateZMatrix(rotate.z)));
	return rotateXYZMatrix;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 Multiply;
	Multiply.m[0][0] =
	    (m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] +
	     m1.m[0][3] * m2.m[3][0]);
	Multiply.m[0][1] =
	    (m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] +
	     m1.m[0][3] * m2.m[3][1]);
	Multiply.m[0][2] =
	    (m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] +
	     m1.m[0][3] * m2.m[3][2]);
	Multiply.m[0][3] =
	    (m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] +
	     m1.m[0][3] * m2.m[3][3]);
	Multiply.m[1][0] =
	    (m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] +
	     m1.m[1][3] * m2.m[3][0]);
	Multiply.m[1][1] =
	    (m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] +
	     m1.m[1][3] * m2.m[3][1]);
	Multiply.m[1][2] =
	    (m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] +
	     m1.m[1][3] * m2.m[3][2]);
	Multiply.m[1][3] =
	    (m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] +
	     m1.m[1][3] * m2.m[3][3]);
	Multiply.m[2][0] =
	    (m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] +
	     m1.m[2][3] * m2.m[3][0]);
	Multiply.m[2][1] =
	    (m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] +
	     m1.m[2][3] * m2.m[3][1]);
	Multiply.m[2][2] =
	    (m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] +
	     m1.m[2][3] * m2.m[3][2]);
	Multiply.m[2][3] =
	    (m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] +
	     m1.m[2][3] * m2.m[3][3]);
	Multiply.m[3][0] =
	    (m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] +
	     m1.m[3][3] * m2.m[3][0]);
	Multiply.m[3][1] =
	    (m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] +
	     m1.m[3][3] * m2.m[3][1]);
	Multiply.m[3][2] =
	    (m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] +
	     m1.m[3][3] * m2.m[3][2]);
	Multiply.m[3][3] =
	    (m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] +
	     m1.m[3][3] * m2.m[3][3]);
	return Multiply;
}

Matrix4x4 MakeAffinMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 rotateXYZMatrix = Multiply(
	    MakeRatateXMatrix(rotate.x),
	    Multiply(MakeRatateYMatrix(rotate.y), MakeRatateZMatrix(rotate.z)));
	Matrix4x4 SRT =
	    Multiply(MakeScaleMatrix(scale), Multiply(rotateXYZMatrix, MakeTranslateMatrix(translate)));

	return SRT;
}