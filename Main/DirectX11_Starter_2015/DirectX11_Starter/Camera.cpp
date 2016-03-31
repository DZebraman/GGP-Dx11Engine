#include "Camera.h"



Camera::Camera()
{
	pos = XMFLOAT3(0, 0, 0);
	fwd = XMFLOAT3(0, 0, 1);

	xRot = 0;
	yRot = 0;
}

void Camera::setPos(XMFLOAT3 _pos) {
	pos = _pos;
}

void Camera::update(float deltaTime) {
	XMMATRIX mRot = XMMatrixRotationRollPitchYaw(xRot, yRot, 0);
	XMVECTOR xmFwd = XMVector3Transform(XMLoadFloat3(&fwd), mRot);
	XMVECTOR xmPos = XMLoadFloat3(&pos);
	XMVECTOR xmUp = XMLoadFloat3(&XMFLOAT3(0, 1, 0));
	XMVECTOR xmRight = XMVector3Cross(xmUp, xmFwd);

	//calculates up so it is no longer a constant 
	//xmUp = XMVector3Cross(xmFwd, xmRight);

	if (GetAsyncKeyState('D') & 0x8000)		 { xmPos +=  xmRight  * 4.f * deltaTime; }
	if (GetAsyncKeyState('A') & 0x8000)		 { xmPos += -xmRight  * 4.f * deltaTime; }
	if (GetAsyncKeyState('W') & 0x8000)		 { xmPos +=  xmFwd    * 4.f * deltaTime; }
	if (GetAsyncKeyState('S') & 0x8000)		 { xmPos += -xmFwd    * 4.f * deltaTime; }
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) { xmPos +=  xmUp	  * 4.f * deltaTime; }
	if (GetAsyncKeyState('X') & 0x8000)		 { xmPos += -xmUp	  * 4.f * deltaTime; }

	XMStoreFloat3(&fwd, xmFwd);
	XMStoreFloat3(&pos, xmPos);
	XMStoreFloat4x4(&mViewMatrix,XMMatrixTranspose(XMMatrixLookToLH(xmPos, xmFwd, xmUp)));

	xRot = 0;
	yRot = 0;
}

void Camera::updateProjection(float aspectRatio) {
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		aspectRatio,				// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&mProjMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

void Camera::setRotation(float _xRot, float _yRot) {
	xRot = _xRot;
	yRot = _yRot;
}

XMFLOAT3 Camera::getPos() { return pos; }

XMFLOAT4X4* Camera::getViewMatrix() { return &mViewMatrix; }
XMFLOAT4X4* Camera::getProjMatrix() { return &mProjMatrix; }
Camera::~Camera()
{
}
