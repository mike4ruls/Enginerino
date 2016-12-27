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
	Tetris* tetrisG;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	Renderer(std::vector<GameEntity> &en, Tetris &tG, SimpleVertexShader &vShader, SimplePixelShader &pShader);
	~Renderer();

	void RenderUpdate(ID3D11DeviceContext* context, Camera cam, DirectionalLight light, DirectionalLight light2);
private:
	std::vector<GameEntity> board;
	std::vector<GameEntity> tBlocks;

};

