#pragma once

#include <iostream>
#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Renderer.h"
#include "GameEntity.h"
#include "Mesh.h"
#include "Camera.h"
#include "Material.h"

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y, float deltaTime);
	void OnMouseWheel(float wheelDelta,   int x, int y, float deltaTime);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateMatrices();
	void CreateBasicGeometry();

	GameEntity *entities;
	Camera *mainCam;
	Renderer *render;
	Material *materi;

	bool noClick;
	float distX;
	float distY;
	float newPosX;
	float newPosY;

	Mesh *obj1;
	Mesh *obj2;
	Mesh *obj3;

	// Buffers to hold actual geometry data
	//ID3D11Buffer* vertexBuffer;
	//ID3D11Buffer* indexBuffer;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
};

