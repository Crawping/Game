//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

Camera::Camera()
{
	CalculateViewMatrix(Vec(0,0,0), 0, 0, 0);
	CalculatePerspectiveProjectionMatrix();
}

//////////////////////////////////////////////////////////////////////

Matrix Camera::ViewMatrix(Vec4 target, Vec4 position, Vec4 up)
{
	Vec4 zaxis = Normalize(target - position);
	Vec4 xaxis = Normalize(Cross(up, zaxis));
	Vec4 yaxis = Cross(zaxis, xaxis);
	Vec4 trans = Vec(-Dot(xaxis, position), -Dot(yaxis, position), -Dot(zaxis, position), 1);
	return Matrix(	GetX3(xaxis, yaxis, zaxis),
					GetY3(xaxis, yaxis, zaxis),
					GetZ3(xaxis, yaxis, zaxis),
					trans);
}

//////////////////////////////////////////////////////////////////////

Matrix Camera::ViewMatrix(Vec4 position, float yaw, float pitch, float roll)
{
	Matrix m = DirectX::XMMatrixIdentity();
	m.r[3] = SetW(Negate(position), 1.0f);
	m *= DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	return m;
}

//////////////////////////////////////////////////////////////////////

Matrix Camera::PerspectiveProjection(float fov /* = 0.5f */, float aspectRatio /* = 4.0f / 3.0f */, float nearZ /* = 1.0f */, float farZ /* = 1000.0f */ )
{
	float sinFov = sinf(fov * 0.5f);
	float cosFov = cosf(fov * 0.5f);
	float high = cosFov / sinFov;
	float wide = high / aspectRatio;
	float zDiff = farZ - nearZ;
	float zF = farZ / zDiff;
	float zN = -zF * nearZ;
	return Matrix(	wide,	0,		0,		0,
					0,		high,	0,		0,
					0,		0,		zF,		1,
					0,		0,		zN,		0);
}
//////////////////////////////////////////////////////////////////////

Matrix Camera::OrthoProjection3D(float width /* = 40 */, float height /* = 30 */, float nearZ /* = 1.0f */, float farZ /* = 1000.0f */ )
{
	float high = 2/height;
	float wide = 2/width;
	float zDiff = farZ - nearZ;
	float zF = 1/zDiff;
	float zN = -nearZ/zDiff;
	return Matrix(	wide,	0,		0,		0,
					0,		high,	0,		0,
					0,		0,		zF,		0,
					0,		0,		zN,		1);
}

//////////////////////////////////////////////////////////////////////

Matrix Camera::OrthoProjection2D(int width, int height)
{
	float hw = 2.0f / width;
	float hh = -2.0f / height;
	return Matrix(	hw,		0,		0,		0,
					0,		hh,		0,		0,
					0,		0,		1,		0,
					-1,		1,		0,		1);
}

//////////////////////////////////////////////////////////////////////

void Camera::CalculateViewMatrix(Vec4 target, Vec4 position, Vec4 up)
{
	mViewMatrix = ViewMatrix(target, position, up);
}

//////////////////////////////////////////////////////////////////////

void Camera::CalculateViewMatrix(Vec4 position, float yaw, float pitch, float roll)
{
	mViewMatrix = ViewMatrix(position, yaw, pitch, roll);
}

//////////////////////////////////////////////////////////////////////

void Camera::CalculatePerspectiveProjectionMatrix(float fov /* = 0.5f */, float aspectRatio /* = 4.0f / 3.0f */, float nearZ /* = 1.0f */, float farZ /* = 1000.0f */ )
{
	mProjectionMatrix = PerspectiveProjection(fov, aspectRatio, nearZ, farZ);
}
//////////////////////////////////////////////////////////////////////

void Camera::CalculateOrthoProjectionMatrix(float width /* = 40 */, float height /* = 30 */, float nearZ /* = 1.0f */, float farZ /* = 1000.0f */ )
{
	mProjectionMatrix = OrthoProjection3D(width, height, nearZ, farZ);
}

//////////////////////////////////////////////////////////////////////

void Camera::CalculateOrthoProjectionMatrix2D(int width, int height)
{
	mProjectionMatrix = OrthoProjection2D(width, height);
}

//////////////////////////////////////////////////////////////////////

void Camera::CalculateViewProjectionMatrix()
{
	mTransformMatrix = mViewMatrix * mProjectionMatrix;
}

//////////////////////////////////////////////////////////////////////

void Camera::CalculateViewProjectionMatrix(Matrix const &modelMatrix)
{
	mTransformMatrix = modelMatrix * mViewMatrix * mProjectionMatrix;
}

