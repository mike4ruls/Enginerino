#include "Game.h"
#include "Vertex.h"
#include <math.h>

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
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	//vertexBuffer = 0;
	//indexBuffer = 0;
	vertexShader = 0;
	pixelShader = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
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

	if (render != nullptr) { delete render; render = nullptr;}
	if (mainCam != nullptr) { delete mainCam; mainCam = nullptr;}

	if (defaultMat != nullptr) { delete defaultMat; defaultMat = nullptr; }
	if(redMat != nullptr) { delete redMat; redMat = nullptr; }
	if (greenMat != nullptr) { delete greenMat; greenMat = nullptr; }
	if (blueMat != nullptr) { delete blueMat; blueMat = nullptr; }
	if (purpleMat != nullptr) { delete purpleMat; purpleMat = nullptr; }
	if (block != nullptr) { delete block; block = nullptr; }
	if (tetrisGame != nullptr) { delete tetrisGame; tetrisGame = nullptr; }
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
	//CreateMatrices();
	CreateBasicGeometry();

	noClick = true;
	newPosX = 0.0;
	newPosY = 0.0;

	light1.AmbientColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);//{ (0.1 , 0.1, 0.1,1.0),(0,0,1,1),(1,-1,0) };
	light1.DiffuseColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	light1.Direction = XMFLOAT3(1.0f, -1.0f, 1.0f);

	light2.AmbientColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);//{ (0.1 , 0.1, 0.1,1.0),(0,0,1,1),(1,-1,0) };
	light2.DiffuseColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	light2.Direction = XMFLOAT3(-1.0f, 0.3f, 1.0f);

	mainCam = new Camera(width, height);


	//Texture time boisssss
	//DirectX::CreateWICTextureFromFile(device,context,"",0,);
	//SVR = new SimpleSRV();
	//CreateWICTextureFromFile(*device, *context, L"Tetures/brick.jpg",0,);

	//Wondering if i can make pointers to materials and switch the material whenever
	//I deed it :D
	defaultMat = new Material(*pixelShader, *vertexShader, XMFLOAT4(0.5f , 0.5f, 0.5f, 1.0f));
	redMat = new Material(*pixelShader, *vertexShader, XMFLOAT4(1.0f, 0.0f, .0f, 1.0f));
	greenMat = new Material(*pixelShader, *vertexShader, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	blueMat = new Material(*pixelShader, *vertexShader, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
	purpleMat = new Material(*pixelShader, *vertexShader, XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f));

	tetrisGame = new Tetris(*cube, *redMat, *blueMat, *greenMat, *purpleMat);

	block = new TetrisBlock(*cube,*blueMat,2,2);

	entities = block->GetEntities();

	currentState = false;
	previousState = false;

	/*
	entities.push_back(GameEntity(*helix, "Square", *defaultMat));
	entities.push_back(GameEntity(*torus, "Square", *defaultMat));
	entities.push_back(GameEntity(*cylinder, "Pentagon", *defaultMat));
	entities.push_back(GameEntity(*sphere, "Star", *defaultMat));
	entities.push_back(GameEntity(*cube, "Star", *defaultMat));

	(entities)[0].LoadMaterial(*purpleMat);
	//(entities)[0].LoadMaterial(*redMat);
	//(entities)[0].LoadMaterial(*purpleMat);
	(entities)[1].LoadMaterial(*redMat);
	(entities)[2].LoadMaterial(*blueMat);
	(entities)[3].LoadMaterial(*greenMat);*/

	render = new Renderer(entities,*tetrisGame, *vertexShader, *pixelShader);
	(tetrisGame)->StartGame(30, 10);
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
	obj3 = new Mesh(vertices3, sizeof(vertices3), indices3, sizeof(indices3), *device);
	
	cone = new Mesh("Assets/Models/cone.obj", *device);
	cube = new Mesh("Assets/Models/cube.obj", *device);
	cylinder = new Mesh("Assets/Models/cylinder.obj", *device);
	helix = new Mesh("Assets/Models/helix.obj", *device);
	sphere = new Mesh("Assets/Models/sphere.obj", *device);
	torus = new Mesh("Assets/Models/torus.obj", *device);

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

	/*
	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	//  Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!*/
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	mainCam->Update(deltaTime);
	
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
	}
	/*
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

	(entities)[0].Translate(sin(totalTime*2)* deltaTime*5.0f,0.0f,0.0f);
	(entities)[1].Translate(cos(totalTime)* deltaTime*2, sin(totalTime)* deltaTime*2.0f, 0.0f);
	(entities)[2].Translate(0.0f, cos(totalTime)* deltaTime, 0.0f);*/
	
	previousState = currentState;
	entities.clear();
	entities = block->GetEntities();
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

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{

	
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future

	mainCam->IsClicking(buttonState, x, y);

	/*
		if (noClick) {
			prevMousePos.x = x;
			prevMousePos.y = y;
			noClick = false;
		}*/

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
	// Add any custom code here...
	mainCam->UpdateNewPos(buttonState, x, y);
	/*
	noClick = true;
	newPosX += distX;
	newPosY += distY;*/

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
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future

	mainCam->OnMouseMove(buttonState, x, y, deltaTime);
	
	/*
	if (noClick) {
		prevMousePos.x = x;
		prevMousePos.y = y;
	}
	else
	{
		distX = (x - prevMousePos.x)*-0.01f;
		distY = (y - prevMousePos.y)*0.01f;

		printf("X dist:%f, Y dist:%f\n", distX,distY);
		
		XMVECTOR pos = XMVectorSet((newPosX + distX), (newPosY + distY), viewMatrix._34 * -1, 0);
		XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
		XMVECTOR up = XMVectorSet(0, 1, 0, 0);
		XMMATRIX V = XMMatrixLookToLH(
			pos,     // The position of the "camera"
			dir,     // Direction the camera is looking
			up);     // "Up" direction in 3D space (prevents roll)
		XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));
	}*/
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y, float deltaTime)
{
	mainCam->CheckScrolling(wheelDelta, x, y, deltaTime);

	/*
	if(wheelDelta <0 )
	{
		XMVECTOR pos = XMVectorSet(viewMatrix._14 * -1.0f, viewMatrix._24 * -1.0f, (viewMatrix._34 + 0.1f) * -1.0f, 0.0f);
		XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
		XMVECTOR up = XMVectorSet(0, 1, 0, 0);
		XMMATRIX V = XMMatrixLookToLH(
			pos,     // The position of the "camera"
			dir,     // Direction the camera is looking
			up);     // "Up" direction in 3D space (prevents roll)
		XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));
	}
	else if(wheelDelta >0)
	{
		XMVECTOR pos = XMVectorSet(viewMatrix._14 * -1.0f, viewMatrix._24 * -1.0f, (viewMatrix._34 - 0.1f) * -1.0f, 0.0f);
		XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
		XMVECTOR up = XMVectorSet(0, 1, 0, 0);
		XMMATRIX V = XMMatrixLookToLH(
			pos,     // The position of the "camera"
			dir,     // Direction the camera is looking
			up);     // "Up" direction in 3D space (prevents roll)
		XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));
	}
	else{}*/

}
#pragma endregion