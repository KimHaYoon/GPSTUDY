
#include "Share.fx"

// 함수 인자에 레지스터를 지정해놓는 경우 인자를 안넣어주고 해당 레지스터에서
// 값을 끌어다 사용한다.
VS_OUTPUT_COLOR StandardColorVS(VS_INPUT_COLOR input)
{
	VS_OUTPUT_COLOR	output = (VS_OUTPUT_COLOR)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT StandardColorPS(VS_OUTPUT_COLOR input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	output.vColor = input.vColor;

	return output;
}

VS_OUTPUT_COLOR_NORMAL	StandardColorNormalVS(VS_INPUT_COLOR_NORMAL input)
{
	VS_OUTPUT_COLOR_NORMAL	output = (VS_OUTPUT_COLOR_NORMAL)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);

	// Normal을 뷰공간으로 만들어준다.
	output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV).xyz);
	output.vViewPos = mul(float4(vPos, 1.f), g_matWV).xyz;
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT StandardColorNormalPS(VS_OUTPUT_COLOR_NORMAL input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	// Vertex Shader에서 ViewPos, Normal을 View공간으로 변환해서 가지고 왔다.
	// 조명의 정보들도 뷰공간으로 변환해 주어야 한다.
	float3 vLightDir = (float3)0;

	// 방향성 조명일 경우
	if (g_iLightType == 0)
	{
		// 월드공간의 조명 방향을 뷰 공간으로 변환해 준다.
		vLightDir = mul(float4(g_vLightDir, 0.f), g_matView).xyz;

		// 변환된 뷰 공간의 조명 방향을 단위벡터로 만들어주고 조명의 역방향을 구한다.
		// 조명의 역방향을 구해야 내적을 통해서 조명의 강도를 구할 수 있기 때문이다.
		// 정점에서 카메라를 향하는 벡터를 만들어준다.
		vLightDir = -normalize(vLightDir);
	}
	
	// 점 조명일 경우
	if (g_iLightType == 1)
	{

	}

	// Spot 조명일 경우
	if (g_iLightType == 2)
	{
	}

	// 위에서 구해준 뷰 공간의 조명의 역방향과 정점의 뷰공간의 노말을 내적해서 cos 세타를 구해
	// 준다. 이 값을 이용해서 조명의 강도로 처리를 하는데 이 값이 - 값이 나오게 되면 조명이
	// 뒤에서 비춘다는 의미이다. 그래서 음수가 나온다면 0으로 처리해준다.
	// 이 조명강도를 이용해서 Diffuse 에 적용해준다.
	// Diffuse는 조명의 Diffuse * 재질의 Diffuse * 조명강도가 된다.
	float4	vDif = g_vLightDiffuse * g_vMtrlDiffuse * max(0, dot(input.vNormal, vLightDir));
	float4	vAmb = g_vLightAmbient * g_vMtrlAmbient;

	// 정반사광 처리를 하기 위해서는 조명이 해당 정점에 부딪혔을때 반사되는 벡터가 필요하다.
	float3	vR = 2.f * input.vNormal * dot(input.vNormal, vLightDir) - vLightDir;
	float3	vView = -normalize(input.vViewPos);

	float4	vMtrlSpc = float4(g_vMtrlSpecular.xyz, 1.f);
	float4	vSpc = g_vLightSpecular * vMtrlSpc * pow(max(0, dot(vR, vView)), g_vMtrlSpecular.w);

	output.vColor = input.vColor * (vDif + vAmb) + vSpc;

	return output;
}

VS_OUTPUT_TEX StandardTexVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX	output = (VS_OUTPUT_TEX)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);

	if (g_iAniType == 0)
		output.vUV = ComputeUV(input.vUV);

	else
		output.vUV = input.vUV;

	return output;
}

PS_OUTPUT StandardTexPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	if (g_iAniType == 1)
	{
		float3	vUV;
		vUV.xy = input.vUV;
		vUV.z = g_iAniFrameX;
		output.vColor = g_DifArrTex.Sample(g_DifSmp, vUV);
	}

	else
	{
		output.vColor = g_DifTex.Sample(g_DifSmp, input.vUV);
	}

	// clip : 픽셀값을 쓰지 않고 폐기한다.
	// 이경우 깊이버퍼에도 값을 안쓴다.
	if (output.vColor.a == 0.f)
		clip(-1);

	return output;
}
