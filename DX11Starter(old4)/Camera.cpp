#include "Camera.h"

using namespace DirectX;

Camera::Camera(int width, int height)
{
	camPos = XMVectorSet(0.0f, 0.0f, -8.0f, 0.0f);
	camDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	noClick = true;
	newPosX = 0.0f;
	newPosY = 0.0f;

	newRotX = 0.0f;
	newRotY = 0.0f;
	
	xRot = 0.0f;
	yRot = 0.0f;

	speed = 2.0f;

	SetView();
	SetProject(width, height);

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
	XMMATRIX V = XMMatrixLookToLH(
		camPos,     // The position of the "camera"
		camDir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
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
	
	XMMATRIX rotx = XMMatrixRotationX(yRot + newRotY);
	XMMATRIX roty = XMMatrixRotationY((xRot*-1.0f) + newRotX*-1.0f);

	XMMATRIX rotat = roty * rotx;

	XMVECTOR foward = { 0,0,1 };
	//camDir = foward * XMQuaternionRotationRollPitchYaw(xRot + newRotX, yRot + newRotY, 0);
	camDir = XMVector3Transform(foward, rotat);
	
	SetView();
}




void Camera::ResetCamera()
{
	camPos = XMVectorSet(0.0f, 0.0f, -8.0f, 0.0f);
	camDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	newPosX = 0.0f;
	newPosY = 0.0f;

	newRotX = 0.0f;
	newRotY = 0.0f;

	xRot = 0.0f;
	yRot = 0.0f;

	SetView();
}




void Camera::Update(float deltaTime)
{
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
		camPos += camDir* speed * deltaTime;
		SetView();
	}
	//Bacwards
	if (GetAsyncKeyState('S'))
	{
		camPos += (camDir * -speed) * deltaTime;
		SetView();
	}
	//Strafe left
	if (GetAsyncKeyState('A'))
	{
		camPos += XMVector3Cross(camDir, up)* speed * deltaTime;
		SetView();
	}
	//Strafe Right
	if (GetAsyncKeyState('D'))
	{
		camPos += (XMVector3Cross(camDir, up)*-speed) * deltaTime;
		SetView();
	}
	//Move down the y axis
	if (GetAsyncKeyState('X'))
	{
		camPos += (up * -speed) * deltaTime;
		SetView();
	}
	//Move up the y axis
	if (GetAsyncKeyState(VK_SPACE))
	{
		camPos += up* speed * deltaTime;
		SetView();
	}


	//Roll = z axis i think
	//Picth = x axis i think
	//Yaw = y axis i think
	//camDir = XMQuaternionRotationRollPitchYaw(,,);
	
}
void Camera::IsClicking(WPARAM buttonState, int x, int y)
{


	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	if (noClick) {
		prevMousePos.x = x;
		prevMousePos.y = y;
		Switch();
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
	Switch();
	if (buttonState & 0x0001)
	{
		
	}
	else
	{
		newPosX += distX;
		newPosY += distY;
	}
	newRotX += distX;
	newRotY += distY;
	
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Camera::OnMouseMove(WPARAM buttonState, int x, int y, float deltaTime)
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

		if(buttonState & 0x0001)
		{
			xRot = distX;
			yRot = distY;
			SetRotation();
		}
		else if(buttonState & 0x0010)
		{
			if(distX < 0)
			{
				camPos += (XMVector3Cross(camDir, up)*-distX*10) * deltaTime;
				SetView();
			}
			if (distX > 0)
			{
				camPos += XMVector3Cross(camDir, up) * -distX*10 * deltaTime;
				SetView();
			}
			if (distY < 0)
			{
				camPos += (up *distY*10) * deltaTime;
				SetView();
			}
			if (distY > 0)
			{
				camPos += (up * distY*10) * deltaTime;
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
	if (wheelDelta <0)
	{
		camPos += camDir * -80 * deltaTime;
		SetView();
	}
	else if (wheelDelta >0)
	{
		camPos += camDir * 80 * deltaTime;
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
