#pragma once

#include "GameEntity.h"
#include <d3d11.h>
#include "SimpleShader.h"
#include "Lights.h"
#include "Camera.h"

class Renderer
{
public:
	std::vector<GameEntity>* entities;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	Renderer(std::vector<GameEntity> &en, SimpleVertexShader &vShader, SimplePixelShader &pShader);
	~Renderer();

	void RenderUpdate(ID3D11DeviceContext* context, Camera cam, DirectionalLight light, DirectionalLight light2);
private:

};

