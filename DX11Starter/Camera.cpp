#include "Camera.h"

using namespace DirectX;

Camera::Camera()
{
	camPos = XMVectorSet(0, 0, -8, 0);
	camDir = XMVectorSet(0, 0, 1, 0);
	up = XMVectorSet(0, 1, 0, 0);

	noClick = true;
	newPosX = 0.0;
	newPosY = 0.0;

	XMMATRIX V = XMMatrixLookToLH(
		camPos,     // The position of the "camera"
		camDir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!
	/*
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!*/
	//XMStoreFloat4(&camPos, pos);
}


Camera::~Camera()
{
}

XMFLOAT4X4 Camera::GetView()
{
	return viewMatrix;
}
XMFLOAT4X4 Camera::GetProject()
{
	return projectionMatrix;
}
void Camera::SetView()
{

}
void Camera::SetProject()
{

}

void Camera::Update()
{
	XMMATRIX rotx = XMMatrixRotationX(xRot);
	XMMATRIX roty = XMMatrixRotationY(yRot);

	XMMATRIX rotat = rotx*roty;

	XMVECTOR foward = {0,0,1};
	camDir = XMVector3Transform(foward, rotat);

	XMMatrixLookToLH(camPos, camDir, up);

	//Roll = z axis i think
	//Picth = x axis i think
	//Yaw = y axis i think
	//camDir = XMQuaternionRotationRollPitchYaw(,,);
	
}
void Camera::CheckMoveUpdate(WPARAM buttonState, int x, int y)
{


	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	if (noClick) {
		prevMousePos.x = x;
		prevMousePos.y = y;
		noClick = false;
	}

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	////SetCapture(DXCore::);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Camera::UpdateNewPos(WPARAM buttonState, int x, int y)
{
	newPosX += distX;
	newPosY += distY;
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Camera::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	if (noClick) {
		prevMousePos.x = x;
		prevMousePos.y = y;
	}
	else
	{
		distX = (x - prevMousePos.x)*-0.01f;
		distY = (y - prevMousePos.y)*0.01f;

		printf("X dist:%f, Y dist:%f\n", distX, distY);

		camPos = XMVectorSet((newPosX + distX), (newPosY + distY), viewMatrix._34 * -1, 0);
	}
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Camera::CheckScrolling(float wheelDelta, int x, int y)
{
	if (wheelDelta <0)
	{
		camPos = XMVectorSet(viewMatrix._14 * -1.0f, viewMatrix._24 * -1.0f, (viewMatrix._34 + 0.1f) * -1.0f, 0.0f);
	}
	else if (wheelDelta >0)
	{
		camPos = XMVectorSet(viewMatrix._14 * -1.0f, viewMatrix._24 * -1.0f, (viewMatrix._34 - 0.1f) * -1.0f, 0.0f);
	}
	else {}
}
void Camera::Switch()
{
	if (noClick) {
		noClick = false;
	}
	else {
		noClick = true;
	}
}
