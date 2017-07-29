//copied from Light shader.h
// Basic single light shader setup
#ifndef _MANISHADER_H_
#define _MANISHADER_H_

#include "BaseShader.h"
#include "light.h"
#include "Timer.h"

using namespace std;
using namespace DirectX;


class ManiShader : public BaseShader
{
private:
	struct ManiBufferType
	{
		XMFLOAT4 diffuse;
		XMFLOAT4 ambient;
		XMFLOAT4 direction;
	};

	struct TimeBufferType
	{
		float time;
		float padding;
		float frequency;
		float height;
	};

public:

	ManiShader(ID3D11Device* device, HWND hwnd);
	~ManiShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Light* light, float deltaTime, float height, float frequency);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_ManiBuffer;
	ID3D11Buffer* m_TimeBuffer;
};

#endif