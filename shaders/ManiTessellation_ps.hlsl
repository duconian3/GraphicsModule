// Tessellation pixel shader
// Output colour passed to stage.

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 position3D : TEXCOORD2;
};

float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;
	float4 ambientColour = float4(0.2, 0.2, 0.2, 1.0);
	float4 colour = ambientColour;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColour = shaderTexture.Sample(SampleType, input.tex);

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
	colour = colour * textureColour;

	return colour;
}