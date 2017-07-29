// Tessellation Hull Shader
// Prepares control points for tessellation

cbuffer TessellationBuffer : register(cb)
{
	float tessellationFactor;
	float3 padding;
};

struct InputType
{
	float3 position : POSITION;
	float4 colour : COLOR;
};

struct ConstantOutputType
{
	float edges[4] : SV_TessFactor;
	float inside[2] : SV_InsideTessFactor;
};

struct OutputType
{
	float3 position : POSITION;
	float4 colour : COLOR;
};

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 3> inputPatch, uint patchId : SV_PrimitiveID)
{
	ConstantOutputType output;

	// Set the tessellation factors for the three edges of the triangle.
	output.edges[0] = tessellationFactor;
	output.edges[1] = tessellationFactor;
	output.edges[2] = tessellationFactor; //hard code for uneven tessellation here [e.g. 1,2,3]
	output.edges[3] = tessellationFactor;

	// Set the tessellation factor for tessallating inside the triangle.
	output.inside[0] = tessellationFactor;
	output.inside[1] = tessellationFactor;

	return output;
}

//partition types
//Integer[1 - 64]
//fractional_even[2 - 64]
//fractional_odd[1 - 63]


[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
	OutputType output;

	// Set the position for this control point as the output position.
	output.position = patch[pointId].position;

	// Set the input color as the output color.
	output.colour = patch[pointId].colour;

	return output;
}