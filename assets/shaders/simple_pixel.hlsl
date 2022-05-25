// globals
cbuffer object_colors: register(b0)
{
	float4 g_diffuseObjectColor: packoffset(c0);
	bool g_has_texture: packoffset(c1.x);
};

// textures and samplers
Texture2D g_texture: register(t0);
SamplerState g_sampler_linear: register(s0);

// input
struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 textcoord: TEXCOORD0;
	float4 diffuse: COLOR0;
};

// logic
float4 main(PS_INPUT input): SV_TARGET
{
	float4 output_color = input.diffuse;

	if (g_has_texture)
		output_color = g_texture.Sample(g_sampler_linear, input.textcoord);

	return output_color;
}
