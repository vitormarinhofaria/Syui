struct VS_Input {
	float2 pos: POS;
	float2 uv: TEX;
    float4 color: COLOR;
};

struct VS_Output {
	float4 pos : SV_POSITION;
	float2 uv: TEXCOORD;
    float4 color: COLOR;
};

Texture2D font_texture : register(t0);
SamplerState font_sampler : register(s0);

float contour(float dist, float edge, float width) {
	return clamp(smoothstep(edge - width, edge + width, dist), 0.0, 1.0);
}

float median(float3 v) {
	return max(min(v.r, v.g), min(max(v.r, v.g), v.b));
}

float4 ps_main(VS_Output input) : SV_TARGET
{
	float4 color_out = input.color;
	float4 transparent = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float midpoint = 0.4;
	float upperpoint = 0.5;
	float clipUp = 0.51;
	float4 color = font_texture.Sample(font_sampler, input.uv);

	color_out.a = smoothstep(midpoint, upperpoint, color.a);
	return color_out;
}
