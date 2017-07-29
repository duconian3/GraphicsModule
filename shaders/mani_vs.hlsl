// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader

cbuffer MatrixBuffer : register(cb0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer TimeBufferType : register(cb1)
{
	float time;
	float padding;
	float height;
	float frequency;
};

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 position3D : TEXCOORD2;
};


OutputType main(InputType input)
{
	OutputType output;
	float4 worldPosition;


	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	//offset position based on sine wave
	input.position.y += height *sin(input.position.x + time);
	input.position.z += frequency *sin(input.position.x + time);

	//modify normals
	input.normal.x = 1 - cos(input.position.x + time);
	input.normal.y = abs(cos(input.position.x + time));

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalize the normal vector.
	output.normal = normalize(output.normal);

	// world position of vertex
	output.position3D = mul(input.position, worldMatrix);

	return output;
}