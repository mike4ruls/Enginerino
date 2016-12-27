#pragma once

#include "SimpleShader.h"

class Material
{
public:
	Material(SimplePixelShader &pxs, SimpleVertexShader &vxs, DirectX::XMFLOAT4 sColor);
	~Material();

	SimplePixelShader *pixelShader;
	SimpleVertexShader *vertexShader;

	SimplePixelShader GetPixelShader();
	SimpleVertexShader GetVertexShader();

	DirectX::XMFLOAT4 surfaceColor;

private:

};

