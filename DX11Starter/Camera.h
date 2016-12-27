#pragma once

#include <DirectXMath.h>
#include "DXCore.h"

class Camera
{
public:
	Camera();
	~Camera();

	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	DirectX::XMVECTOR camPos;
	DirectX::XMVECTOR camDir;
	DirectX::XMVECTOR up;
	float xRot;
	float yRot;

	DirectX::XMFLOAT4X4 GetView();
	DirectX::XMFLOAT4X4 GetProject();
	void SetView();
	void SetProject();

	void Update();
	void Switch();
	void CheckScrolling(float wheelDelta, int x, int y);
	void OnMouseMove(WPARAM buttonState, int x, int y);
	void UpdateNewPos(WPARAM buttonState, int x, int y);
	void CheckMoveUpdate(WPARAM buttonState, int x, int y);

	bool noClick;
	float distX;
	float distY;
	float newPosX;
	float newPosY;

	POINT prevMousePos;

private:

};

