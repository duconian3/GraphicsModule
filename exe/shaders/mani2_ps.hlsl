// copied from Light pixel shader
//Vertex Manipulation pixel shader
// Calculate ambient and diffuse lighting for a single light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer ManiBuffer : register(cb0)
{
	float4 diffuseColour;
	float4 ambientColour;
	float4 ManiDirection;
};

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
	float3 ManiDir;
	float ManiIntensity;
	float4 ambientColour = float4 (0.0f, 0.0f, 0.0f, 0.2f);

		float4 colour = ambientColour;

		// Sample the pixel color from the texture using the sampler at this texture coordinate location.
		textureColour = shaderTexture.Sample(SampleType, input.tex);


	// Invert the light direction for calculations.
	ManiDir = normalize(input.position3D - ManiDirection);

	// Calculate the amount of light on this pixel.
	ManiIntensity = saturate(dot(input.normal, -ManiDir));

	if (ManiIntensity >= 0)
	{
		// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
		colour += saturate(diffuseColour * ManiIntensity);
	}

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
	//can send boolean as a floag and then ask if flag 1 or 0 for a for loop
	colour = colour * textureColour;
	return colour;
}

