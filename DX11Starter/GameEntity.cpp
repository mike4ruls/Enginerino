#include "GameEntity.h"

using namespace DirectX;

GameEntity::GameEntity()
{

}

GameEntity::GameEntity(Mesh &o, std::string n, Material &mt)
{
	InitMesh(o);
	mat = &mt;

	position = { 0.0f,0.0f,0.0f };
	rotation = { 0.0f,0.0f,0.0f };
	scale    = { 1.0f,1.0f,1.0f };

	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));

	std::string name = n;
	thisType = gameObject;
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

void GameEntity::ResetEntity()
{
	position = { 0.0f,0.0f,0.0f };
	rotation = { 0.0f,0.0f,0.0f };
	scale = { 1.0f,1.0f,1.0f };

	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));

	SetWorldMatrix();
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
void GameEntity::PrepareMaterial(XMFLOAT4X4 view, XMFLOAT4X4 proj)
{
	XMMATRIX viewMatrix = XMLoadFloat4x4(&view);
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(viewMatrix));

	(&mat->GetVertexShader())->SetMatrix4x4("world", worldMatrix);
	(&mat->GetVertexShader())->SetMatrix4x4("view", view);
	(&mat->GetVertexShader())->SetMatrix4x4("projection", proj);

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	(&mat->GetVertexShader())->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	(&mat->GetVertexShader())->SetShader();
	(&mat->GetPixelShader())->SetShader();
}
void GameEntity::InitMesh(Mesh &o)
{
	obj = &o;
}
void GameEntity::ReleaseMesh()
{

}

void GameEntity::LoadMaterial(Material &newMat)
{
	mat = &newMat;
}