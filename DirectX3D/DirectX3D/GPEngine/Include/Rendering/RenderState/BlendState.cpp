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

	tDesc.BlendEnable = bEnable;				// 혼합 활성화 : true, 비활성화 : false
	tDesc.SrcBlend = eSrc;						// RGB성분 혼합의 원본 혼합 계수
	tDesc.DestBlend = eDest;					// RGB성분 혼합의 대상 혼합 계수
	tDesc.BlendOp = eOp;						// RGB 성분 혼합 연산자
	tDesc.SrcBlendAlpha = eSrcAlpha;			// 알파 성분 혼합의 원본 혼합 계수
	tDesc.DestBlendAlpha = eDestAlpha;			// 알파 성분 혼합의 대상 혼합 계수
	tDesc.BlendOpAlpha = eAlphaOp;				// 알파 성분 혼합의 혼합 연산자
	tDesc.RenderTargetWriteMask = iWriteMask;	// 렌더 대상 쓰기 마스크
	// -> 이 플래그들은 혼합의 결과를 후면 버퍼의 어떤 색상 채널들에 기록할 것인지를 결정함

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
	// True로 설정하면 다중표본화 기법인 알파-포괄도 변환이 활성화 된다. 
	// -> 알파-포괄도 변환을 위해서는 다중표본화를 활성화해야 한다
	//		=> 즉, 다중표본화에 맞는 후면 버퍼와 깊이 버퍼를 생성해야 한다.
	tDesc.IndependentBlendEnable = bIndependent;
	/* Direct3D 11에서는 장면을 최대 8개까지의 렌더 대상들에 동시에 렌더링할 수 있다.
	이 플래그를 True로 설정하면 각 렌더 대상마다 혼합을 개별적으로(혼합 활성화 여부, 혼합 게수, 연산자 등을 각각 다르게 두어서) 수행할 수 있다. 
	False로 설정하면 모든 렌더 대상의 혼합연산이 RenderTarget 배열의 첫 원소에 있는 설정에 따라 동일하게 수행된다.*/

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

// 기존의 혼합상태를 받아와서 Old~ 에 저장을 하고 내가 생성한 혼합 상태를 설정
void BlendState::SetState()
{
	// OMGetBlendState : 기존의 혼합 상태를 받아오는 함수
	CONTEXT->OMGetBlendState((ID3D11BlendState**)&m_pOldState, m_fOldBlendFactor,
		&m_iOldSampleMask);
	// OMSetBlendState : 혼합 상태를 설정하는 함수
	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pState, m_fBlendFactor, m_iSampleMask);
}

// 기존의 혼합 상태로 설정
void BlendState::ResetState()
{
	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pOldState, m_fOldBlendFactor, m_iOldSampleMask);
	SAFE_RELEASE(m_pOldState);
}
