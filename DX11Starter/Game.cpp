#include "Game.h"
#include "Vertex.h"
#include <math.h>
#include "WICTextureLoader.h" // From DirectX Tool Kit
#include "DDSTextureLoader.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore( 
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1200,			   // Width of the window's client area
		640,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;


}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Release any (and all!) DirectX objects
	// we've made in the Game class
	if (obj1 != nullptr) { delete obj1; obj1 = nullptr; }
	if (obj2 != nullptr) { delete obj2; obj2 = nullptr;}
	if (obj3 != nullptr) { delete obj3; obj3 = nullptr;}

	if (cone != nullptr) { delete cone; cone = nullptr; }
	if (cube != nullptr) { delete cube; cube = nullptr; }
	if (cylinder != nullptr) { delete cylinder; cylinder = nullptr; }
	if (helix != nullptr) { delete helix; helix = nullptr; }
	if (sphere != nullptr) { delete sphere; sphere = nullptr; }
	if (torus != nullptr) { delete torus; torus = nullptr; }
	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff

	if (vertexShader != nullptr) { delete vertexShader; vertexShader = nullptr;}
	if (pixelShader != nullptr) { delete pixelShader; pixelShader = nullptr;}
	if (skyPS != nullptr) { delete skyPS; skyPS = nullptr; }
	if (skyVS != nullptr) { delete skyVS; skyVS = nullptr; }
	if (instancePS != nullptr) { delete instancePS; instancePS = nullptr; }
	if (instanceVS != nullptr) { delete instanceVS; instanceVS = nullptr; }
	if (waterVS != nullptr) { delete waterVS; waterVS = nullptr; }
	if (waterPS != nullptr) { delete waterPS; waterPS = nullptr; }

	if (render != nullptr) { delete render; render = nullptr;}
	if (mainCam != nullptr) { delete mainCam; mainCam = nullptr;}

	if (defaultMat != nullptr) { delete defaultMat; defaultMat = nullptr; }
	if(redMat != nullptr) { delete redMat; redMat = nullptr; }
	if (greenMat != nullptr) { delete greenMat; greenMat = nullptr; }
	if (blueMat != nullptr) { delete blueMat; blueMat = nullptr; }
	if (purpleMat != nullptr) { delete purpleMat; purpleMat = nullptr; }
	if (lightBlueMat != nullptr) { delete lightBlueMat; lightBlueMat = nullptr; }
	if (yellowMat != nullptr) { delete yellowMat; yellowMat = nullptr; }
	if (greyMat != nullptr) { delete greyMat; greyMat = nullptr; }

	if (block != nullptr) { delete block; block = nullptr; }
	if (tetrisGame != nullptr) { delete tetrisGame; tetrisGame = nullptr; }
	if (cB != nullptr) { delete cB; cB = nullptr; }

	if (SVR != nullptr) { SVR->Release(); SVR = nullptr; }
	if (skyBoxSVR != nullptr) { skyBoxSVR->Release(); skyBoxSVR = nullptr; }
	if (sample != nullptr) { sample->Release(); sample = nullptr; }
	if (skyRast != nullptr) { skyRast->Release(); skyRast = nullptr; }
	if (skyDepth != nullptr) { skyDepth->Release(); skyDepth = nullptr; }

	if (ter != nullptr) { delete ter; ter = nullptr;};
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateBasicGeometry();

	skyBox = 1;

	noClick = true;
	newPosX = 0.0;
	newPosY = 0.0;

	light1.AmbientColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	light1.DiffuseColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	light1.Direction = XMFLOAT3(1.0f, -1.0f, 1.0f);

	light2.AmbientColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	light2.DiffuseColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	light2.Direction = XMFLOAT3(-1.0f, 0.3f, 1.0f);

	mainCam = new Camera(width, height);


	//Texture time boisssss
	SVR = 0;
	skyBoxSVR = 0;
	//CreateWICTextureFromFile(device, context, L"Textures/brick.jpg", 0, &SVR);
	CreateWICTextureFromFile(device, context, L"Textures/harambe.jpg", 0, &SVR);

	LoadSkyBox();

	D3D11_SAMPLER_DESC sDes = {};
	sDes.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; 
	sDes.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sDes.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sDes.Filter = D3D11_FILTER_ANISOTROPIC;
	sDes.MaxAnisotropy = 16;
	sDes.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&sDes,&sample);

	// Create a rasterizer state so we can render backfaces
	D3D11_RASTERIZER_DESC rsDesc = {};
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_FRONT;
	rsDesc.DepthClipEnable = true;
	device->CreateRasterizerState(&rsDesc, &skyRast);

	// Create a depth state so that we can accept pixels
	// at a depth less than or EQUAL TO an existing depth
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; // Make sure we can see the sky (at max depth)
	device->CreateDepthStencilState(&dsDesc, &skyDepth);

	//Wondering if i can make pointers to materials and switch the material whenever
	//I deed it :D
	defaultMat = new Material(*pixelShader, *vertexShader, XMFLOAT4(0.5f , 0.5f, 0.5f, 1.0f), *SVR, *sample);
	redMat = new Material(*pixelShader, *vertexShader, XMFLOAT4(8.0f, 0.0f, .0f, 1.0f), *SVR, *sample);
	greenMat = new Material(*pixelShader, *vertexShader, XMFLOAT4(0.0f, 8.0f, 0.0f, 1.0f), *SVR, *sample);
	blueMat = new Material(*pixelShader, *vertexShader, XMFLOAT4(0.0f, 0.0f, 8.0f, 1.0f), *SVR, *sample);
	purpleMat = new Material(*pixelShader, *vertexShader, XMFLOAT4(8.0f, 0.0f, 8.0f, 1.0f), *SVR, *sample);
	lightBlueMat = new Material(*pixelShader, *vertexShader, XMFLOAT4(0.0f, 0.0f, 6.0f, 1.0f), *SVR, *sample);
	yellowMat = new Material(*pixelShader, *vertexShader, XMFLOAT4(8.0f, 8.0f, 0.0f, 1.0f), *SVR, *sample);
	greyMat = new Material(*pixelShader, *vertexShader, XMFLOAT4(5.0f, 5.0f, 5.0f, 1.0f), *SVR, *sample);

	tetrisGame = new Tetris(*cube, GameEntity(*helix, "Square", *purpleMat), GameEntity(*helix, "Square", *purpleMat), *redMat, *blueMat, *greenMat, *purpleMat, *lightBlueMat, *yellowMat, *greyMat, 1, 1);

	//block = new TetrisBlock(*cube,*blueMat,2,2);
	//entities = block->GetEntities();

	currentState = false;
	previousState = false;

	ter = new Terrain(100, 100, *blueMat, *device);
	
	entities.push_back(GameEntity(*helix, "Square", *defaultMat));
	entities.push_back(GameEntity(*torus, "Square", *defaultMat));
	entities.push_back(GameEntity(*cylinder, "Pentagon", *defaultMat));
	entities.push_back(GameEntity(*sphere, "Star", *defaultMat));
	entities.push_back(GameEntity(*cube, "Star", *defaultMat));
	entities.push_back(ter->GetEntity());

	(entities)[0].LoadMaterial(*purpleMat);
	//(entities)[0].LoadMaterial(*redMat);
	//(entities)[0].LoadMaterial(*purpleMat);
	(entities)[1].LoadMaterial(*redMat);
	(entities)[2].LoadMaterial(*blueMat);
	(entities)[3].LoadMaterial(*greenMat);

	render = new Renderer(entities, *vertexShader, *pixelShader, *instanceVS, *instancePS, *waterVS, *waterPS,*tetrisGame, *device);
	render->board = (tetrisGame)->GetBoard();
	cB = new ConsoleBuddy(*tetrisGame, *render, *mainCam);

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(L"Debug/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"VertexShader.cso");		

	pixelShader = new SimplePixelShader(device, context);
	if(!pixelShader->LoadShaderFile(L"Debug/PixelShader.cso"))	
		pixelShader->LoadShaderFile(L"PixelShader.cso");

	skyVS = new SimpleVertexShader(device, context);
	if (!skyVS->LoadShaderFile(L"Debug/SkyVS.cso"))
		skyVS->LoadShaderFile(L"SkyVS.cso");

	skyPS = new SimplePixelShader(device, context);
	if (!skyPS->LoadShaderFile(L"Debug/SkyPS.cso"))
		skyPS->LoadShaderFile(L"SkyPS.cso");

	instanceVS = new SimpleVertexShader(device, context);
	if (!instanceVS->LoadShaderFile(L"Debug/InstanceVS.cso"))
		instanceVS->LoadShaderFile(L"InstanceVS.cso");

	instancePS = new SimplePixelShader(device, context);
	if (!instancePS->LoadShaderFile(L"Debug/InstancePS.cso"))
		instancePS->LoadShaderFile(L"InstancePS.cso");

	waterVS = new SimpleVertexShader(device, context);
	if (!waterVS->LoadShaderFile(L"Debug/WaterVS.cso"))
		waterVS->LoadShaderFile(L"WaterVS.cso");

	waterPS = new SimplePixelShader(device, context);
	if (!waterPS->LoadShaderFile(L"Debug/WaterPS.cso"))
		waterPS->LoadShaderFile(L"WaterPS.cso");

	// You'll notice that the code above attempts to load each
	// compiled shader file (.cso) from two different relative paths.

	// This is because the "working directory" (where relative paths begin)
	// will be different during the following two scenarios:
	//  - Debugging in VS: The "Project Directory" (where your .cpp files are) 
	//  - Run .exe directly: The "Output Directory" (where the .exe & .cso files are)

	// Checking both paths is the easiest way to ensure both 
	// scenarios work correctly, although others exist
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//   update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//   an identity matrix.  This is just to show that HLSL expects a different
	//   matrix (column major vs row major) than the DirectX Math library
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -8, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up  = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V   = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//   the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT3 normal	= XMFLOAT3(0.0f, 0.0f, -1.0f);
	XMFLOAT2 uv	= XMFLOAT2(0.0f, 0.0f);


	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)

	//Old Meshes
	/*
	//Square
	Vertex vertices1[] = 
	{
		{ XMFLOAT3(-1.5f, +1.0f, +0.0f), normal, uv},
		{ XMFLOAT3(+1.5f, +1.0f, +0.0f), normal, uv },
		{ XMFLOAT3(+1.5f, -1.0f, +0.0f), normal, uv },
		{ XMFLOAT3(-1.5f, -1.0f, +0.0f), normal, uv },
		
	};
	
	int indices1[] = { 0, 1, 2, 0, 2, 3 };

	//Pentagon
	Vertex vertices2[] =
	{
		{ XMFLOAT3(-1.0f, +1.0f, +0.0f), normal, uv },
		{ XMFLOAT3(+0.0f, +2.0f, +0.0f), normal, uv },
		{ XMFLOAT3(+1.0f, +1.0f, +0.0f), normal, uv },
		{ XMFLOAT3(+1.0f, -1.0f, +0.0f), normal, uv },
		{ XMFLOAT3(-1.0f, -1.0f, +0.0f), normal, uv },
		
	};

	int indices2[] = { 0, 1, 2, 0, 2, 3, 0, 3, 4 };

	//Star
	Vertex vertices3[] =
	{
		//Outsides Verts
		{ XMFLOAT3(-1.5f, +0.8f, +0.0f), normal, uv },  //Top Left
		{ XMFLOAT3(+0.0f, +2.0f, +0.0f), normal, uv }, //Top
		{ XMFLOAT3(+1.5f, +0.8f, +0.0f), normal, uv }, // Top Right
		{ XMFLOAT3(+1.15f, -1.4f, +0.0f), normal, uv }, // Bottom Right
		{ XMFLOAT3(-1.15f, -1.4f, +0.0f), normal, uv }, // Bottom Left
		//Insides Verts
		{ XMFLOAT3(-0.75f, -0.15f, +0.0f), normal, uv }, // Bottom Left
		{ XMFLOAT3(-0.50f, +0.80f, +0.0f), normal, uv }, //Top Left
		{ XMFLOAT3(+0.50f, +0.80f, +0.0f), normal, uv }, // Top Right
		{ XMFLOAT3(+0.75f, -0.15f, +0.0f), normal, uv }, // Bottom Right	
		{ XMFLOAT3(+0.0f, -0.80f, +0.0f), normal, uv }, // Bottom
	};

	int indices3[] = { 0,6,5, 6,1,7, 7,2,8, 8,3,9, 9,4,5, 5,8,9, 6,8,5, 6,7,8 };

	obj1 = new Mesh(vertices1, sizeof(vertices1), indices1, sizeof(indices1), *device);
	obj2 = new Mesh(vertices2, sizeof(vertices2), indices2, sizeof(indices2), *device);
	obj3 = new Mesh(vertices3, sizeof(vertices3), indices3, sizeof(indices3), *device);*/
	
	cone = new Mesh("Assets/Models/cone.obj", *device);
	cube = new Mesh("Assets/Models/cube.obj", *device);
	cylinder = new Mesh("Assets/Models/cylinder.obj", *device);
	helix = new Mesh("Assets/Models/helix.obj", *device);
	sphere = new Mesh("Assets/Models/sphere.obj", *device);
	torus = new Mesh("Assets/Models/torus.obj", *device);
	//game = new Mesh("Assets/Models/marte_isabel_game_obj.obj", *device);
	//over = new Mesh("Assets/Models/marte_isabel_game_obj.obj", *device);

}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();
	mainCam->SetProject(width, height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	mainCam->Update(deltaTime);
	cB->UpdateCB(deltaTime);

	// Testing entities
	{
		//For Testing purposes
		/*
		if (GetAsyncKeyState(VK_UP))
		{
			currentState = true;
			if (currentState != previousState) {
				block->rot += 1;
				block->LoadTetrisBlock();
			}
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			currentState = true;
			if (currentState != previousState) {
				block->type += 1;
				block->LoadTetrisBlock();
			}
		}*/

		if (GetAsyncKeyState(VK_UP))
		{
			(entities)[3].Translate(+0.0f, +2.0f * deltaTime, +0.0f);
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			(entities)[3].Translate(+2.0f * deltaTime, +0.0f, +0.0f);
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			(entities)[3].Translate(+0.0f, -2.0f * deltaTime, +0.0f);
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			(entities)[3].Translate(-2.0f * deltaTime, +0.0, +0.0f);
		}
		if (GetAsyncKeyState(VK_TAB))
		{
			currentState = true;
			if (currentState != previousState)
			{
				skyBox += 1;
				if (skyBox > 3)
				{
					skyBox = 1;
				}
				LoadSkyBox();
			}
		}


		(entities)[0].Translate(sin(totalTime * 2)* deltaTime*5.0f, 0.0f, 0.0f);
		(entities)[1].Translate(cos(totalTime)* deltaTime * 2, sin(totalTime)* deltaTime*2.0f, 0.0f);
		(entities)[2].Translate(0.0f, cos(totalTime)* deltaTime, 0.0f);
		//(entities)[3].Scale(cos(totalTime)* deltaTime * 2, sin(totalTime)* deltaTime*2.0f, 0.0f);
	}

	//Tetris Input Stuff (Start, End, Reset)
	{
	if (tetrisGame->gameStart) {
		if (GetAsyncKeyState(VK_LCONTROL) && GetAsyncKeyState('C'))
		{
			mainCam->SetTetrisCamera();
		}
		(tetrisGame)->UpdateGame(deltaTime, totalTime);
		if (tetrisGame->tChange)
		{
			render->tBlocks = (tetrisGame)->GetTBlocks();
			render->LoadAllBlocks();
			tetrisGame->tChange = false;
		}
		if (tetrisGame->pChange)
		{
			render->pBlocks = (tetrisGame)->GetPBlocks();
			render->LoadAllBlocks();
			tetrisGame->pChange = false;
		}

		// End Game
		if (GetAsyncKeyState('U'))
		{
			(tetrisGame)->EndGame();
			(tetrisGame)->gameStart = false;
			cB->playerStatus = 2;
		}
		// Reset Game
		if (GetAsyncKeyState('I'))
		{
			(tetrisGame)->EndGame();
			(tetrisGame)->StartGame(30, 10);
		}
	}
	else
	{
		// Start game
		if (GetAsyncKeyState('Y') || cB->playerStatus == 1)
		{

			(tetrisGame)->StartGame(30, 10);
			render->LoadAllBlocks();
			render->freeRoam = true;
			mainCam->SetTetrisCamera();
			cB->playerStatus = 1;
		}
	}
}


	previousState = currentState;
	//entities.clear();
	//entities = block->GetEntities();
	currentState = false;
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	render->RenderUpdate(context, *mainCam, light1, light2);

	ID3D11Buffer* skyVB = (entities)[4].obj->GetVertexBuffer();
	ID3D11Buffer* skyIB = (entities)[4].obj->GetIndexBuffer();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &skyVB, &stride, &offset);
	context->IASetIndexBuffer(skyIB, DXGI_FORMAT_R32_UINT, 0);

	skyVS->SetMatrix4x4("view", mainCam->GetView());
	skyVS->SetMatrix4x4("projection", mainCam->GetProject());
	skyVS->CopyAllBufferData();
	skyVS->SetShader();

	skyPS->SetShaderResourceView("Sky", skyBoxSVR);
	skyPS->SetSamplerState("Sampler", sample);
	skyPS->CopyAllBufferData();
	skyPS->SetShader();

	context->RSSetState(skyRast);
	context->OMSetDepthStencilState(skyDepth, 0);

	context->DrawIndexed((entities)[4].obj->GetIndexCount(), 0, 0);

	context->RSSetState(0);
	context->OMSetDepthStencilState(0,0);

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}
void Game::LoadSkyBox()
{
	if (skyBoxSVR != nullptr) { skyBoxSVR->Release(); skyBoxSVR = nullptr; }
	switch(skyBox)
	{
	case 1:
	{
		CreateDDSTextureFromFile(device, L"Textures/SunnyCubeMap.dds", 0, &skyBoxSVR);
		//CreateDDSTextureFromFile(device, L"Textures/Mars.dds", 0, &skyBoxSVR);
		break;
	}
	case 2:
	{
		CreateDDSTextureFromFile(device, L"Textures/Mars.dds", 0, &skyBoxSVR);
		break;
	}
	case 3:
	{
		CreateDDSTextureFromFile(device, L"Textures/space.dds", 0, &skyBoxSVR);
		break;
	}
	}
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Save the previous mouse position, so we have it for the future
	mainCam->IsClicking(buttonState, x, y);

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	mainCam->UpdateNewPos(buttonState, x, y);

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y, float deltaTime)
{
	// Save the previous mouse position, so we have it for the future
	mainCam->OnMouseMove(buttonState, x, y, deltaTime);

}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y, float deltaTime)
{
	mainCam->CheckScrolling(wheelDelta, x, y, deltaTime);
}
#pragma endregion