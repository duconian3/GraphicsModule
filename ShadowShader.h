// Light shader.h
// Basic single light shader setup
#ifndef _SHADOWSHADER_H_
#define _SHADOWSHADER_H_

#include "BaseShader.h"
#include "light.h"

using namespace std;
using namespace DirectX;


class ShadowShader : public BaseShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightView;
		XMMATRIX lightProjection;
		XMMATRIX lightView2;
		XMMATRIX lightProjection2;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT4 diffuse2;
	};

	struct LightBufferType2
	{
		XMFLOAT3 position;
		float padding;
		XMFLOAT3 position2;
		float padding2;
	};

public:

	ShadowShader(ID3D11Device* device, HWND hwnd);
	~ShadowShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, const XMMATRIX &lightView, const XMMATRIX &lightProjection,
		ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView*depthMap, Light* light, const XMMATRIX &lightView2, const XMMATRIX &lightProjection2, ID3D11ShaderResourceView*depthMap2, Light* light2);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11SamplerState* m_sampleStateClamp;
	ID3D11Buffer* m_LightBuffer;
	ID3D11Buffer* m_LightBuffer2;

};

#endif