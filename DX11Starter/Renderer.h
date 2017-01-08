#pragma once

#include "GameEntity.h"
#include <d3d11.h>
#include <stdlib.h>
#include "SimpleShader.h"
#include "Lights.h"
#include "Camera.h"
#include "Tetris.h"
#include "InstanceStuff.h"

class Renderer
{
public:
	std::vector<GameEntity>* entities;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	SimpleVertexShader* instanceVS;
	SimplePixelShader* instancePS;

	SimpleVertexShader* waterVS;
	SimplePixelShader* waterPS;

	InstanceStuff* instanceStuff = nullptr;
	ID3D11Buffer* instanceStuffBuff = nullptr;

	Tetris* TetrisGame;

	Renderer(std::vector<GameEntity> &en, SimpleVertexShader &vShader, SimplePixelShader &pShader, SimpleVertexShader &iv, SimplePixelShader &ip, SimpleVertexShader &wv, SimplePixelShader &wp, Tetris &Tet, ID3D11Device &dev);
	~Renderer();
	void LoadAllBlocks();
	ID3D11Device* device;

	int turnOn;

	bool gotBoard;
	bool freeRoam;
	std::vector<GameEntity> *board;
	std::vector<GameEntity> *scoreBlocks;
	std::vector<GameEntity> *otherBlocks;
	std::vector<GameEntity> tBlocks;
	std::vector<GameEntity> pBlocks;

	std::vector<GameEntity> allBlocks;

	void RenderUpdate(ID3D11DeviceContext* context, Camera cam, DirectionalLight light, DirectionalLight light2);
	void DrawInstanceObject(ID3D11DeviceContext* context, Camera cam, DirectionalLight light, DirectionalLight light2, int numOfIn, std::vector<GameEntity> entity);
private:


};

