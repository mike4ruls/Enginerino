#include "Renderer.h"


using namespace DirectX;
Renderer::Renderer(std::vector<GameEntity> &en, SimpleVertexShader &vShader, SimplePixelShader &pShader, Tetris &Tet, ID3D11Device &dev)
{
	entities = &en;
	vertexShader = &vShader;
	pixelShader = &pShader;
	TetrisGame = &Tet;
	gotBoard = false;
	device = &dev;
}


Renderer::~Renderer()
{
}
void Renderer::RenderUpdate(ID3D11DeviceContext* context, Camera cam, DirectionalLight light, DirectionalLight light2)
{
	if (TetrisGame->gameStart == true) {
		
		for (int i = 0; i < (int)board.size(); i++)
		{
			vertexShader->SetMatrix4x4("world", board[i].GetWorldMatrix());
			vertexShader->SetMatrix4x4("view", cam.viewMatrix);
			vertexShader->SetMatrix4x4("projection", cam.projectionMatrix);
			vertexShader->SetData("camPos", &cam.camPos, sizeof(XMFLOAT4));

			pixelShader->SetData("light", &light, sizeof(DirectionalLight));
			pixelShader->SetData("light2", &light2, sizeof(DirectionalLight));
			pixelShader->SetData("surfaceColor", &board[i].mat->surfaceColor, sizeof(XMFLOAT4));

			vertexShader->CopyAllBufferData();
			pixelShader->CopyAllBufferData();

			vertexShader->SetShader();
			pixelShader->SetShader();

			board[i].Draw(context);
		}
		for (int i = 0; i < (int)tBlocks.size(); i++)
		{
			vertexShader->SetMatrix4x4("world", tBlocks[i].GetWorldMatrix());
			vertexShader->SetMatrix4x4("view", cam.viewMatrix);
			vertexShader->SetMatrix4x4("projection", cam.projectionMatrix);
			vertexShader->SetData("camPos", &cam.camPos, sizeof(XMFLOAT4));

			pixelShader->SetData("light", &light, sizeof(DirectionalLight));
			pixelShader->SetData("light2", &light2, sizeof(DirectionalLight));
			pixelShader->SetData("surfaceColor", &tBlocks[i].mat->surfaceColor, sizeof(XMFLOAT4));

			vertexShader->CopyAllBufferData();
			pixelShader->CopyAllBufferData();

			vertexShader->SetShader();
			pixelShader->SetShader();

			tBlocks[i].Draw(context);
		}
		for (int i = 0; i < (int)pBlocks.size(); i++)
		{
			vertexShader->SetMatrix4x4("world", pBlocks[i].GetWorldMatrix());
			vertexShader->SetMatrix4x4("view", cam.viewMatrix);
			vertexShader->SetMatrix4x4("projection", cam.projectionMatrix);
			vertexShader->SetData("camPos", &cam.camPos, sizeof(XMFLOAT4));

			pixelShader->SetData("light", &light, sizeof(DirectionalLight));
			pixelShader->SetData("light2", &light2, sizeof(DirectionalLight));
			pixelShader->SetData("surfaceColor", &pBlocks[i].mat->surfaceColor, sizeof(XMFLOAT4));

			vertexShader->CopyAllBufferData();
			pixelShader->CopyAllBufferData();

			vertexShader->SetShader();
			pixelShader->SetShader();

			pBlocks[i].Draw(context);
		}
	}
	else
	{
		for (int i = 0; i < (int)(*entities).size(); i++)
		{
			vertexShader->SetMatrix4x4("world", (*entities)[i].GetWorldMatrix());
			vertexShader->SetMatrix4x4("view", cam.viewMatrix);
			vertexShader->SetMatrix4x4("projection", cam.projectionMatrix);
			vertexShader->SetData("camPos", &cam.camPos, sizeof(XMFLOAT4));

			pixelShader->SetData("light", &light, sizeof(DirectionalLight));
			pixelShader->SetData("light2", &light2, sizeof(DirectionalLight));
			pixelShader->SetData("surfaceColor", &(*entities)[i].mat->surfaceColor, sizeof(XMFLOAT4));
			pixelShader->SetSamplerState("basicSampler", (*entities)[i].mat->GetSampler());
			pixelShader->SetShaderResourceView("diffuseTexture", (*entities)[i].mat->GetSVR());

			vertexShader->CopyAllBufferData();
			pixelShader->CopyAllBufferData();

			vertexShader->SetShader();
			pixelShader->SetShader();

			(*entities)[i].Draw(context);
		}
	}
}
void Renderer::DrawInstanceObject(ID3D11DeviceContext* context, Mesh &obj,int numOfIn, std::vector<GameEntity> entity)
{
	instanceData = new XMFLOAT4X4[numOfIn];
	D3D11_BUFFER_DESC instDesc = {};
	instDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instDesc.ByteWidth = sizeof(XMFLOAT4X4) * numOfIn;
	instDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instDesc.MiscFlags = 0;
	instDesc.StructureByteStride = 0;
	instDesc.Usage = D3D11_USAGE_DYNAMIC;
	device->CreateBuffer(&instDesc, 0, &instanceWorldMatrixBuff);

	for (int i = 0; i < (int)(entity).size(); i++)
	{
		XMMATRIX worldMat = XMMatrixTranspose(XMLoadFloat4x4(&entity[i].GetWorldMatrix()));
		XMStoreFloat4x4(&instanceData[i], XMMatrixTranspose(worldMat));
	}

	D3D11_MAPPED_SUBRESOURCE mapped = {};
	context->Map(instanceWorldMatrixBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	// Copy to the resource
	memcpy(mapped.pData, instanceData, sizeof(XMFLOAT4X4) * numOfIn);

	// Unmap so the GPU can use it again
	context->Unmap(instanceWorldMatrixBuff, 0);

	ID3D11Buffer* vbs[2] = {obj.GetVertexBuffer() ,instanceWorldMatrixBuff	};
	UINT strides[2] = { sizeof(Vertex), sizeof(XMFLOAT4X4) };
	UINT offsets[2] = { 0, 0 };

	ID3D11Buffer* ib = obj.GetIndexBuffer();

	context->IASetVertexBuffers(0, 2, vbs, strides, offsets);
	context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexedInstanced(obj.GetIndexCount(),numOfIn,0,0,0); 
}
	


