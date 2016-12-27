#pragma once

#include <iostream>
#include <vector>
#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Renderer.h"
#include "GameEntity.h"
#include "Mesh.h"
#include "Camera.h"
#include "Material.h"
#include "Lights.h"
#include "WICTextureLoader.h"
#include "TetrisBlock.h"

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

	bool noClick;
	float distX;
	float distY;
	float newPosX;
	float newPosY;

	bool currentState;
	bool previousState;

	std::vector<GameEntity> entities;
	Camera *mainCam = nullptr;
	Renderer *render = nullptr;

	Material *defaultMat = nullptr;
	Material *redMat = nullptr;
	Material *greenMat = nullptr;
	Material *blueMat = nullptr;
	Material *purpleMat = nullptr;

	TetrisBlock* block = nullptr;

	DirectionalLight light1;
	DirectionalLight light2;

	Mesh *obj1 = nullptr;
	Mesh *obj2 = nullptr;
	Mesh *obj3 = nullptr;
	Mesh *cone = nullptr;
	Mesh *cube = nullptr;
	Mesh *cylinder = nullptr;
	Mesh *helix = nullptr;
	Mesh *sphere = nullptr;
	Mesh *torus = nullptr;

	// Buffers to hold actual geometry data
	//ID3D11Buffer* vertexBuffer;
	//ID3D11Buffer* indexBuffer;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader = nullptr;
	SimplePixelShader* pixelShader = nullptr;

	SimpleSRV* SVR = nullptr;

	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;


	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
};

