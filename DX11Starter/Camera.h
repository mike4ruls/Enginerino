#pragma once

#include <DirectXMath.h>
#include "DXCore.h"

class Camera
{
public:
	Camera(int width, int height);
	~Camera();

	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	DirectX::XMFLOAT4 camPos;
	DirectX::XMFLOAT4 camDir;
	DirectX::XMFLOAT4 up;
	DirectX::XMFLOAT4 rotation;
	float xRot;
	float yRot;

	DirectX::XMFLOAT4X4 GetView();
	DirectX::XMFLOAT4X4 GetProject();
	void SetView();
	void SetProject(int width, int height);
	void SetRotation();
	void ResetCamera();
	void SetTetrisCamera();

	void Update(float deltaTime);
	void Switch();
	void CheckScrolling(float wheelDelta, int x, int y, float deltaTime);
	void OnMouseMove(WPARAM buttonState, int x, int y, float deltaTime);
	void UpdateNewPos(WPARAM buttonState, int x, int y);
	void IsClicking(WPARAM buttonState, int x, int y);

	bool midClick;
	bool noClick;
	float distX;
	float distY;
	float newPosX;
	float newPosY;
	float newRotX;
	float newRotY;

	float speed;

	POINT prevMousePos;

private:

};

