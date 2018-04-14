
// �������ۿ��� �Ѿ���� ���������� �޾ƿ��� ���� ����ü
struct VS_INPUT_COLOR
{
	// : �ڿ� ���� ��Ҹ� Semantic �̶�� �Ѵ�. �ǹ��ڶ�� ���̴�.
	// �� ������ �Է·������� Ȥ�� ��·��������� � �������Ϳ���
	// ���� �������ų� ������ �������ش�.
	// Semantic�� �ڿ� ��ȣ�� ���� �� �ִ�. �� ��ȣ�� 0 ~ 15 �������̴�.
	// ��ȣ�� �Ⱥٿ��ָ� �⺻���� 0�̴�.
	float3	vPos	: POSITION;
	float4	vColor	: COLOR;
};

struct VS_OUTPUT_COLOR
{
	// SV�� ������ System Value�� �ȴ�. �� System���� ����ϴ� ���̴�.
	// �� �������Ϳ� �־��ָ� �츮�� �ٽ� ����� �� ����.
	float4	vPos	: SV_POSITION;
	float4	vColor	: COLOR;
};

struct VS_INPUT_TEX
{
	float3	vPos	: POSITION;
	float2	vUV		: TEXCOORD;
};

struct VS_OUTPUT_TEX
{
	float4	vPos	: SV_POSITION;
	float2	vUV		: TEXCOORD;
};

struct PS_OUTPUT
{
	float4	vColor	: SV_Target;
	//float4	vColor1	: SV_Target1;
};

Texture2D		g_DifTex	: register(t0);
SamplerState	g_DifSmp	: register(s0);

Texture2DArray	g_DifArrTex	: register(t10);

cbuffer Transform	: register(b0)
{
	matrix	g_matWorld;
	matrix	g_matView;
	matrix	g_matProj;
	matrix	g_matWV;
	matrix	g_matWVP;
	matrix	g_matWP;
	float3	g_vPivot;
	float	g_fTrEmpty;
	float3	g_vTrLength;
	float	g_fTrEmpty1;
}

cbuffer Animation2D	: register(b10)
{
	int		g_iAniType;
	int		g_iAniFrameX;
	int		g_iAniFrameY;
	int		g_iAniLengthX;
	int		g_iAniLengthY;
	float3	g_vAniEmpty;
}

float2 ComputeUV(float2 vInputUV)
{
	float2	vUV;

	vUV.x = (g_iAniFrameX + vInputUV.x) / g_iAniLengthX;
	vUV.y = (g_iAniFrameY + vInputUV.y) / g_iAniLengthY;

	return vUV;
}
