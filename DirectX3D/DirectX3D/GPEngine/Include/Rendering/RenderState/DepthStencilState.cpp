#include "DepthStencilState.h"
#include "../../Device/Device.h"

GP_USING

CDepthStencilState::CDepthStencilState()
{
	m_eType = RST_DEPTH;
}

CDepthStencilState::~CDepthStencilState()
{
}

bool CDepthStencilState::CreateDepthStencilState(const string & strKey, bool bDepth, D3D11_DEPTH_WRITE_MASK eDepthWriteMask, D3D11_COMPARISON_FUNC eDepthFunc, bool bStencil, UINT8 iReadMask, UINT8 iWriteMask, D3D11_DEPTH_STENCILOP_DESC tFrontDesc, D3D11_DEPTH_STENCILOP_DESC tBackDesc)
{
	m_strKey = strKey;

	D3D11_DEPTH_STENCIL_DESC	tDesc = {};

	// ���� ����
	/*���� ���۸��� Ȱ��ȭ �Ϸ��� True, ��Ȱ��ȭ�Ϸ��� false�� ����.
	���� ���۸��� Ȱ��ȭ�ϸ� ���� ������ �Ͼ�� �����Ƿ� �׸��� ������ �߿�������. �ٸ� ��ü �ڿ� ������ �ȼ��� �� ��ü�� �ȼ��� ��� ���̱� �����̴�. 
	����, ���� ���۸��� ��Ȱ��ȭ�ϸ� ���� ������ �ȼ����� �� �̻� ���ŵ��� �ʴ´�.*/
	tDesc.DepthEnable = bDepth;
	/* DepthEanble�� True�� �� ��� D3D11_DEPTH_WRITE_MASK_ZERO�� �����ϸ� ���� ���� ����(����)�� �����ǳ� ���� ������ ������ ����ȴ�.
	D3D11_DEPTH_WRITE_MASK_ALL�� �����ϸ� ���� ���� ���Ⱑ Ȱ��ȭ �Ǿ, ���� ������ ���ٽ� ������ ��� ����� �ȼ��� ���̰� ���� ���ۿ� ������ ��ϵȴ�.*/
	tDesc.DepthWriteMask = eDepthWriteMask;
	/*���� ������ ���δ� �� �Լ�*/
	tDesc.DepthFunc = eDepthFunc;
	
	// ���ٽ� ����
	/*���ٽ� ������ Ȱ��ȭ�Ϸ��� True, ��Ȱ��ȭ�Ϸ��� False�� �����Ѵ�.*/
	tDesc.StencilEnable = bStencil;
	/*���ٽ� ���� ���ǹ�*/
	tDesc.StencilReadMask = iReadMask;
	/*���ٽ� ���۸� ������ �� Ư�� ��Ʈ ������ ���ŵ��� �ʵ��� �ϴ� ��Ʈ ����ũ*/
	tDesc.StencilWriteMask = iWriteMask;
	/*���� �ﰢ���� ���� ���ٽ� ���� ���� ����� �����ϴ� D3D11_DEPTH_SETENCILOP_DESC ����ü�� ����*/
	tDesc.FrontFace = tFrontDesc;
	/*�ĸ� �ﰢ���� ���� ���ٽ� ���� ���� ����� �����ϴ� D33D11_DEPTH_STENCILOP_DESC ����ü�� ����*/
	tDesc.BackFace = tBackDesc;

	/*CreateDepthStencilState�� �̿��ؼ� ID3D11DepthStencilState�� ���Ѵ�*/
	if (FAILED(DEVICE->CreateDepthStencilState(&tDesc, (ID3D11DepthStencilState**)&m_pState)))
		return false;

	/*ID3D11DepthStencilState �������̽��� ����ٸ� �������� �� ���� OMSetDepthStencilState�� �̿��ؼ� ���̡����ٽ� ���¸� ��� ���ձ� �ܰ迡 ���� ���̴�.
	m_iStencilRef : ���ٽ� ������ ���̴� 32��Ʈ ���ٽ� ���� ��*/
	m_iStencilRef = 0xff;

	return true;
}

void CDepthStencilState::SetState()
{
	CONTEXT->OMGetDepthStencilState((ID3D11DepthStencilState**)&m_pOldState,
		&m_iOldStencilRef);
	CONTEXT->OMSetDepthStencilState((ID3D11DepthStencilState*)m_pState,
		m_iStencilRef);
}

void CDepthStencilState::ResetState()
{
	CONTEXT->OMSetDepthStencilState((ID3D11DepthStencilState*)m_pOldState,
		m_iOldStencilRef);
	SAFE_RELEASE(m_pOldState);
}