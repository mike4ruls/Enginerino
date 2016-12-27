#include "GameEntity.h"

using namespace DirectX;

GameEntity::GameEntity()
{

}

GameEntity::GameEntity(Mesh &o, std::string n)
{
	obj = &o;

	position = { 0.0f,0.0f,0.0f };
	rotation = { 0.0f,0.0f,0.0f };
	scale    = { 1.0f,1.0f,1.0f };

	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));

	std::string name = n;
}


GameEntity::~GameEntity()
{
	//if (obj) { obj->~Mesh(); }
}

///
///
//Get Properties Methods 
XMFLOAT4X4 GameEntity::GetWorldMatrix()
{
	return worldMatrix;
}
XMFLOAT3 GameEntity::GetPosition()
{
	return position;
}
XMFLOAT3 GameEntity::GetRotation()
{
	return rotation;
}
XMFLOAT3 GameEntity::GetScale()
{
	return scale;
}


///
///
//Set Properties Methods 
void GameEntity::SetWorldMatrix()
{
	XMMATRIX trans = XMMatrixTranslation(GameEntity::GetPosition().x, GameEntity::GetPosition().y, GameEntity::GetPosition().z);
	XMMATRIX rotx   = XMMatrixRotationX(GameEntity::GetRotation().x);
	XMMATRIX roty   = XMMatrixRotationY(GameEntity::GetRotation().y);
	XMMATRIX rotz   = XMMatrixRotationZ(GameEntity::GetRotation().z);
	XMMATRIX scale = XMMatrixScaling(GameEntity::GetScale().x, GameEntity::GetScale().y, GameEntity::GetScale().z);

	XMMATRIX world = scale * rotx * roty * rotz * trans;

	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(world));

}


void GameEntity::SetPosition(XMFLOAT3 pos)
{
	position = pos;
}

void GameEntity::SetRotation(XMFLOAT3 rot)
{
	rotation = rot;
}

void GameEntity::SetScale(XMFLOAT3 sca)
{
	scale = sca;
}



///
///
//Transformation Methods
void GameEntity::Translate(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;

	GameEntity::SetWorldMatrix();
	 
}
void GameEntity::Rotate(float x, float y, float z)
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;

	GameEntity::SetWorldMatrix();
}
void GameEntity::Scale(float x, float y, float z)
{
	scale.x += x;
	scale.y += y;
	scale.z += z;

	GameEntity::SetWorldMatrix();
}



///
///
//Method to offset the object from its center
void GameEntity::OffSetMove()
{

}
void GameEntity::Draw(ID3D11DeviceContext* context)
{
	obj->Draw(context);
}
