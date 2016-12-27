#pragma once

#include <d3d11.h>
#include "Vertex.h"
class Mesh
{
public:
	Mesh(Vertex v[], int numOfV, int i[], int numOfI, ID3D11Device* dev);
	~Mesh();

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	void Draw(ID3D11DeviceContext* context);
	int GetIndexCount();
	
private:
	ID3D11Buffer* vertBuffer;
	ID3D11Buffer* indBuffer;
	int numOfIndex;
};
