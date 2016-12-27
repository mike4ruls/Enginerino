#pragma once
#include "GameEntity.h"


class TetrisBlock
{
public:
	TetrisBlock(Mesh &bShape, Material &mat, int tp, int r);
	~TetrisBlock();

	DirectX::XMFLOAT3 translation;


	/// The type of Block
	//1. Straight Block
	//2. Z Block
	//3. Reverse Z Block
	//4. L Block
	//5. Reverse L Block
	//6. Square Block
	int type;
	int rot;
	// The status of the block
	//1. Falling 
	//2. Placed
	int status;
	std::vector<GameEntity> tetrisBlocks;

	void LoadTetrisBlock();
	void TransTetrisBlock(float x, float y, float z);
	void TransTetrisBlock(DirectX::XMFLOAT3 trans);
	std::vector<GameEntity> GetEntities();


private:

};

