#include "Material.h"

using namespace DirectX;

Material::Material(SimplePixelShader &pxs, SimpleVertexShader &vxs, XMFLOAT4 sColor, ID3D11ShaderResourceView &svr, ID3D11SamplerState &sam)
{
	pixelShader = &pxs;
	vertexShader = &vxs;
	surfaceColor = sColor;
	SVR = &svr;
	sample = &sam;
}


Material::~Material()
{
}
SimplePixelShader Material::GetPixelShader()
{
	return *pixelShader;
}
SimpleVertexShader Material::GetVertexShader()
{
	return *vertexShader;
}
ID3D11ShaderResourceView* Material::GetSVR() 
{
	return SVR;
}
ID3D11SamplerState* Material::GetSampler()
{
	return sample;
}
