#include "Tetris.h"



Tetris::Tetris(Mesh &shape, Material &r, Material &b, Material &g, Material &p)
{
	shapeBlock = &shape;
	red = &r;
	blue = &b;
	green = &g;
	purple = &p;
	gameStart = false;
	
}


Tetris::~Tetris()
{
}
void Tetris::StartGame(int h, int w)
{
	height = h;
	width = w;

	DrawBoard(height, width);
	currentBlock = GenerateBlock();
	futureBlock = GenerateBlock();
	gameStart = true;

}

void Tetris::DrawBoard(int height, int width)
{
	for (int i = 0; i < height; i++)
	{
		GameEntity wall = GameEntity(*shapeBlock,"wall",*blue);
		wall.Translate(0.0, (float)i, 0.0);

		board.push_back(wall);
	}
	for (int i = 0; i < width; i++)
	{
		GameEntity wall = GameEntity(*shapeBlock, "wall", *blue);
		wall.Translate((float)(i+1), 0, 0);

		board.push_back(wall);
	}
	for (int i = 0; i < height; i++)
	{
		GameEntity wall = GameEntity(*shapeBlock, "wall", *blue);
		wall.Translate((float)(width + 1), (float)i, 0.0);

		board.push_back(wall);
	}


}

TetrisBlock* Tetris::GenerateBlock()
{
	TetrisBlock* block;
	int ranBlock = rand() %6;
	switch(ranBlock)
	{
	case 1:
	{
		block = new TetrisBlock(*shapeBlock, *blue, ranBlock, 2);
		break;
	}
	case 2:
	{
		block = new TetrisBlock(*shapeBlock, *green, ranBlock, 2);
		break;
	}
	case 3:
	{
		block = new TetrisBlock(*shapeBlock, *purple, ranBlock, 2);
		break;
	}
	case 4:
	{
		block = new TetrisBlock(*shapeBlock, *blue, ranBlock, 2);
		break;
	}
	case 5:
	{
		block = new TetrisBlock(*shapeBlock, *green, ranBlock, 2);
		break;
	}
	case 6:
	{
		block = new TetrisBlock(*shapeBlock, *red, ranBlock, 2);
		break;
	}
	}
	int newH = height - 1;
	int newW = (int)(width / 2);
	(block)->translation = {(float)newW,(float)newH,0.0};
	(block)->LoadTetrisBlock();
	return block;
}
std::vector<GameEntity> Tetris::GetBoard()
{
	return board;
}
std::vector<GameEntity> Tetris::GetBlocks()
{
	std::vector<GameEntity> blockEntities;


	//Gets placed blocks
	for (int i = 0; i < (int)(tBlocks).size(); i++)
	{
		for (int j = 0; j < (int)((tBlocks)[i]->GetEntities()).size(); j++)
		{
			blockEntities.push_back(((tBlocks)[i]->GetEntities())[j]);
		}
	}
	//Gets current block falling
	for (int i = 0; i < (int)(currentBlock)->GetEntities().size(); i++)
	{
		blockEntities.push_back(currentBlock->GetEntities()[i]);
	}
	//Gets future block to fall
	for (int i = 0; i < (int)(futureBlock)->GetEntities().size(); i++)
	{
		blockEntities.push_back(futureBlock->GetEntities()[i]);
	}

	return blockEntities;
}
