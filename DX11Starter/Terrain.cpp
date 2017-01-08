#include "Terrain.h"



Terrain::Terrain(int le, int wd, Material &color, ID3D11Device &dev)
{
	length = le;
	width = wd;
	terrainMesh = GenerateTerrain(le, wd, dev);
	terrain = GameEntity(*terrainMesh,"",color);
	terrain.thisType = terrainObject;
}


Terrain::~Terrain()
{
	if (terrainMesh != nullptr) { delete terrainMesh;  terrainMesh = nullptr; }
}



Mesh* Terrain::GenerateTerrain(int length, int width, ID3D11Device &dev)
{
	DirectX::XMFLOAT3 tangent = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 normal = DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f);
	DirectX::XMFLOAT2 uv = DirectX::XMFLOAT2(0.0f, 0.0f);

	Vertex *terVerts;
	int *terInd;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < length; j++)
		{
			Vertex thisVert= { DirectX::XMFLOAT3(+(float)(i), +0.0f, +(float)(j)), uv, normal, tangent};
			verts.push_back(thisVert);
		}
	}

	int i = 0;
	int stride = 1;
	do
	{
		if(i == ((length * stride) - 1) && i != 0)
		{
			i++;
			stride++;
		}
		ind.push_back(i);
		ind.push_back(i + 1);
		ind.push_back(i + (length + 1));
		ind.push_back(i);
		ind.push_back(i + (length + 1));
		ind.push_back(i + (length));

		i++;
	} while (i + (length + 1) != verts.size());

	
	terVerts= new Vertex[verts.size()];
	terInd = new int[ind.size()];


	for (unsigned int i = 0; i < verts.size(); i++)
	{
		terVerts[i] = verts[i];
	}
	for (unsigned int i = 0; i < ind.size(); i++)
	{
		terInd[i] = ind[i];
	}
	Mesh* newMesh = new Mesh(terVerts, verts.size(), terInd, ind.size(), dev);

	delete terVerts;
	delete terInd;

	return newMesh;

}

GameEntity Terrain::GetEntity()
{
	return terrain;
}
void Terrain::ChangeMaterial(Material *newMat)
{
	terrain.LoadMaterial(*newMat);
}
