// Application.h
#ifndef _LAB1_H
#define _LAB1_H

// Includes
#include "baseapplication.h"

#include "RenderTexture.h"
#include "Model.h"
#include "cubemesh.h"
#include "planemesh.h"
#include "spheremesh.h"
#include "orthomesh.h"
#include "TessellationMesh.h"
#include "PointMesh.h"
#include "ColourShader.h"
#include "TextureShader.h"
#include "LightShader.h"
#include "DepthShader.h"
#include "ManiTessShader.h"
#include "GeometryShader.h"
#include "ShadowShader.h"


class Lab1 : public BaseApplication
{
public:

	Lab1(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~Lab1();

	bool Frame();

private:
	void RenderToTexture();
	void RenderToTexture2();
	void RenderScene();
	bool Render();
	void input();

private:
	TextureShader* m_TextureShader;
	DepthShader* m_DepthShader;
	ShadowShader* m_ShadowShader;
	ManiTessShader* m_ManiTessShader;
	GeometryShader* m_GeoShader;

	Model* m_model;
	PlaneMesh* m_PlaneMesh;
	SphereMesh* m_SphereMesh;
	TessellationMesh* m_TessMesh;
	PointMesh* m_PointMesh;

	Light* m_Light;
	Light* m_Light2;
	RenderTexture* m_RenderTexture;
	RenderTexture* m_RenderTexture2;
	OrthoMesh* m_OrthoMesh;
	OrthoMesh* m_MiniOrthoMesh;//might be alright to just get rid of this stuff

	float Height;
	float Freq;
	float ourTime;//passing the varied time through
	Timer* m_timer2;//made own timer
	int TessAmount;

};

#endif