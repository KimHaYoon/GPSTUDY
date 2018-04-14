
// 정점버퍼에서 넘어오는 정점정보를 받아오기 위한 구조체
struct VS_INPUT_COLOR
{
	// : 뒤에 오는 요소를 Semantic 이라고 한다. 의미자라는 뜻이다.
	// 이 변수가 입력레지스터 혹은 출력레지스터의 어떤 레지스터에서
	// 값을 가져오거나 보낼지 결정해준다.
	// Semantic은 뒤에 번호가 붙을 수 있다. 이 번호는 0 ~ 15 번까지이다.
	// 번호를 안붙여주면 기본으로 0이다.
	float3	vPos	: POSITION;
	float4	vColor	: COLOR;
};

struct VS_OUTPUT_COLOR
{
	// SV가 붙으면 System Value가 된다. 즉 System에서 사용하는 값이다.
	// 이 레지스터에 넣어주면 우리가 다시 사용할 수 없다.
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
