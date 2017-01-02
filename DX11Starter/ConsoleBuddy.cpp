#include "ConsoleBuddy.h"


ConsoleBuddy::ConsoleBuddy(Tetris &Tetris)
{
	tetrisGame = &Tetris;
	//ClearLine();
	sentIndex = 0;
	textTime = 0.0;
	textSpeed = 0.08f;
	botTalk = false;

	botTime = 0.0;
	botTimeTalk = 0.0;

	previousState = false;
	currentState = false;

	playerStatus = 0;

	tut = 0;
	tetris = 0;
	free = 0;

	
}


ConsoleBuddy::~ConsoleBuddy()
{
}
void ConsoleBuddy::UpdateCB(float deltaTime)
{
	switch(playerStatus)
	{
		//Tutorial
	case 0:
	{
		if (GetAsyncKeyState(VK_RETURN))
		{
			currentState = true;
			if ((previousState != currentState))
			{
				if (sentIndex <= sent.size())
				{
					Talk(sent);
				}
				tut += 1;
				sentIndex = 0;
				if (tut == 6)
				{
					playerStatus += 1;
				}
			}
		}
		switch(tut)
		{
		case 0: 
		{
			sent = "Hi, my name is CB, i'm your Console Buddy!";
			break;
		}
		case 1:
		{
			sent = "I'll be your companion through out your time playing this game";
			break;
		}
		case 2:
		{
			sent = "So could resize my screen height so the bottom kind of meets my text please? Too lazy to do it my self";
			break;
		}
		case 3:
		{
			sent = "Also could you place me somewhere you can see me and the game?(you can resize the game if you need space btw)";
			break;
		}
		case 4:
		{
			sent = "AWESOME, thanks. Now I know you are here for Tetris right?";
			break;
		}
		case 5:
		{
			sent = "Well say no more fam, press enter and i'll send you right to the game ^.^";
			break;
		}
		}

		break;
	}
	//Tetris
	case 1:
	{
		if(botTalk != true)
		{
			if(botTime >= botTimeTalk)
			{
				botTime = 0.0;

				tetris += 1;
				sentIndex = 0;
				if (tetris == 2) { botTimeTalk = 5.0; }
			}
		}
		if (GetAsyncKeyState(VK_RETURN))
		{
			currentState = true;
			if ((previousState != currentState))
			{
				if (sentIndex <= sent.size())
				{
					Talk(sent);
				}
				tetris += 1;
				sentIndex = 0;
				if (tetris == 7)
				{
					playerStatus += 1;
				}
			}
		}
		switch (tetris)
		{
		case 0:
		{
			sent = "Use the 'LEFT' and 'RIGHT' arrow keys to navigate the tetris blocks";
			break;
		}
		case 1:
		{
			sent = "If you want to speed up the block that's falling use the 'DOWN' arrow key and press 'ENTER' to rotate the blocks";
			break;
		}
		case 2:
		{
			sent = "If you're gettng wrekt by this game and doing horribly you can always press 'I' to reset your game :3";
			break;
		}
		case 3:
		{
			sent = "Want to see something cool?";
			break;
		}
		case 4:
		{
			sent = "Go ahead and use the 'WASD' keys and left click around in the game";
			break;
		}
		case 5:
		{
			sent = "LOOK YOU'RE FLYING!!!";
			break;
		}
		case 6:
		{
			sent = "I believe I can fly. I believe I can touch the sky, and all i wanted was some chicken wings...   wait I don't think those are the words...";
			break;
		}
		}
		botTime += 1 * deltaTime;
		break;
	}
	//FreeRoam
	case 2:
	{
		switch (free)
		{
		case 0:
		{
			break;
		}
		}
		break;
	}
	//Reset tetris
	case 3:
	{
		switch (free)
		{
		case 0:
		{
			break;
		}
		}
		break;
	}

	}

	Talk(sent, deltaTime);

	previousState = currentState;
	currentState = false;

}
void ConsoleBuddy::ClearLine()
{
	printf("\r                                      \r");

}
void ConsoleBuddy::Talk(std::string sentance, float deltaTime)
{
	if (sentIndex == 0)
	{
		ClearLine();
		printf("~ ");
		botTalk = true;
	}


	if (botTalk && (textTime >= textSpeed)) 
	{
		if (sentIndex <= sentance.size())
		{	
			//printf("%c", &sentance.at(sentIndex));
			printf("%c", sentance[sentIndex]);
			sentIndex += 1;
		}
		else
		{
			printf("\n(Press Enter To Continue...)");
			botTalk = false;
		}
		textTime = 0.0;
	}
	else
	{
	
	}
	textTime += 1 * deltaTime;
}
void ConsoleBuddy::Talk(std::string sentance)
{
	ClearLine();
	printf("~ ");
	printf(sentance.data());
	printf("\n(Press Enter To Continue...)");
	botTalk = false;
}
