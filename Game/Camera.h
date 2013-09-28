//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class Camera
{
public:
	
	//////////////////////////////////////////////////////////////////////

	Camera();

	//////////////////////////////////////////////////////////////////////

	Matrix const &GetTransformMatrix() const
	{
		return mTransformMatrix;
	}

	//////////////////////////////////////////////////////////////////////

	Matrix GetTransformMatrix(Matrix const &modelMatrix) const
	{
		return modelMatrix * mTransformMatrix;
	}

	//////////////////////////////////////////////////////////////////////

	Matrix const &GetProjectionMatrix() const
	{
		return mProjectionMatrix;
	}

	//////////////////////////////////////////////////////////////////////

	Matrix const &GetViewMatrix() const
	{
		return mViewMatrix;
	}

	//////////////////////////////////////////////////////////////////////

	void CalculateViewMatrix(Vector target, Vector position, Vector up);
	void CalculateViewMatrix(Vector position, float yaw, float pitch, float roll);
	void CalculatePerspectiveProjectionMatrix(float fov = 0.5f, float aspectRatio = 4.0f / 3.0f, float nearZ = 1.0f, float farZ = 1000.0f);
	void CalculateOrthoProjectionMatrix(float width = 40, float height = 30, float nearZ = 1.0f, float farZ = 1000.0f);
	void CalculateOrthoProjectionMatrix2D(int width, int height);
	void CalculateViewProjectionMatrix();
	void CalculateViewProjectionMatrix(Matrix const &modelMatrix);

	static Matrix ViewMatrix(Vector target, Vector position, Vector up);
	static Matrix ViewMatrix(Vector position, float yaw, float pitch, float roll);

	static Matrix PerspectiveProjection(float fov = 0.5f, float aspectRatio = 4.0f / 3.0f, float nearZ = 1.0f, float farZ = 1000.0f);
	static Matrix OrthoProjection3D(float width, float height, float nearZ = 1.0f, float farZ = 1000.0f);
	static Matrix OrthoProjection2D(int width, int height);

	//////////////////////////////////////////////////////////////////////

private:

	Matrix			mViewMatrix;
	Matrix			mProjectionMatrix;
	Matrix			mTransformMatrix;
};

//////////////////////////////////////////////////////////////////////

