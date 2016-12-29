#include "Tetris.h"



Tetris::Tetris(Mesh &shape, Material &r, Material &b, Material &g, Material &p)
{
	shapeBlock = &shape;
	red = &r;
	blue = &b;
	green = &g;
	purple = &p;
	gameStart = false;
	gameOver = false;
	score = 0;

	tChange = false;
	pChange = false;
	
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

}
void Tetris::UpdateGame()
{
	if (gameOver != true) 
	{
		MoveBlock();
		CheckWallCollide();
		CheckBlockCollide();
		CheckFloorCollide();
		CheckForLine();
		CheckGameOver();
		
		tTime += (float)0.1;
		previousState = currentState;
		(currentBlock)->LoadTetrisBlock();
		SetFutureBlock();
		currentState = false;
	}
	else
	{

	}
}
void Tetris::StartGame(int h, int w)
{
	ResetGame();
	height = h;
	width = w;

	DrawBoard(height, width);
	currentBlock = GenerateBlock();
	futureBlock = GenerateBlock();
	gameStart = true;
	tChange = true;
	pChange = true;
	gameOver = false;

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
std::vector<GameEntity> Tetris::GetTBlocks()
{
	std::vector<GameEntity> blockEntities;
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
std::vector<GameEntity> Tetris::GetPBlocks()
{
	std::vector<GameEntity> blockEntities;
	//Gets placed blocks
	for (int i = 0; i < (int)(tBlocks).size(); i++)
	{
		blockEntities.push_back((tBlocks)[i]);
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
			tChange = true;
		}
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		currentState = true;
		if (currentState != previousState) {
			currentBlock->TransTetrisBlock(-1.0, 0.0, 0.0);
			if(CheckWallBlockCollide())
			{
				currentBlock->TransTetrisBlock(1.0, 0.0, 0.0);
			}
			tChange = true;
		}
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		currentState = true;
		if (currentState != previousState) {
			currentBlock->TransTetrisBlock(1.0, 0.0, 0.0);
			if (CheckWallBlockCollide())
			{
				currentBlock->TransTetrisBlock(-1.0, 0.0, 0.0);
			}
			tChange = true;
		}
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		currentState = true;
		currentBlock->TransTetrisBlock(0.0, -1.0, 0.0);
		tChange = true;
		tTime = 0.0;
	}

	if (tTime >= timeOfDescent)
	{
		currentBlock->TransTetrisBlock(0.0, -1.0, 0.0);
		tChange = true;
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
			tChange = true;
			//(currentBlock)->LoadTetrisBlock();
			//CheckWallCollide();
			//break;
		}
		else if ((currentBlock)->GetEntities()[i].GetPosition().x >= board[1].GetPosition().x + (width + 1))
		{
			(currentBlock)->TransTetrisBlock(-1.0,0.0,0.0);
			tChange = true;
			//(currentBlock)->LoadTetrisBlock();
			//CheckWallCollide();
			//break;
		}
	}
}
void Tetris::CheckBlockCollide()
{
	if ((int)(tBlocks).size() == 0)
	{
	}
	else {
		for (int i = 0; i < (int)(currentBlock)->GetEntities().size(); i++)
		{
			for (int j = 0; j < (int)(tBlocks).size(); j++)
			{
				if ((currentBlock)->GetEntities()[i].GetPosition().x == (tBlocks)[j].GetPosition().x && (currentBlock)->GetEntities()[i].GetPosition().y == (tBlocks)[j].GetPosition().y)
				{
					(currentBlock)->TransTetrisBlock(0.0, 1.0, 0.0);
					PlaceBlock();
					//(currentBlock)->LoadTetrisBlock();
					//CheckWallCollide();
					break;
				}
			}

		}
	}
}
bool Tetris::CheckWallBlockCollide()
{
	if ((int)(tBlocks).size() == 0)
	{
	}
	else {
		for (int i = 0; i < (int)(currentBlock)->GetEntities().size(); i++)
		{
			for (int j = 0; j < (int)(tBlocks).size(); j++)
			{
				if ((currentBlock)->GetEntities()[i].GetPosition().x == (tBlocks)[j].GetPosition().x && (currentBlock)->GetEntities()[i].GetPosition().y == (tBlocks)[j].GetPosition().y)
				{
					return true;
					//(currentBlock)->LoadTetrisBlock();
					//CheckWallCollide();
					break;
				}
			}

		}
	}
	return false;
}
void Tetris::CheckFloorCollide()
{
	for (int i = 0; i < (int)(currentBlock)->GetEntities().size(); i++)
	{
		if ((currentBlock)->GetEntities()[i].GetPosition().y <= board[1].GetPosition().y - 1)
		{
			(currentBlock)->TransTetrisBlock(0.0, 1.0, 0.0);
			PlaceBlock();
		}
	}
}
void Tetris::PlaceBlock()
{
	for (int i = 0; i < (int)(currentBlock)->GetEntities().size(); i++)
	{
		tBlocks.push_back(currentBlock->GetEntities()[i]);
	}
	if (currentBlock != nullptr) { delete currentBlock; currentBlock = nullptr; }
	currentBlock = futureBlock;
	futureBlock = GenerateBlock();
	SetCurrentBlock(currentBlock);
	pChange = true;
}
void Tetris::CheckForLine()
{
	std::vector<int> spot;
	int numOfBlocks = 0;

	for (int i = 1; i < height; i++) 
	{
		for (int j = 0; j < (int)(tBlocks).size(); j++)
		{
			if(tBlocks[j].GetPosition().y == i)
			{
				numOfBlocks += 1;
				spot.push_back(j);
			}
		}
		if(numOfBlocks == width)
		{
			for (int j = 0; j < (int)(spot).size();j++)
			{
				for (int k = j; k < (int)(spot).size(); k++)
				{
					if(spot[j]<spot[k])
					{
						std::iter_swap(spot.begin() + j, spot.begin() + k);
					}
				}
			}
			for (int k = 0; k < width; k++) 
			{
				tBlocks.erase(tBlocks.begin()+spot[k]);
			}
			score += 100;
			Reposition(i);
		}
		numOfBlocks = 0;
		spot.clear();
	}
}
void Tetris::Reposition(int yHieght)
{
	for (int i = 0; i < (int)(tBlocks).size();i++)
	{
		if(tBlocks[i].GetPosition().y > yHieght)
		{
			tBlocks[i].Translate(0.0, -1.0, 0.0);
		}
	}
	pChange = true;
}
void Tetris::CheckGameOver()
{
	for (int i = 0; i < (int)(tBlocks).size(); i++)
	{
		if(tBlocks[i].GetPosition().y == height)
		{
			gameOver = true;
			//gameStart = false;
			break;
		}
	}
}
void Tetris::ResetGame()
{
	tBlocks.clear();
	if (currentBlock != nullptr) { delete currentBlock; currentBlock = nullptr; }
	if (futureBlock != nullptr) { delete futureBlock; futureBlock = nullptr; }

	score = 0;
	tTime = 0.0;
}
