
cbuffer MatrixBuffer : register(cb0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix lightViewMatrix;
	matrix lightProjectionMatrix;
	matrix lightViewMatrix2;
	matrix lightProjectionMatrix2;
};

cbuffer LightBuffer2 : register(cb1)
{
    float3 lightPosition;
	float padding;
	float3 lightPosition2;
	float padding2;
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
    float4 lightViewPosition : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
	float4 lightViewPosition2 : TEXCOORD3;
	float3 lightPos2 : TEXCOORD4;
};


OutputType main(InputType input)
{
    OutputType output;
	float4 worldPosition;
        
	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Calculate the position of the vertice as viewed by the light source.
    output.lightViewPosition = mul(input.position, worldMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);

	output.lightViewPosition2 = mul(input.position, worldMatrix);
	output.lightViewPosition2 = mul(output.lightViewPosition2, lightViewMatrix2);
	output.lightViewPosition2 = mul(output.lightViewPosition2, lightProjectionMatrix2);

	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    // Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

    // Determine the light position based on the position of the light and the position of the vertex in the world.
    output.lightPos = lightPosition.xyz - worldPosition.xyz;
    // Normalize the light position vector.
    output.lightPos = normalize(output.lightPos);

	output.lightPos2 = lightPosition2.xyz - worldPosition.xyz;
	output.lightPos2 = normalize(output.lightPos2);

	return output;
}