#pragma once
#include "GameEntity.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Material.h"
#include <DirectXMath.h>
#include <stdlib.h>

class Terrain
{
public:
	Terrain(int le, int wd, Material &color, ID3D11Device &dev);
	~Terrain();

	int length;
	int width;
	Mesh* terrainMesh = nullptr;
	GameEntity terrain;
	std::vector<Vertex> verts;
	std::vector<int> ind;


	Mesh* GenerateTerrain(int length, int width, ID3D11Device &dev);
	
	GameEntity GetEntity();
	void ChangeMaterial(Material *newMat);

private:

};

