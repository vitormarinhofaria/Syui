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

VS_Output vs_main(VS_Input input)
{
	VS_Output output;
	output.pos = float4(input.pos.x, input.pos.y, 1.0, 0.0);
	output.uv = input.uv;
    output.color = input.color;
	return output;
}