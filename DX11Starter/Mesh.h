#pragma once

#include <d3d11.h>
#include <vector>
#include <d3d11.h>
#include <fstream>
#include "Vertex.h"
class Mesh
{
public:
	Mesh(Vertex v[], int numOfV, int i[], int numOfI, ID3D11Device &dev);
	Mesh(const char* fileName, ID3D11Device &dev);
	~Mesh();

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	void Draw(ID3D11DeviceContext* context);
	int GetIndexCount();
	int GetVertexCount();
	void CalculateTangents(Vertex* verts, int numVerts, int* indices, int numIndices);
	void CreateBuffer(Vertex v[], int numOfV, int i[], int numOfI, ID3D11Device &dev);
	
private:
	ID3D11Buffer* vertBuffer = nullptr;
	ID3D11Buffer* indBuffer = nullptr;

	ID3D11ShaderResourceView* SVR = nullptr;

	int numOfIndex;
	int numOfVertex;
};

