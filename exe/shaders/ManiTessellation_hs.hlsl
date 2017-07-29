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
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct OutputType
{
    float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 3> inputPatch, uint patchId : SV_PrimitiveID)
{    
    ConstantOutputType output;


    // Set the tessellation factors for the four edges of the quad.
    output.edges[0] = tessellationFactor;
    output.edges[1] = tessellationFactor;
    output.edges[2] = tessellationFactor;
	output.edges[3] = tessellationFactor;


    // Set the tessellation factor for tessallating inside of the quad.
    output.inside[0] = tessellationFactor;
	output.inside[1] = tessellationFactor;

    return output;
}


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

	output.tex = patch[pointId].tex;

	output.normal = patch[pointId].normal;

    return output;
}