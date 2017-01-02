#include "Camera.h"

using namespace DirectX;

Camera::Camera(int width, int height)
{
	camPos = XMFLOAT4(0.0f, 0.0f, -8.0f, 0.0f);
	camDir = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
	up = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

	noClick = true;
	newPosX = 0.0f;
	newPosY = 0.0f;

	newRotX = 0.0;
	newRotY = 0.0;
	
	xRot = 0.0f;
	yRot = 0.0f;

	distX = 0.0f;
	distY = 0.0f;

	speed = 2.0f;

	SetView();
	SetRotation();
	SetProject(width, height);
	midClick = false;

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
	XMVECTOR camP = XMLoadFloat4(&camPos);
	XMVECTOR camD = XMLoadFloat4(&camDir);
	XMVECTOR upp = XMLoadFloat4(&up);
	XMMATRIX V = XMMatrixLookToLH(
		camP,     // The position of the "camera"
		camD,     // Direction the camera is looking
		upp);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!
}
void Camera::SetProject(int width, int height)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!*/
}
void Camera::SetRotation()
{
	float y = yRot + newRotY;
	float x = xRot + newRotX;

	y= max(min(y, XM_PIDIV2), -XM_PIDIV2);

	//Roll = z axis i think
	//Picth = x axis i think
	//Yaw = y axis i think
	XMStoreFloat4(&rotation, XMQuaternionRotationRollPitchYaw(y, -x, 0));
	XMVECTOR dir = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), XMLoadFloat4(&rotation));
	XMStoreFloat4(&camDir, dir);

	SetView();
}




void Camera::ResetCamera()
{
	camPos = XMFLOAT4(0.0f, 0.0f, -8.0f, 0.0f);
	camDir = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
	up = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

	newPosX = 0.0f;
	newPosY = 0.0f;

	newRotX = 0.0f;
	newRotY = 0.0f;

	xRot = 0.0f;
	yRot = 0.0f;

	distX = 0.0f;
	distY = 0.0f;

	SetView();
	SetRotation();
}
void Camera::SetTetrisCamera()
{
	camPos = XMFLOAT4(6.5f, 16.0f, -40.0f, 0.0f);
	camDir = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
	up = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

	newPosX = 0.0f;
	newPosY = 0.0f;

	newRotX = 0.0f;
	newRotY = 0.0f;

	xRot = 0.0f;
	yRot = 0.0f;

	distX = 0.0f;
	distY = 0.0f;

	SetView();
	SetRotation();
}




void Camera::Update(float deltaTime)
{
	XMVECTOR camP = XMLoadFloat4(&camPos);
	XMVECTOR camD = XMLoadFloat4(&camDir);
	XMVECTOR upp = XMLoadFloat4(&up);

	if (GetAsyncKeyState(VK_LCONTROL) && GetAsyncKeyState('C'))
	{
		ResetCamera();
	}

	if(GetAsyncKeyState(VK_LSHIFT))
	{
		speed = 5.0;
	}
	else
	{
		speed = 2.0f;
	}

	//Foward
	if (GetAsyncKeyState('W'))
	{
		camP += camD* speed * deltaTime;
		XMStoreFloat4(&camPos, camP);
		SetView();
	}
	//Bacwards
	if (GetAsyncKeyState('S'))
	{
		camP += (camD * -speed) * deltaTime;
		XMStoreFloat4(&camPos, camP);
		SetView();
	}
	//Strafe left
	if (GetAsyncKeyState('A'))
	{
		camP += XMVector3Cross(camD, upp)* speed * deltaTime;
		XMStoreFloat4(&camPos, camP);
		SetView();
	}
	//Strafe Right
	if (GetAsyncKeyState('D'))
	{
		camP += (XMVector3Cross(camD, upp)*-speed) * deltaTime;
		XMStoreFloat4(&camPos, camP);
		SetView();
	}
	//Move down the y axis
	if (GetAsyncKeyState('X'))
	{
		camP += (upp * -speed) * deltaTime;
		XMStoreFloat4(&camPos, camP);
		SetView();
	}
	//Move up the y axis
	if (GetAsyncKeyState(VK_SPACE))
	{
		camP += upp* speed * deltaTime;
		XMStoreFloat4(&camPos, camP);
		SetView();
	}


	
	
}
void Camera::IsClicking(WPARAM buttonState, int x, int y)
{

	// Save the previous mouse position, so we have it for the future
	if (noClick) {
		prevMousePos.x = x;
		prevMousePos.y = y;
		Switch();
	}
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Camera::UpdateNewPos(WPARAM buttonState, int x, int y)
{
	Switch();
	if (midClick)
	{
		midClick = false;
	}
	else
	{
		newPosX += distX;
		newPosY += distY;

		newRotX += distX;
		newRotY += distY;
	}
	
	//printf("New X dist:%f, New Y dist:%f\n", newRotX, newRotY);
	
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Camera::OnMouseMove(WPARAM buttonState, int x, int y, float deltaTime)
{
	XMVECTOR camP = XMLoadFloat4(&camPos);
	XMVECTOR camD = XMLoadFloat4(&camDir);
	XMVECTOR upp = XMLoadFloat4(&up);

	// Save the previous mouse position, so we have it for the future
	if (noClick) {
		prevMousePos.x = x;
		prevMousePos.y = y;
	}
	else
	{
		distX = (x - prevMousePos.x)*-0.01f;
		distY = (y - prevMousePos.y)*0.01f;

		//printf("X dist:%f, Y dist:%f\n", distX, distY);

		if(buttonState & 0x0001)
		{
			xRot = distX;
			yRot = distY;
			SetRotation();
		}
		else if(buttonState & 0x0010)
		{
			midClick = true;
			if(distX < 0)
			{
				camP += (XMVector3Cross(camD, upp)*-distX*10) * deltaTime;
				XMStoreFloat4(&camPos, camP);
				SetView();
			}
			if (distX > 0)
			{
				camP += XMVector3Cross(camD, upp) * -distX*10 * deltaTime;
				XMStoreFloat4(&camPos, camP);
				SetView();
			}
			if (distY < 0)
			{
				camP += (upp *distY*10) * deltaTime;
				XMStoreFloat4(&camPos, camP);
				SetView();
			}
			if (distY > 0)
			{
				camP += (upp * distY*10) * deltaTime;
				XMStoreFloat4(&camPos, camP);
				SetView();
			}
			
		}
	}
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Camera::CheckScrolling(float wheelDelta, int x, int y, float deltaTime)
{
	XMVECTOR camP = XMLoadFloat4(&camPos);
	XMVECTOR camD = XMLoadFloat4(&camDir);
	XMVECTOR upp = XMLoadFloat4(&up);
	if (wheelDelta <0)
	{
		camP += camD * -80 * deltaTime;
		XMStoreFloat4(&camPos, camP);
		SetView();
	}
	else if (wheelDelta >0)
	{
		camP += camD * 80 * deltaTime;
		XMStoreFloat4(&camPos, camP);
		SetView();
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
