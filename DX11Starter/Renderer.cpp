#include "Renderer.h"


using namespace DirectX;
Renderer::Renderer(std::vector<GameEntity> &en, SimpleVertexShader &vShader, SimplePixelShader &pShader, SimpleVertexShader &iv, SimplePixelShader &ip, SimpleVertexShader &wv, SimplePixelShader &wp, Tetris &Tet, ID3D11Device &dev)
{
	entities = &en;
	vertexShader = &vShader;
	pixelShader = &pShader;
	instanceVS = &iv;
	instancePS = &ip;
	waterVS = &wv;
	waterPS = &wp;
	TetrisGame = &Tet;
	gotBoard = false;
	device = &dev;
	turnOn = 0;
	freeRoam = false;

	scoreBlocks = &TetrisGame->scoreBlocks;
	otherBlocks = &TetrisGame->otherBlocks;

	instanceStuff= new InstanceStuff[1000];

	D3D11_BUFFER_DESC instDesc = {};
	instDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instDesc.ByteWidth = sizeof(InstanceStuff) * 1000;
	instDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instDesc.MiscFlags = 0;
	instDesc.StructureByteStride = 0;
	instDesc.Usage = D3D11_USAGE_DYNAMIC;
	device->CreateBuffer(&instDesc, 0, &instanceStuffBuff);

}


Renderer::~Renderer()
{
	if (instanceStuff != nullptr) { delete instanceStuff; instanceStuff = nullptr; }
	if (instanceStuffBuff != nullptr) { instanceStuffBuff->Release();  instanceStuffBuff = nullptr; }
}
void Renderer::RenderUpdate(ID3D11DeviceContext* context, Camera cam, DirectionalLight light, DirectionalLight light2)
{
	
	if (TetrisGame->gameStart == true) {
		pixelShader->SetData("on", &turnOn, sizeof(int));


		// The Dark days of rendering everything over and over...
		/*
		for (int i = 0; i < (int)board->size(); i++)
		{
			vertexShader->SetMatrix4x4("world", (*board)[i].GetWorldMatrix());
			vertexShader->SetMatrix4x4("view", cam.viewMatrix);
			vertexShader->SetMatrix4x4("projection", cam.projectionMatrix);
			vertexShader->SetData("camPos", &cam.camPos, sizeof(XMFLOAT4));

			pixelShader->SetData("light", &light, sizeof(DirectionalLight));
			pixelShader->SetData("light2", &light2, sizeof(DirectionalLight));
			pixelShader->SetData("surfaceColor", &(*board)[i].mat->surfaceColor, sizeof(XMFLOAT4));

			vertexShader->CopyAllBufferData();
			pixelShader->CopyAllBufferData();

			vertexShader->SetShader();
			pixelShader->SetShader();

			(*board)[i].Draw(context);
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
		*/


		DrawInstanceObject(context, cam, light, light2, (int)(allBlocks).size(),  allBlocks);
		/*if (TetrisGame->gameOver == true) 
		{
			for (int i = 0; i < (int)otherBlocks->size(); i++)
			{
				vertexShader->SetMatrix4x4("world", (*otherBlocks)[i].GetWorldMatrix());
				vertexShader->SetMatrix4x4("view", cam.viewMatrix);
				vertexShader->SetMatrix4x4("projection", cam.projectionMatrix);
				vertexShader->SetData("camPos", &cam.camPos, sizeof(XMFLOAT4));

				pixelShader->SetData("light", &light, sizeof(DirectionalLight));
				pixelShader->SetData("light2", &light2, sizeof(DirectionalLight));
				pixelShader->SetData("surfaceColor", &(*otherBlocks)[i].mat->surfaceColor, sizeof(XMFLOAT4));

				vertexShader->CopyAllBufferData();
				pixelShader->CopyAllBufferData();

				vertexShader->SetShader();
				pixelShader->SetShader();

				(*otherBlocks)[i].Draw(context);
			}
		}*/
	}
	else
	{
		if (freeRoam) {
			turnOn = 1;
			for (int i = 0; i < (int)(*entities).size(); i++)
			{
				if((*entities)[i].thisType == terrainObject)
				{
					waterVS->SetMatrix4x4("world", (*entities)[i].GetWorldMatrix());
					waterVS->SetMatrix4x4("view", cam.viewMatrix);
					waterVS->SetMatrix4x4("projection", cam.projectionMatrix);
					waterVS->SetData("camPos", &cam.camPos, sizeof(XMFLOAT4));

					waterPS->SetData("light", &light, sizeof(DirectionalLight));
					waterPS->SetData("light2", &light2, sizeof(DirectionalLight));
					waterPS->SetData("surfaceColor", &(*entities)[i].mat->surfaceColor, sizeof(XMFLOAT4));
					waterPS->SetSamplerState("basicSampler", (*entities)[i].mat->GetSampler());
					waterPS->SetShaderResourceView("diffuseTexture", (*entities)[i].mat->GetSVR());
					waterPS->SetData("on", &turnOn, sizeof(int));

					waterVS->CopyAllBufferData();
					waterPS->CopyAllBufferData();

					waterVS->SetShader();
					waterPS->SetShader();

					(*entities)[i].Draw(context);
				}
				else
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
					pixelShader->SetData("on", &turnOn, sizeof(int));

					vertexShader->CopyAllBufferData();
					pixelShader->CopyAllBufferData();

					vertexShader->SetShader();
					pixelShader->SetShader();

					(*entities)[i].Draw(context);
				}
				
			}
		}
	}
}
void Renderer::DrawInstanceObject(ID3D11DeviceContext* context, Camera cam, DirectionalLight light, DirectionalLight light2,int numOfIn, std::vector<GameEntity> entity)
{
	if (instanceStuff != nullptr) { delete instanceStuff; instanceStuff = nullptr; }
	instanceStuff = new InstanceStuff[numOfIn];
	for (int i = 0; i < (int)scoreBlocks->size(); i++)
	{
		allBlocks.pop_back();
	}
	for (int i = 0; i < (int)scoreBlocks->size(); i++)
	{
		allBlocks.push_back((*scoreBlocks)[i]);
	}

	for (int i = 0; i < (int)(entity).size(); i++)
	{
		XMStoreFloat4x4(&instanceStuff[i].worldMatr, XMLoadFloat4x4(&entity[i].GetWorldMatrix()));
		XMStoreFloat4(&instanceStuff[i].surColor, XMLoadFloat4(&entity[i].mat->surfaceColor));
	}

	D3D11_MAPPED_SUBRESOURCE mapped = {};
	context->Map(instanceStuffBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	// Copy to the resource
	memcpy(mapped.pData, instanceStuff, sizeof(InstanceStuff) * numOfIn);

	// Unmap so the GPU can use it again
	context->Unmap(instanceStuffBuff, 0);


	ID3D11Buffer* vbs[2] = { entity[0].obj->GetVertexBuffer() ,instanceStuffBuff};
	UINT strides[2] = { sizeof(Vertex), sizeof(InstanceStuff) };
	UINT offsets[2] = { 0, 0 };

	ID3D11Buffer* ib = entity[0].obj->GetIndexBuffer();

	context->IASetVertexBuffers(0, 2, vbs, strides, offsets);
	context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

	instanceVS->SetMatrix4x4("world", (*board)[0].GetWorldMatrix());
	instanceVS->SetMatrix4x4("view", cam.viewMatrix);
	instanceVS->SetMatrix4x4("projection", cam.projectionMatrix);
	instanceVS->SetData("camPos", &cam.camPos, sizeof(XMFLOAT4));

	instanceVS->CopyAllBufferData();
	instanceVS->SetShader();

	instancePS->SetData("light", &light, sizeof(DirectionalLight));
	instancePS->SetData("light2", &light2, sizeof(DirectionalLight));
	instancePS->SetData("surfaceColor", &(*entities)[0].mat->surfaceColor, sizeof(XMFLOAT4));
	instancePS->SetSamplerState("basicSampler", (*entities)[0].mat->GetSampler());
	instancePS->SetShaderResourceView("diffuseTexture", (*entities)[0].mat->GetSVR());
	instancePS->SetData("on", &turnOn, sizeof(int));

	instancePS->CopyAllBufferData();
	instancePS->SetShader();

	context->DrawIndexedInstanced(entity[0].obj->GetIndexCount(),numOfIn,0,0,0);
}
void Renderer::LoadAllBlocks()
{
	allBlocks.clear();
	for (int i = 0; i < (int)board->size(); i++)
	{
		allBlocks.push_back((*board)[i]);
	}
	for (int i = 0; i < (int)tBlocks.size(); i++)
	{
		allBlocks.push_back((tBlocks)[i]);
	}
	for (int i = 0; i < (int)pBlocks.size(); i++)
	{
		allBlocks.push_back((pBlocks)[i]);
	}
	for (int i = 0; i < (int)scoreBlocks->size(); i++)
	{
		allBlocks.push_back((*scoreBlocks)[i]);
	}
}


