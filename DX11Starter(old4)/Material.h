#pragma once

#include "SimpleShader.h"

class Material
{
public:
	Material(SimplePixelShader &pxs, SimpleVertexShader &vxs);
	~Material();

	SimplePixelShader *pixelShader;
	SimpleVertexShader *vertexShader;

	SimplePixelShader GetPixelShader();
	SimpleVertexShader GetVertexShader();

private:

};

