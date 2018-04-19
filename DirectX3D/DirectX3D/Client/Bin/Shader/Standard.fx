
#include "Share.fx"

// �Լ� ���ڿ� �������͸� �����س��� ��� ���ڸ� �ȳ־��ְ� �ش� �������Ϳ���
// ���� ����� ����Ѵ�.
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

	// Normal�� ��������� ������ش�.
	output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV).xyz);
	output.vViewPos = mul(float4(vPos, 1.f), g_matWV).xyz;
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT StandardColorNormalPS(VS_OUTPUT_COLOR_NORMAL input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	// Vertex Shader���� ViewPos, Normal�� View�������� ��ȯ�ؼ� ������ �Դ�.
	// ������ �����鵵 ��������� ��ȯ�� �־�� �Ѵ�.
	float3 vLightDir = (float3)0;

	// ���⼺ ������ ���
	if (g_iLightType == 0)
	{
		// ��������� ���� ������ �� �������� ��ȯ�� �ش�.
		vLightDir = mul(float4(g_vLightDir, 0.f), g_matView).xyz;

		// ��ȯ�� �� ������ ���� ������ �������ͷ� ������ְ� ������ �������� ���Ѵ�.
		// ������ �������� ���ؾ� ������ ���ؼ� ������ ������ ���� �� �ֱ� �����̴�.
		// �������� ī�޶� ���ϴ� ���͸� ������ش�.
		vLightDir = -normalize(vLightDir);
	}
	
	// �� ������ ���
	if (g_iLightType == 1)
	{

	}

	// Spot ������ ���
	if (g_iLightType == 2)
	{
	}

	// ������ ������ �� ������ ������ ������� ������ ������� �븻�� �����ؼ� cos ��Ÿ�� ����
	// �ش�. �� ���� �̿��ؼ� ������ ������ ó���� �ϴµ� �� ���� - ���� ������ �Ǹ� ������
	// �ڿ��� ����ٴ� �ǹ��̴�. �׷��� ������ ���´ٸ� 0���� ó�����ش�.
	// �� �������� �̿��ؼ� Diffuse �� �������ش�.
	// Diffuse�� ������ Diffuse * ������ Diffuse * �������� �ȴ�.
	float4	vDif = g_vLightDiffuse * g_vMtrlDiffuse * max(0, dot(input.vNormal, vLightDir));
	float4	vAmb = g_vLightAmbient * g_vMtrlAmbient;

	// ���ݻ籤 ó���� �ϱ� ���ؼ��� ������ �ش� ������ �ε������� �ݻ�Ǵ� ���Ͱ� �ʿ��ϴ�.
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

	// clip : �ȼ����� ���� �ʰ� ����Ѵ�.
	// �̰�� ���̹��ۿ��� ���� �Ⱦ���.
	if (output.vColor.a == 0.f)
		clip(-1);

	return output;
}
