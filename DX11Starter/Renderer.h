#pragma once

#include "GameEntity.h"
#include <d3d11.h>
#include <stdlib.h>
#include "SimpleShader.h"
#include "Lights.h"
#include "Camera.h"
#include "Tetris.h"

class Renderer
{
public:
	std::vector<GameEntity>* entities;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	DirectX::XMFLOAT4X4* instanceData;
	ID3D11Buffer* instanceWorldMatrixBuff;

	Tetris* TetrisGame;

	Renderer(std::vector<GameEntity> &en, SimpleVertexShader &vShader, SimplePixelShader &pShader, Tetris &Tet, ID3D11Device &dev);
	~Renderer();
	ID3D11Device* device;

	bool gotBoard;
	std::vector<GameEntity> board;
	std::vector<GameEntity> tBlocks;
	std::vector<GameEntity> pBlocks;

	void RenderUpdate(ID3D11DeviceContext* context, Camera cam, DirectionalLight light, DirectionalLight light2);
	void DrawInstanceObject(ID3D11DeviceContext* context, Mesh &obj, int numOfIn, std::vector<GameEntity> entity);
private:


};

