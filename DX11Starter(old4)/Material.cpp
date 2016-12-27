#include "Material.h"



Material::Material(SimplePixelShader &pxs, SimpleVertexShader &vxs)
{
	pixelShader = &pxs;
	vertexShader = &vxs;
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
