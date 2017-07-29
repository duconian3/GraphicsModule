// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices

cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer TimeBufferType : register(cb1)
{
	float time;
	float height;
	float frequency;
	float padding;
};

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct InputType
{
    float3 position : POSITION;
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

[domain("quad")]

OutputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
    float3 vertexPosition;
	float3 normalPosition;
	float2 texPosition;
    OutputType output;

    // Determine the position of the new vertex.
	// Invert the y and Z components of uvwCoord as these coords are generated in UV space and therefore y is positive downward.
	// Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).
	float3 v1 = lerp(patch[0].position, patch[1].position, 1 - uvwCoord.y);
	float3 v2 = lerp(patch[2].position, patch[3].position, 1 - uvwCoord.y);
	float3 n1 = lerp(patch[0].normal, patch[1].normal, 1 - uvwCoord.y);
	float3 n2 = lerp(patch[2].normal, patch[3].normal, 1 - uvwCoord.y);
	float2 t1 = lerp(patch[0].tex, patch[1].tex, 1 - uvwCoord.y);
	float2 t2 = lerp(patch[2].tex, patch[3].tex, 1 - uvwCoord.y);
	

	vertexPosition = lerp(v1, v2, uvwCoord.x);
	normalPosition = lerp(n1, n2, uvwCoord.x);
	texPosition = lerp(t1, t2, uvwCoord.x);
    
	//offset position based on sine wave: divide position by frequency before adding time
	vertexPosition.z += height *sin(vertexPosition.x / frequency + time);
	vertexPosition.z += height *cos(vertexPosition.y / frequency + time);

	//modify normals
	normalPosition.x = 1 - cos(vertexPosition.x + time);
	normalPosition.y = abs(cos(vertexPosition.x + time));

    // Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = texPosition;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(normalPosition, (float3x3)worldMatrix);

	// Normalize the normal vector.
	output.normal = normalize(output.normal);

	output.position3D = mul(vertexPosition, worldMatrix);

    return output;
}