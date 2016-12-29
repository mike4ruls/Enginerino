#include "TetrisBlock.h"



TetrisBlock::TetrisBlock(Mesh &bShape, Material &mat, int tp, int r)
{
	type = tp;
	rot = r;
	translation = {0.0, 0.0, 0.0};
	for (int i = 0; i < 5; i++)
	{
		tetrisBlocks.push_back(GameEntity(bShape, "Box", mat));
	}

	// If the shape is a square
	if(type == 6)
	{
		tetrisBlocks.pop_back();
	}

	LoadTetrisBlock();
}

std::vector<GameEntity> TetrisBlock::GetEntities()
{
	return tetrisBlocks;
}

TetrisBlock::~TetrisBlock()
{

}
void TetrisBlock::LoadTetrisBlock()
{
	if(rot > 4)
	{
		rot = 1;
	}
	if (type > 6)
	{
		type = 1;
	}

	if (type == 6) {
		(tetrisBlocks)[0].ResetEntity();
		(tetrisBlocks)[1].ResetEntity();
		(tetrisBlocks)[2].ResetEntity();
		(tetrisBlocks)[3].ResetEntity();
	}
	else {
		(tetrisBlocks)[0].ResetEntity();
		(tetrisBlocks)[1].ResetEntity();
		(tetrisBlocks)[2].ResetEntity();
		(tetrisBlocks)[3].ResetEntity();
		(tetrisBlocks)[4].ResetEntity();
	}

	switch(type)
	{
		//STRAIGHT BLOCK
	case 1:
	{
		if(rot == 1)
		{
			(tetrisBlocks)[0].Translate(2.0, 0.0, 0.0);
			(tetrisBlocks)[1].Translate(1.0, 0.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(-1.0, 0.0, 0.0);
			(tetrisBlocks)[4].Translate(-2.0, 0.0, 0.0);
		}
		if (rot == 2)
		{
			(tetrisBlocks)[0].Translate(0.0, -2.0, 0.0);
			(tetrisBlocks)[1].Translate(0.0, -1.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(0.0, 1.0, 0.0);
			(tetrisBlocks)[4].Translate(0.0, 2.0, 0.0);
		}
		if (rot == 3)
		{
			(tetrisBlocks)[0].Translate(-2.0, 0.0, 0.0);
			(tetrisBlocks)[1].Translate(-1.0, 0.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(1.0, 0.0, 0.0);
			(tetrisBlocks)[4].Translate(2.0, 0.0, 0.0);
		}
		if (rot == 4)
		{
			(tetrisBlocks)[0].Translate(0.0, 2.0, 0.0);
			(tetrisBlocks)[1].Translate(0.0, 1.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(0.0, -1.0, 0.0);
			(tetrisBlocks)[4].Translate(0.0, -2.0, 0.0);
		}
		break;
	}
	// Z BLOCK
	case 2:
	{
		if (rot == 1)
		{
			(tetrisBlocks)[0].Translate(1.0, 1.0, 0.0);
			(tetrisBlocks)[1].Translate(1.0, 0.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(-1.0, 0.0, 0.0);
			(tetrisBlocks)[4].Translate(-1.0, -1.0, 0.0);
		}
		if (rot == 2)
		{
			(tetrisBlocks)[0].Translate(1.0, -1.0, 0.0);
			(tetrisBlocks)[1].Translate(0.0, -1.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(0.0, 1.0, 0.0);
			(tetrisBlocks)[4].Translate(-1.0, 1.0, 0.0);
		}
		if (rot == 3)
		{
			(tetrisBlocks)[0].Translate(-1.0, -1.0, 0.0);
			(tetrisBlocks)[1].Translate(-1.0, 0.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(1.0, 0.0, 0.0);
			(tetrisBlocks)[4].Translate(1.0, 1.0, 0.0);
		}
		if (rot == 4)
		{
			(tetrisBlocks)[0].Translate(-1.0, 1.0, 0.0);
			(tetrisBlocks)[1].Translate(0.0, 1.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(0.0, -1.0, 0.0);
			(tetrisBlocks)[4].Translate(1.0, -1.0, 0.0);
		}
		break;
	}
	// REVERSE Z BLOCK
	case 3:
	{
		if (rot == 1)
		{
			(tetrisBlocks)[0].Translate(1.0, -1.0, 0.0);
			(tetrisBlocks)[1].Translate(1.0, 0.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(-1.0, 0.0, 0.0);
			(tetrisBlocks)[4].Translate(-1.0, 1.0, 0.0);
		}
		if (rot == 2)
		{
			(tetrisBlocks)[0].Translate(-1.0, -1.0, 0.0);
			(tetrisBlocks)[1].Translate(0.0, -1.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(0.0, 1.0, 0.0);
			(tetrisBlocks)[4].Translate(1.0, 1.0, 0.0);
		}
		if (rot == 3)
		{
			(tetrisBlocks)[0].Translate(-1.0, 1.0, 0.0);
			(tetrisBlocks)[1].Translate(-1.0, 0.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(1.0, 0.0, 0.0);
			(tetrisBlocks)[4].Translate(1.0, -1.0, 0.0);
		}
		if (rot == 4)
		{
			(tetrisBlocks)[0].Translate(1.0, 1.0, 0.0);
			(tetrisBlocks)[1].Translate(0.0, 1.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(0.0, -1.0, 0.0);
			(tetrisBlocks)[4].Translate(-1.0, -1.0, 0.0);
		}

		break;
	}
	// L BLOCK
	case 4:
	{
		if (rot == 1)
		{
			(tetrisBlocks)[0].Translate(1.0, 1.0, 0.0);
			(tetrisBlocks)[1].Translate(1.0, 0.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(-1.0, 0.0, 0.0);
			(tetrisBlocks)[4].Translate(-2.0, 0.0, 0.0);
		}
		if (rot == 2)
		{
			(tetrisBlocks)[0].Translate(1.0, -1.0, 0.0);
			(tetrisBlocks)[1].Translate(0.0, -1.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(0.0, 1.0, 0.0);
			(tetrisBlocks)[4].Translate(0.0, 2.0, 0.0);
		}
		if (rot == 3)
		{
			(tetrisBlocks)[0].Translate(-1.0, -1.0, 0.0);
			(tetrisBlocks)[1].Translate(-1.0, 0.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(1.0, 0.0, 0.0);
			(tetrisBlocks)[4].Translate(2.0, 0.0, 0.0);
		}
		if (rot == 4)
		{
			(tetrisBlocks)[0].Translate(-1.0, 1.0, 0.0);
			(tetrisBlocks)[1].Translate(0.0, 1.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(0.0, -1.0, 0.0);
			(tetrisBlocks)[4].Translate(0.0, -2.0, 0.0);
		}
		break;
	}
	// REVERSE L BLOCK
	case 5:
	{
		if (rot == 1)
		{
			(tetrisBlocks)[0].Translate(2.0, 0.0, 0.0);
			(tetrisBlocks)[1].Translate(1.0, 0.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(-1.0, 0.0, 0.0);
			(tetrisBlocks)[4].Translate(-1.0, 1.0, 0.0);
		}
		if (rot == 2)
		{
			(tetrisBlocks)[0].Translate(0.0, -2.0, 0.0);
			(tetrisBlocks)[1].Translate(0.0, -1.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(0.0, 1.0, 0.0);
			(tetrisBlocks)[4].Translate(1.0, 1.0, 0.0);
		}
		if (rot == 3)
		{
			(tetrisBlocks)[0].Translate(-2.0, 0.0, 0.0);
			(tetrisBlocks)[1].Translate(-1.0, 0.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(1.0, 0.0, 0.0);
			(tetrisBlocks)[4].Translate(1.0, -1.0, 0.0);
		}
		if (rot == 4)
		{
			(tetrisBlocks)[0].Translate(0.0, 2.0, 0.0);
			(tetrisBlocks)[1].Translate(0.0, 1.0, 0.0);
			(tetrisBlocks)[2].Translate(0.0, 0.0, 0.0);
			(tetrisBlocks)[3].Translate(0.0, -1.0, 0.0);
			(tetrisBlocks)[4].Translate(-1.0, -1.0, 0.0);
		}
		break;
	}
	// SQUARE BLOCK
	case 6:
	{
		(tetrisBlocks)[0].Translate(0.0, 0.0, 0.0);
		(tetrisBlocks)[1].Translate(0.0, -1.0, 0.0);
		(tetrisBlocks)[2].Translate(1.0, -1.0, 0.0);
		(tetrisBlocks)[3].Translate(1.0, 0.0, 0.0);
		break;
	}
	}
	TransTetrisBlock(translation);
}
void TetrisBlock::TransTetrisBlock(float x, float y, float z)
{
	for (int i = 0; i < (int)(tetrisBlocks).size(); i++)
	{
		(tetrisBlocks)[i].Translate(x, y, z);
	}
	translation.x += x;
	translation.y += y;
	translation.z += z;
}
void TetrisBlock::TransTetrisBlock(DirectX::XMFLOAT3 trans)
{
	for (int i = 0; i < (int)(tetrisBlocks).size(); i++)
	{
		(tetrisBlocks)[i].Translate(trans.x, trans.y, trans.z);
	}
}