#include "BlendState.h"
#include "../../Device/Device.h"

GP_USING

BlendState::BlendState()
{
	m_eType = RST_BLEND;
}


BlendState::~BlendState()
{
}

bool BlendState::AddDesc(bool bEnable, D3D11_BLEND eSrc, D3D11_BLEND eDest, D3D11_BLEND_OP eOp, D3D11_BLEND eSrcAlpha, D3D11_BLEND eDestAlpha, D3D11_BLEND_OP eAlphaOp, UINT8 iWriteMask)
{
	if(m_vecDesc.size() == 8)
		return false;

	D3D11_RENDER_TARGET_BLEND_DESC tDesc = {};

	tDesc.BlendEnable = bEnable;				// ȥ�� Ȱ��ȭ : true, ��Ȱ��ȭ : false
	tDesc.SrcBlend = eSrc;						// RGB���� ȥ���� ���� ȥ�� ���
	tDesc.DestBlend = eDest;					// RGB���� ȥ���� ��� ȥ�� ���
	tDesc.BlendOp = eOp;						// RGB ���� ȥ�� ������
	tDesc.SrcBlendAlpha = eSrcAlpha;			// ���� ���� ȥ���� ���� ȥ�� ���
	tDesc.DestBlendAlpha = eDestAlpha;			// ���� ���� ȥ���� ��� ȥ�� ���
	tDesc.BlendOpAlpha = eAlphaOp;				// ���� ���� ȥ���� ȥ�� ������
	tDesc.RenderTargetWriteMask = iWriteMask;	// ���� ��� ���� ����ũ
	// -> �� �÷��׵��� ȥ���� ����� �ĸ� ������ � ���� ä�ε鿡 ����� �������� ������

	m_vecDesc.push_back(tDesc);

	return true;
}

void BlendState::AddDesc(const D3D11_RENDER_TARGET_BLEND_DESC & tDesc)
{
	if (m_vecDesc.size() == 8)
		return;

	m_vecDesc.push_back(tDesc);
}

bool BlendState::CreateBlendState(const string & strKey, bool bAlphaCoverage, bool bIndependent)
{
	m_strKey = strKey;

	D3D11_BLEND_DESC	tDesc = {};

	tDesc.AlphaToCoverageEnable = bAlphaCoverage;
	// True�� �����ϸ� ����ǥ��ȭ ����� ����-������ ��ȯ�� Ȱ��ȭ �ȴ�. 
	// -> ����-������ ��ȯ�� ���ؼ��� ����ǥ��ȭ�� Ȱ��ȭ�ؾ� �Ѵ�
	//		=> ��, ����ǥ��ȭ�� �´� �ĸ� ���ۿ� ���� ���۸� �����ؾ� �Ѵ�.
	tDesc.IndependentBlendEnable = bIndependent;
	/* Direct3D 11������ ����� �ִ� 8�������� ���� ���鿡 ���ÿ� �������� �� �ִ�.
	�� �÷��׸� True�� �����ϸ� �� ���� ��󸶴� ȥ���� ����������(ȥ�� Ȱ��ȭ ����, ȥ�� �Լ�, ������ ���� ���� �ٸ��� �ξ) ������ �� �ִ�. 
	False�� �����ϸ� ��� ���� ����� ȥ�տ����� RenderTarget �迭�� ù ���ҿ� �ִ� ������ ���� �����ϰ� ����ȴ�.*/

	for (size_t i = 0; i < m_vecDesc.size(); ++i)
	{
		tDesc.RenderTarget[i] = m_vecDesc[i];
	}

	if (FAILED(DEVICE->CreateBlendState(&tDesc, (ID3D11BlendState**)&m_pState)))
		return false;

	memset(m_fBlendFactor, 0, sizeof(float) * 4);
	m_iSampleMask = 0xffffffff;

	return true;
}

// ������ ȥ�ջ��¸� �޾ƿͼ� Old~ �� ������ �ϰ� ���� ������ ȥ�� ���¸� ����
void BlendState::SetState()
{
	// OMGetBlendState : ������ ȥ�� ���¸� �޾ƿ��� �Լ�
	CONTEXT->OMGetBlendState((ID3D11BlendState**)&m_pOldState, m_fOldBlendFactor,
		&m_iOldSampleMask);
	// OMSetBlendState : ȥ�� ���¸� �����ϴ� �Լ�
	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pState, m_fBlendFactor, m_iSampleMask);
}

// ������ ȥ�� ���·� ����
void BlendState::ResetState()
{
	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pOldState, m_fOldBlendFactor, m_iOldSampleMask);
	SAFE_RELEASE(m_pOldState);
}
