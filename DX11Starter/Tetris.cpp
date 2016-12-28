#include "Tetris.h"



Tetris::Tetris(Mesh &shape, Material &r, Material &b, Material &g, Material &p)
{
	shapeBlock = &shape;
	red = &r;
	blue = &b;
	green = &g;
	purple = &p;
	gameStart = false;
	
	currentState = false;
	previousState = false;
	tTime = 0.0;
	timeOfDescent = 4.0;
	srand((unsigned int)time(NULL));
}


Tetris::~Tetris()
{
	if (currentBlock != nullptr) { delete currentBlock; currentBlock = nullptr; }
	if (futureBlock != nullptr) { delete futureBlock; futureBlock = nullptr; }

	for (int i = 0; i < (int)(tBlocks).size(); i++)
	{
		if (tBlocks[i] != nullptr) { delete tBlocks[i]; tBlocks[i] = nullptr; }
	}
}
void Tetris::UpdateGame()
{
	MoveBlock();
	CheckWallCollide();
	CheckFloorCollide();

	tTime += (float)0.1;
	previousState = currentState;
	(currentBlock)->LoadTetrisBlock();
	SetFutureBlock();
	currentState = false;
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
	int ranBlock = rand() % 6 + 1;
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
	SetCurrentBlock(block);
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
void Tetris::SetCurrentBlock(TetrisBlock *block)
{
	int newH = height - 1;
	int newW = (int)(width / 2);
	(block)->translation = { (float)newW,(float)newH,0.0 };
	(block)->LoadTetrisBlock();
}
void Tetris::SetFutureBlock()
{
	int newH = (int)(height/2);
	int newW = (int)width + 5;
	(futureBlock)->translation = { (float)newW,(float)newH,0.0 };
	(futureBlock)->LoadTetrisBlock();
}
void Tetris::MoveBlock()
{
	if (GetAsyncKeyState(VK_RETURN))
	{
		currentState = true;
		if (currentState != previousState) {
			currentBlock->rot += 1;
		}
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		currentState = true;
		if (currentState != previousState) {
			currentBlock->TransTetrisBlock(-1.0, 0.0, 0.0);
		}
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		currentState = true;
		if (currentState != previousState) {
			currentBlock->TransTetrisBlock(1.0, 0.0, 0.0);
		}
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		currentState = true;
		currentBlock->TransTetrisBlock(0.0, -1.0, 0.0);
		tTime = 0.0;
	}

	if (tTime >= timeOfDescent)
	{
		currentBlock->TransTetrisBlock(0.0, -1.0, 0.0);
		tTime = 0.0;
	}
}
void Tetris::CheckWallCollide()
{
	for (int i = 0; i < (int)(currentBlock)->GetEntities().size(); i++)
	{
		if ((currentBlock)->GetEntities()[i].GetPosition().x <= board[1].GetPosition().x)
		{
			(currentBlock)->TransTetrisBlock(1.0, 0.0, 0.0);
			//(currentBlock)->LoadTetrisBlock();
			//CheckWallCollide();
			//break;
		}
		else if ((currentBlock)->GetEntities()[i].GetPosition().x >= board[1].GetPosition().x + (width + 1))
		{
			(currentBlock)->TransTetrisBlock(-1.0,0.0,0.0);
			//(currentBlock)->LoadTetrisBlock();
			//CheckWallCollide();
			//break;
		}
	}
}
void Tetris::CheckFloorCollide()
{
	for (int i = 0; i < (int)(currentBlock)->GetEntities().size(); i++)
	{
		if ((currentBlock)->GetEntities()[i].GetPosition().y <= board[1].GetPosition().y - 1)
		{
			(currentBlock)->TransTetrisBlock(0.0, 1.0, 0.0);
			tBlocks.push_back(currentBlock);
			currentBlock = futureBlock;
			futureBlock = GenerateBlock();
			SetCurrentBlock(currentBlock);
		}
	}
}
