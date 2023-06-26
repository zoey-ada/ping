// globals
cbuffer matrices: register(b0)
{
	matrix g_world_view_projection: packoffset(c0);
	matrix g_world: packoffset(c4);
};

cbuffer object_colors: register(b1)
{
	float4 g_diffuse_color: packoffset(c0);
	bool g_has_texture: packoffset(c1.x);
};

// input
struct VS_INPUT
{
	float4 position: POSITION;
	float3 normal: NORMAL;
	float2 textcoord: TEXCOORD0;
};

// output
struct VS_OUTPUT
{
	float4 position: SV_POSITION;
	float2 textcoord: TEXCOORD0;
	float4 diffuse: COLOR0;
};

// logic
VS_OUTPUT main(VS_INPUT input)
{
	input.position.w = 1.0f;

	VS_OUTPUT output;

	output.position = mul(input.position, g_world_view_projection);
	output.textcoord = input.textcoord;
	output.diffuse = g_diffuse_color;

	return output;
}
