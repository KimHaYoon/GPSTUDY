
#include "Share.fx"

cbuffer ColliderColor	: register(b13)
{
	float4	g_vColliderColor;
}

VS_OUTPUT_COLOR ColliderVS(VS_INPUT_COLOR input)
{
	VS_OUTPUT_COLOR	output = (VS_OUTPUT_COLOR)0;

	output.vPos = mul(float4(input.vPos, 1.f), g_matWVP);
	output.vColor = g_vColliderColor;

	return output;
}

PS_OUTPUT ColliderPS(VS_OUTPUT_COLOR input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	output.vColor = input.vColor;

	return output;
}