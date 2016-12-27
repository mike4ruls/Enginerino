#include "Renderer.h"


using namespace DirectX;
Renderer::Renderer(GameEntity *en, SimpleVertexShader* vShader, SimplePixelShader* pShader)
{
	entities = en;
	vertexShader = vShader;
	pixelShader = pShader;
}


Renderer::~Renderer()
{
	//delete &entities;
}
void Renderer::RenderUpdate(ID3D11DeviceContext* context, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix)
{
	for(int i= 0; i<sizeof(entities); i++)
	{
		// Send data to shader variables
		//  - Do this ONCE PER OBJECT you're drawing
		//  - This is actually a complex process of copying data to a local buffer
		//    and then copying that entire buffer to the GPU.  
		//  - The "SimpleShader" class handles all of that for you.
		vertexShader->SetMatrix4x4("world", (entities + i)->GetWorldMatrix());
		vertexShader->SetMatrix4x4("view", viewMatrix);
		vertexShader->SetMatrix4x4("projection", projectionMatrix);

		// Once you've set all of the data you care to change for
		// the next draw call, you need to actually send it to the GPU
		//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
		vertexShader->CopyAllBufferData();

		// Set the vertex and pixel shaders to use for the next Draw() command
		//  - These don't technically need to be set every frame...YET
		//  - Once you start applying different shaders to different objects,
		//    you'll need to swap the current shaders before each draw
		vertexShader->SetShader();
		pixelShader->SetShader();

		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		(entities + i)->Draw(context);
	}
}
