#pragma once

#include <d3d11.h>
#include "Vertex.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "DirectXGameCore.h"

using namespace DirectX;

class Camera
{
private:
	XMFLOAT3 pos, fwd;
	XMFLOAT4X4 mViewMatrix;
	XMFLOAT4X4 mProjMatrix;
	float xRot, yRot;
public:
	Camera();

	void setRotation(float _xRot, float _yRot);
	void update(float deltaTime);
	void updateProjection(float aspectRatio);
	void setPos(XMFLOAT3 _pos);

	XMFLOAT4X4 * getViewMatrix();
	XMFLOAT4X4 * getProjMatrix();

	~Camera();
};

