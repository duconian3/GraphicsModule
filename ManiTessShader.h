// Light shader.h
// Basic single light shader setup
#ifndef _MANITESSSHADER_H_
#define _MANITESSSHADER_H_

#include "BaseShader.h"

using namespace std;
using namespace DirectX;


class ManiTessShader : public BaseShader
{
private:
	struct TessellationBufferType
	{
		float tessellationFactor = 2.0f;
		XMFLOAT3 padding;
	};

	struct TimeBufferType
	{
		float time;
		float height;
		float frequency;
		float padding;
	};

public:

	ManiTessShader(ID3D11Device* device, HWND hwnd);
	~ManiTessShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, float tessellationFactor, float dt, float height, float frequency);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR* vsFilename, WCHAR* psFilename);
	void InitShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_tessellationBuffer;
	ID3D11Buffer* m_timeBuffer;
};

#endif