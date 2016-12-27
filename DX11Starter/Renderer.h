#pragma once

#include "GameEntity.h"
#include <d3d11.h>
#include "SimpleShader.h"

class Renderer
{
public:
	GameEntity *entities;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	Renderer(GameEntity *en, SimpleVertexShader* vShader, SimplePixelShader* pShader);
	~Renderer();

	void RenderUpdate(ID3D11DeviceContext* context, DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);
private:

};

