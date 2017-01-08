#pragma once

#include <DirectXMath.h>
#include <iostream>
#include <d3d11.h>
#include "Mesh.h"
#include "Material.h"
enum type
{
	gameObject,
	terrainObject
};

class GameEntity
{
public:
	GameEntity();
	GameEntity(Mesh &o, std::string n, Material &mt);
	~GameEntity();

	//Get Properties Methods 
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMFLOAT3 GetScale();

	//Set Properties Methods
	void SetWorldMatrix();
	void SetPosition(DirectX::XMFLOAT3 pos);
	void SetRotation(DirectX::XMFLOAT3 rot);
	void SetScale(DirectX::XMFLOAT3 sca);

	void ResetEntity();
	type thisType;

	//Transformation Methods
	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void Scale(float x, float y, float z);

	//Method to offset the object from its center
	void OffSetMove();

	void Draw(ID3D11DeviceContext* context);

	void PrepareMaterial(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj);
	void LoadMaterial(Material &newMat);
	void InitMesh(Mesh &o);
	void ReleaseMesh();

	Mesh *obj;
	Material *mat;
	 std::string name;


private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

	DirectX::XMFLOAT4X4 worldMatrix;

	
};

