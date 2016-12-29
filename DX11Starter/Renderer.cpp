#include "Renderer.h"


using namespace DirectX;
Renderer::Renderer(std::vector<GameEntity> &en, SimpleVertexShader &vShader, SimplePixelShader &pShader, Tetris &Tet)
{
	entities = &en;
	vertexShader = &vShader;
	pixelShader = &pShader;
	TetrisGame = &Tet;
	gotBoard = false;
}


Renderer::~Renderer()
{
	//delete &entities;
}
void Renderer::RenderUpdate(ID3D11DeviceContext* context, Camera cam, DirectionalLight light, DirectionalLight light2)
{
	if (TetrisGame->gameStart == true) {
		for (int i = 0; i < (int)board.size(); i++)
		{

			// Send data to shader variables
			//  - Do this ONCE PER OBJECT you're drawing
			//  - This is actually a complex process of copying data to a local buffer
			//    and then copying that entire buffer to the GPU.  
			//  - The "SimpleShader" class handles all of that for you.
			vertexShader->SetMatrix4x4("world", board[i].GetWorldMatrix());
			vertexShader->SetMatrix4x4("view", cam.viewMatrix);
			vertexShader->SetMatrix4x4("projection", cam.projectionMatrix);
			vertexShader->SetData("camPos", &cam.camPos, sizeof(XMFLOAT4));

			pixelShader->SetData("light", &light, sizeof(DirectionalLight));
			pixelShader->SetData("light2", &light2, sizeof(DirectionalLight));
			pixelShader->SetData("surfaceColor", &board[i].mat->surfaceColor, sizeof(XMFLOAT4));

			// Once you've set all of the data you care to change for
			// the next draw call, you need to actually send it to the GPU
			//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
			vertexShader->CopyAllBufferData();
			pixelShader->CopyAllBufferData();

			// Set the vertex and pixel shaders to use for the next Draw() command
			//  - These don't technically need to be set every frame...YET
			//  - Once you start applying different shaders to different objects,
			//    you'll need to swap the current shaders before each draw


			vertexShader->SetShader();
			pixelShader->SetShader();

			// Set buffers in the input assembler
			//  - Do this ONCE PER OBJECT you're drawing, since each object might
			//    have different geometry.
			//(entities + i)->PrepareMaterial(viewMatrix, projectionMatrix);

			board[i].Draw(context);
		}
		for (int i = 0; i < (int)tBlocks.size(); i++)
		{

			// Send data to shader variables
			//  - Do this ONCE PER OBJECT you're drawing
			//  - This is actually a complex process of copying data to a local buffer
			//    and then copying that entire buffer to the GPU.  
			//  - The "SimpleShader" class handles all of that for you.
			vertexShader->SetMatrix4x4("world", tBlocks[i].GetWorldMatrix());
			vertexShader->SetMatrix4x4("view", cam.viewMatrix);
			vertexShader->SetMatrix4x4("projection", cam.projectionMatrix);
			vertexShader->SetData("camPos", &cam.camPos, sizeof(XMFLOAT4));

			pixelShader->SetData("light", &light, sizeof(DirectionalLight));
			pixelShader->SetData("light2", &light2, sizeof(DirectionalLight));
			pixelShader->SetData("surfaceColor", &tBlocks[i].mat->surfaceColor, sizeof(XMFLOAT4));

			// Once you've set all of the data you care to change for
			// the next draw call, you need to actually send it to the GPU
			//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
			vertexShader->CopyAllBufferData();
			pixelShader->CopyAllBufferData();

			// Set the vertex and pixel shaders to use for the next Draw() command
			//  - These don't technically need to be set every frame...YET
			//  - Once you start applying different shaders to different objects,
			//    you'll need to swap the current shaders before each draw


			vertexShader->SetShader();
			pixelShader->SetShader();

			// Set buffers in the input assembler
			//  - Do this ONCE PER OBJECT you're drawing, since each object might
			//    have different geometry.
			//(entities + i)->PrepareMaterial(viewMatrix, projectionMatrix);

			tBlocks[i].Draw(context);
		}
		for (int i = 0; i < (int)pBlocks.size(); i++)
		{

			// Send data to shader variables
			//  - Do this ONCE PER OBJECT you're drawing
			//  - This is actually a complex process of copying data to a local buffer
			//    and then copying that entire buffer to the GPU.  
			//  - The "SimpleShader" class handles all of that for you.
			vertexShader->SetMatrix4x4("world", pBlocks[i].GetWorldMatrix());
			vertexShader->SetMatrix4x4("view", cam.viewMatrix);
			vertexShader->SetMatrix4x4("projection", cam.projectionMatrix);
			vertexShader->SetData("camPos", &cam.camPos, sizeof(XMFLOAT4));

			pixelShader->SetData("light", &light, sizeof(DirectionalLight));
			pixelShader->SetData("light2", &light2, sizeof(DirectionalLight));
			pixelShader->SetData("surfaceColor", &pBlocks[i].mat->surfaceColor, sizeof(XMFLOAT4));

			// Once you've set all of the data you care to change for
			// the next draw call, you need to actually send it to the GPU
			//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
			vertexShader->CopyAllBufferData();
			pixelShader->CopyAllBufferData();

			// Set the vertex and pixel shaders to use for the next Draw() command
			//  - These don't technically need to be set every frame...YET
			//  - Once you start applying different shaders to different objects,
			//    you'll need to swap the current shaders before each draw


			vertexShader->SetShader();
			pixelShader->SetShader();

			// Set buffers in the input assembler
			//  - Do this ONCE PER OBJECT you're drawing, since each object might
			//    have different geometry.
			//(entities + i)->PrepareMaterial(viewMatrix, projectionMatrix);

			pBlocks[i].Draw(context);
		}
	}
	/*
	for(int i= 0; i < (int)(*entities).size(); i++)
	{

		// Send data to shader variables
		//  - Do this ONCE PER OBJECT you're drawing
		//  - This is actually a complex process of copying data to a local buffer
		//    and then copying that entire buffer to the GPU.
		//  - The "SimpleShader" class handles all of that for you.
		vertexShader->SetMatrix4x4("world", (*entities)[i].GetWorldMatrix());
		vertexShader->SetMatrix4x4("view", cam.viewMatrix);
		vertexShader->SetMatrix4x4("projection", cam.projectionMatrix);
		vertexShader->SetData("camPos", &cam.camPos, sizeof(XMFLOAT4));

		pixelShader->SetData("light", &light, sizeof(DirectionalLight));
		pixelShader->SetData("light2", &light2, sizeof(DirectionalLight));
		pixelShader->SetData("surfaceColor", &(*entities)[i].mat->surfaceColor, sizeof(XMFLOAT4));

		// Once you've set all of the data you care to change for
		// the next draw call, you need to actually send it to the GPU
		//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
		vertexShader->CopyAllBufferData();
		pixelShader->CopyAllBufferData();

		// Set the vertex and pixel shaders to use for the next Draw() command
		//  - These don't technically need to be set every frame...YET
		//  - Once you start applying different shaders to different objects,
		//    you'll need to swap the current shaders before each draw


		vertexShader->SetShader();
		pixelShader->SetShader();

		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		//(entities + i)->PrepareMaterial(viewMatrix, projectionMatrix);

		(*entities)[i].Draw(context);
	}*/
}
	


