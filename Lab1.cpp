// Lab1.cpp
#include "lab1.h"

Lab1::Lab1(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in) : BaseApplication(hinstance, hwnd, screenWidth, screenHeight, in)
{
	//manually set to show slight variation at close up with reference to the depth shader
	const float SCREEN_DEPTH = 100;
	const float SCREEN_NEAR = 0.1;
	const int SHADOWMAP_WIDTH = 1024;
	const int SHADOWMAP_HEIGHT = 1024;
	
	// Create Mesh objects
	m_PlaneMesh = new PlaneMesh(m_Direct3D->GetDevice(), L"../res/perlin.bmp");
	m_SphereMesh = new SphereMesh(m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.png");
	m_TessMesh = new TessellationMesh(m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.png");
	m_model = new Model(m_Direct3D->GetDevice(), L"../res/perlin.bmp", L"../res/Wolf.obj");//testing shadows by placing an irregularly shaped model in our scene
	m_PointMesh = new PointMesh(m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.png");

	//initializing shader data
	m_ManiTessShader = new ManiTessShader(m_Direct3D->GetDevice(), hwnd);
	m_DepthShader = new DepthShader(m_Direct3D->GetDevice(), hwnd);
	m_ShadowShader = new ShadowShader(m_Direct3D->GetDevice(), hwnd);
	m_GeoShader = new GeometryShader(m_Direct3D->GetDevice(), hwnd);

	m_Light = new Light;
	m_Light->SetPosition(-10.0, 10.0, -10.0);
	m_Light->SetLookAt(0.0, 0.0, 0.0);
	m_Light->SetAmbientColour(0.1f, 0.1f, 0.1f, 1.0f);
	m_Light->SetDiffuseColour(1.0f, 0.0f, 0.0f, 1.0f);
	m_Light->GenerateProjectionMatrix(SCREEN_NEAR, SCREEN_DEPTH);

	m_Light2 = new Light;
	m_Light2->SetPosition(10.0, 10.0, -20.0);
	m_Light2->SetLookAt(20.0, 20.0, 20.0);
	m_Light2->SetAmbientColour(0.1f, 0.1f, 0.1f, 1.0f);
	m_Light2->SetDiffuseColour(0.0f, 1.0f, 0.0f, 1.0f);
	m_Light2->GenerateProjectionMatrix(SCREEN_NEAR, SCREEN_DEPTH);
    
	// RenderTexture, OrthoMesh and shader set for different renderTarget
	m_RenderTexture = new RenderTexture(m_Direct3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);//light1
	m_RenderTexture2 = new RenderTexture(m_Direct3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);//light2

	m_OrthoMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth, screenHeight, 0, 0);
	m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);

	//initializing variables for both manipulation and tessellation
	ourTime = 0;
	m_timer2 = new Timer;
	TessAmount = 10;
	Freq = 1.0;
	Height = 1.0;
}


Lab1::~Lab1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (m_PlaneMesh)
	{
		delete m_PlaneMesh;
		m_PlaneMesh = 0;
	}
	if (m_model)
	{
		delete m_model;
		m_model = 0;
	}
	if (m_SphereMesh)
	{
		delete m_SphereMesh;
		m_SphereMesh = 0;
	}
	if (m_TessMesh)
	{
		delete m_TessMesh;
		m_TessMesh = 0;
	}
	if (m_PointMesh)
	{
		delete m_PointMesh;
		m_PointMesh = 0;
	}

	if (m_DepthShader)
	{
		delete m_DepthShader;
		m_DepthShader = 0;
	}
	if (m_ShadowShader)
	{
		delete m_ShadowShader;
		m_ShadowShader = 0;
	}
	if (m_GeoShader)
	{
		delete m_GeoShader;
		m_GeoShader = 0;
	}
	
}


bool Lab1::Frame()
{
	bool result;

	result = BaseApplication::Frame();
	if (!result)
	{
		return false;
	}

	m_timer2->Frame();

	ourTime += m_timer2->GetTime()*4.0f;//deltaTime

	// Render the graphics.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

//Implementing the depth of the scene from the camera view for the first light [red]
void Lab1::RenderToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix;

	m_Light->GenerateViewMatrix();
	lightViewMatrix = m_Light->GetViewMatrix();
	m_Light->GenerateProjectionMatrix(1.0f, 100.0f);
	lightProjectionMatrix = m_Light->GetProjectionMatrix();
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture. //Also background colour
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());

	worldMatrix *= XMMatrixTranslation(4,2,4);
	worldMatrix *= XMMatrixScaling(3, 3, 3);
	m_TessMesh->SendData(m_Direct3D->GetDeviceContext());
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_TessMesh->GetIndexCount());
	m_Direct3D->GetWorldMatrix(worldMatrix);

	worldMatrix *= XMMatrixTranslation(13, 2, 13);
	worldMatrix *= XMMatrixScaling(3, 3, 3);
	m_SphereMesh->SendData(m_Direct3D->GetDeviceContext());
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount());
	m_Direct3D->GetWorldMatrix(worldMatrix);
	
	worldMatrix *= XMMatrixTranslation(10, 0, 10);
	worldMatrix *= XMMatrixScaling(2, 2, 2);
	m_model->SendData(m_Direct3D->GetDeviceContext());
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_model->GetIndexCount());
	m_Direct3D->GetWorldMatrix(worldMatrix);

	worldMatrix *= XMMatrixTranslation(8, 1, 15);
	worldMatrix *= XMMatrixScaling(2, 2, 2);
	m_PointMesh->SendData(m_Direct3D->GetDeviceContext());
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_PointMesh->GetIndexCount());
	m_Direct3D->GetWorldMatrix(worldMatrix);
	
	// Present the rendered scene to the screen.
	m_Direct3D->SetBackBufferRenderTarget();
}

//Implementing the depth of the scene from the camera view for the second light [green]
void Lab1::RenderToTexture2()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix2, lightProjectionMatrix2;

	m_Light2->GenerateViewMatrix();
	lightViewMatrix2 = m_Light2->GetViewMatrix();
	m_Light2->GenerateProjectionMatrix(1.0f, 100.0f);
	lightProjectionMatrix2 = m_Light2->GetProjectionMatrix();
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Set the render target to be the render to texture.
	m_RenderTexture2->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture. //Also background colour
	m_RenderTexture2->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.39f, 0.58f, 0.92f, 1.0f);

	//Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, lightViewMatrix2, lightProjectionMatrix2);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());

	worldMatrix *= XMMatrixTranslation(4, 2, 4);
	worldMatrix *= XMMatrixScaling(3, 3, 3);
	m_TessMesh->SendData(m_Direct3D->GetDeviceContext());
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, lightViewMatrix2, lightProjectionMatrix2);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_TessMesh->GetIndexCount());
	m_Direct3D->GetWorldMatrix(worldMatrix);

	worldMatrix *= XMMatrixTranslation(13, 2, 13);
	worldMatrix *= XMMatrixScaling(3, 3, 3);
	m_SphereMesh->SendData(m_Direct3D->GetDeviceContext());
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, lightViewMatrix2, lightProjectionMatrix2);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount());
	m_Direct3D->GetWorldMatrix(worldMatrix);

	worldMatrix *= XMMatrixTranslation(10, 0, 10);
	worldMatrix *= XMMatrixScaling(2, 2, 2);
	m_model->SendData(m_Direct3D->GetDeviceContext());
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, lightViewMatrix2, lightProjectionMatrix2);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_model->GetIndexCount());
	m_Direct3D->GetWorldMatrix(worldMatrix);

	worldMatrix *= XMMatrixTranslation(8, 1, 15);
	worldMatrix *= XMMatrixScaling(2, 2, 2);
	m_PointMesh->SendData(m_Direct3D->GetDeviceContext());
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, lightViewMatrix2, lightProjectionMatrix2);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_PointMesh->GetIndexCount());
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Present the rendered scene to the screen.
	m_Direct3D->SetBackBufferRenderTarget();
}

//Renders the scene with shadows, making use of the depth calculations done previously
void Lab1::RenderScene()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix, lightViewMatrix, lightProjectionMatrix, lightViewMatrix2, lightProjectionMatrix2;

	// Clear the scene. (default blue colour)
	m_Direct3D->BeginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	if (wireframe == true)
	{
		m_Direct3D->TurnOnWireframe();
	}

	worldMatrix *= XMMatrixTranslation(8, 1, 15);
	worldMatrix *= XMMatrixScaling(2, 2, 2);
	m_PointMesh->SendData(m_Direct3D->GetDeviceContext());
	m_GeoShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_PointMesh->GetTexture());
	m_GeoShader->Render(m_Direct3D->GetDeviceContext(), m_PointMesh->GetIndexCount());
	m_ShadowShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix, m_PointMesh->GetTexture(), m_RenderTexture->GetShaderResourceView(), m_Light, lightViewMatrix2, lightProjectionMatrix2, m_RenderTexture2->GetShaderResourceView(), m_Light2);
	m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_PointMesh->GetIndexCount());
	m_Direct3D->GetWorldMatrix(worldMatrix);

	worldMatrix *= XMMatrixTranslation(10, 0, 10);
	worldMatrix*=XMMatrixScaling(2,2,2);
	m_model->SendData(m_Direct3D->GetDeviceContext());
	m_ShadowShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix, m_model->GetTexture(), m_RenderTexture->GetShaderResourceView(), m_Light, lightViewMatrix2, lightProjectionMatrix2, m_RenderTexture2->GetShaderResourceView(), m_Light2);
	m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_model->GetIndexCount());
	m_Direct3D->GetWorldMatrix(worldMatrix);

	worldMatrix *= XMMatrixTranslation(13, 2, 13);
	worldMatrix *= XMMatrixScaling(3, 3, 3);
	m_SphereMesh->SendData(m_Direct3D->GetDeviceContext());
	m_ShadowShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix, m_SphereMesh->GetTexture(), m_RenderTexture->GetShaderResourceView(), m_Light, lightViewMatrix2, lightProjectionMatrix2, m_RenderTexture2->GetShaderResourceView(), m_Light2);
	m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount());
	m_Direct3D->GetWorldMatrix(worldMatrix);

	m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());
	m_ShadowShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix, m_PlaneMesh->GetTexture(), m_RenderTexture->GetShaderResourceView(), m_Light, lightViewMatrix2, lightProjectionMatrix2, m_RenderTexture2->GetShaderResourceView(), m_Light2);
	m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());

	//tessellation and manipulation affecting a plane, with shadows that are only affecting stationary vertices
	worldMatrix *= XMMatrixTranslation(4, 2, 4);
	worldMatrix *= XMMatrixScaling(3, 3, 3);
	m_TessMesh->SendData(m_Direct3D->GetDeviceContext());
	m_ManiTessShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_TessMesh->GetTexture(), TessAmount, ourTime, Height, Freq);
	m_ManiTessShader->Render(m_Direct3D->GetDeviceContext(), m_TessMesh->GetIndexCount());
	m_ShadowShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix, m_TessMesh->GetTexture(), m_RenderTexture->GetShaderResourceView(), m_Light, lightViewMatrix2, lightProjectionMatrix2, m_RenderTexture2->GetShaderResourceView(), m_Light2);
	m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_TessMesh->GetIndexCount());
	m_Direct3D->GetWorldMatrix(worldMatrix);

	if (wireframe == true)
	{
		m_Direct3D->TurnOffWireframe();
	}

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();
}

bool Lab1::Render()
{
	RenderToTexture();
	RenderToTexture2();
	RenderScene();
	input();
	return true;
}

void Lab1::input()
{
	if (m_Input->isKeyDown('Z') && TessAmount > 1)
	{
		TessAmount--;
		m_Input->SetKeyUp('Z');
	}
	if (m_Input->isKeyDown('X') && TessAmount < 32)
	{
		TessAmount++;
		m_Input->SetKeyUp('X');
	}
	if (m_Input->isKeyDown('H'))
	{
		Height++;
		m_Input->SetKeyUp('H');
	}
	if (m_Input->isKeyDown('N'))
	{
		Height--;
		m_Input->SetKeyUp('N');
	}
	if (m_Input->isKeyDown('J'))
	{
		Freq++;
		m_Input->SetKeyUp('J');
	}
	if (m_Input->isKeyDown('M'))
	{
		Freq--;
		m_Input->SetKeyUp('M');
	}
	if (m_Input->isKeyDown(VK_SPACE))
	{
		if (wireframe == false)
		{
			wireframe = true;
			m_Input->SetKeyUp(VK_SPACE);
		}
		else
		{
			wireframe = false;
			m_Input->SetKeyUp(VK_SPACE);
		}
	}
}
