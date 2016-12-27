#include "Material.h"

using namespace DirectX;

Material::Material(SimplePixelShader &pxs, SimpleVertexShader &vxs, XMFLOAT4 sColor)
{
	pixelShader = &pxs;
	vertexShader = &vxs;
	surfaceColor = sColor;
}


Material::~Material()
{
	//if (pixelShader) { delete pixelShader; }
	//if (vertexShader) { delete vertexShader; }
}
SimplePixelShader Material::GetPixelShader()
{
	return *pixelShader;
}
SimpleVertexShader Material::GetVertexShader()
{
	return *vertexShader;
}
