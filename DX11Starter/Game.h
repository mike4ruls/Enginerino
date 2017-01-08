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
#include "TetrisBlock.h"
#include "Tetris.h"
#include "ConsoleBuddy.h"
#include "Terrain.h"

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
	void LoadSkyBox();

	bool noClick;
	float distX;
	float distY;
	float newPosX;
	float newPosY;

	int skyBox;

	bool currentState;
	bool previousState;

	std::vector<GameEntity> entities;
	Camera *mainCam = nullptr;
	Renderer *render = nullptr;

	ConsoleBuddy* cB = nullptr;

	Material *defaultMat = nullptr;
	Material *redMat = nullptr;
	Material *greenMat = nullptr;
	Material *blueMat = nullptr;
	Material *purpleMat = nullptr;
	Material *lightBlueMat = nullptr;
	Material *yellowMat = nullptr;
	Material *greyMat = nullptr;

	TetrisBlock* block = nullptr;
	Tetris* tetrisGame = nullptr;

	Terrain* ter = nullptr;

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
	Mesh *game = nullptr;
	Mesh *over = nullptr;

	// Buffers to hold actual geometry data
	//ID3D11Buffer* vertexBuffer;
	//ID3D11Buffer* indexBuffer;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader = nullptr;
	SimplePixelShader* pixelShader = nullptr;

	SimpleVertexShader* skyVS = nullptr;
	SimplePixelShader* skyPS = nullptr;

	SimpleVertexShader* instanceVS = nullptr;
	SimplePixelShader* instancePS = nullptr;

	SimpleVertexShader* waterVS = nullptr;
	SimplePixelShader* waterPS = nullptr;

	ID3D11ShaderResourceView* SVR = nullptr;
	ID3D11ShaderResourceView* skyBoxSVR = nullptr;
	ID3D11SamplerState* sample = nullptr;

	ID3D11RasterizerState* skyRast;
	ID3D11DepthStencilState* skyDepth;

	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;


	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
};

