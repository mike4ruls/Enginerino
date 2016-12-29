#pragma once

#include "SimpleShader.h"

class Material
{
public:
	Material(SimplePixelShader &pxs, SimpleVertexShader &vxs, DirectX::XMFLOAT4 sColor, ID3D11ShaderResourceView &svr, ID3D11SamplerState &sam);
	~Material();

	SimplePixelShader *pixelShader;
	SimpleVertexShader *vertexShader;

	ID3D11ShaderResourceView* SVR;
	ID3D11SamplerState* sample;

	SimplePixelShader GetPixelShader();
	SimpleVertexShader GetVertexShader();
	ID3D11ShaderResourceView* GetSVR();
	ID3D11SamplerState* GetSampler();

	DirectX::XMFLOAT4 surfaceColor;

private:

};

