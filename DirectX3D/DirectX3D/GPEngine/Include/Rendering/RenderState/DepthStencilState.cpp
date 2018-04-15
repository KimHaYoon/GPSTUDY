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

	// 깊이 설정
	/*깊이 버퍼링을 활성화 하려면 True, 비활성화하려면 false를 지정.
	깊이 버퍼링을 활성화하면 깊이 판정이 일어나지 않으므로 그리기 순서가 중요해진다. 다른 물체 뒤에 가려진 픽셀이 그 물체의 픽셀을 덮어쓸 것이기 때문이다. 
	또한, 깊이 버퍼링을 비활성화하면 깊이 버퍼의 픽셀들이 더 이상 갱신되지 않는다.*/
	tDesc.DepthEnable = bDepth;
	/* DepthEanble를 True로 한 경우 D3D11_DEPTH_WRITE_MASK_ZERO를 지정하면 깊이 버퍼 쓰기(갱신)가 방지되나 깊이 판정은 여전히 수행된다.
	D3D11_DEPTH_WRITE_MASK_ALL을 지정하면 깊이 버퍼 쓰기가 활성화 되어서, 깊이 판정과 스텐실 판정을 모두 통과한 픽셀의 깊이가 깊이 버퍼에 새로이 기록된다.*/
	tDesc.DepthWriteMask = eDepthWriteMask;
	/*깊이 판정에 쓰인느 비교 함수*/
	tDesc.DepthFunc = eDepthFunc;
	
	// 스텐실 설정
	/*스텐실 판정을 활성화하려면 True, 비활성화하려면 False를 지정한다.*/
	tDesc.StencilEnable = bStencil;
	/*스텐실 판정 조건문*/
	tDesc.StencilReadMask = iReadMask;
	/*스텐실 버퍼를 갱신할 때 특정 비트 값들이 갱신되지 않도록 하는 비트 마스크*/
	tDesc.StencilWriteMask = iWriteMask;
	/*전면 삼각형에 대한 스텐실 버퍼 적용 방식을 서술하는 D3D11_DEPTH_SETENCILOP_DESC 구조체를 지정*/
	tDesc.FrontFace = tFrontDesc;
	/*후면 삼각형에 대한 스텐실 버퍼 적용 방식을 서술하는 D33D11_DEPTH_STENCILOP_DESC 구조체를 지정*/
	tDesc.BackFace = tBackDesc;

	/*CreateDepthStencilState를 이용해서 ID3D11DepthStencilState를 구한다*/
	if (FAILED(DEVICE->CreateDepthStencilState(&tDesc, (ID3D11DepthStencilState**)&m_pState)))
		return false;

	/*ID3D11DepthStencilState 인터페이스를 얻었다면 다음으로 할 일은 OMSetDepthStencilState를 이용해서 깊이·스텐실 상태를 출력 병합기 단계에 묶는 것이다.
	m_iStencilRef : 스텐실 판정에 쓰이는 32비트 스텐실 기준 값*/
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