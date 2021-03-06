#include "Device.h"

GP_USING

DEFINITION_SINGLE(CDevice)

CDevice::CDevice() :
	m_pDevice(NULL),
	m_pContext(NULL),
	m_pSwapChain(NULL),
	m_pRTView(NULL),
	m_pDepthBuffer(NULL),
	m_pDSView(NULL)
{
	memset(m_fClearColor, 0, sizeof(float) * 4);
	//m_fClearColor[1] = 1.f;
}

CDevice::~CDevice()
{
	SAFE_RELEASE(m_pDepthBuffer);
	SAFE_RELEASE(m_pDSView);

	SAFE_RELEASE(m_pRTView);
	SAFE_RELEASE(m_pSwapChain);

	if (m_pContext)
		m_pContext->ClearState();

	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pDevice);
}

ID3D11Device * CDevice::GetDevice() const
{
	return m_pDevice;
}

ID3D11DeviceContext * CDevice::GetContext() const
{
	return m_pContext;
}

IDXGISwapChain * CDevice::GetSwapChain() const
{
	return m_pSwapChain;
}

RESOLUTION CDevice::GetResolution() const
{
	return m_tRS;
}

bool CDevice::Init(HWND hWnd, UINT iWidth, UINT iHeight, bool bWindowMode)
{
	m_hWnd = hWnd;
	m_tRS.iWidth = iWidth;
	m_tRS.iHeight = iHeight;

	UINT	iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	DXGI_SWAP_CHAIN_DESC	tDesc = {};

	tDesc.BufferDesc.Width = iWidth;											// 클라이언트 영역 가로
	tDesc.BufferDesc.Height = iHeight;											// 세로
	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;						// 일반적으로 모니터의 색상당 비트 수가 24비트 이하이므로 정밀도를
																				// 그보다 더 높게 잡아봤자 낭비이므로 DXGI_FORMAT_R8G8B8A8_UNORM으로 지정
	tDesc.BufferDesc.RefreshRate.Numerator = 60;		
	tDesc.BufferDesc.RefreshRate.Denominator = 1;
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	tDesc.BufferCount = 1;
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tDesc.OutputWindow = hWnd;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	tDesc.Windowed = bWindowMode;

	D3D_FEATURE_LEVEL	eLevel = D3D_FEATURE_LEVEL_11_0;
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL,	D3D_DRIVER_TYPE_HARDWARE, NULL, iFlag, NULL, 0,	D3D11_SDK_VERSION, &tDesc, &m_pSwapChain,
		&m_pDevice, &eLevel, &m_pContext)))
		return false;
	// D3D11CreateDeviceAndSwapChain(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain
	// ppDevice, pFeatureLevel, ppImmediateContext) -> 장치와 장치 문맥 생성하는 함수


	// 백버퍼를 얻어온다.
	ID3D11Texture2D*	pBackBuffer = NULL;

	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(void**)&pBackBuffer);

	// 얻어온 백버퍼를 이용해서 렌더링 타겟 뷰를 만들어준다.
	m_pDevice->CreateRenderTargetView(pBackBuffer, NULL,
		&m_pRTView);

	SAFE_RELEASE(pBackBuffer);

	// 깊이버퍼 : 깊이 정보를 담는 2차원 텍스쳐 
	// 깊이버퍼용 텍스쳐를 만든다.
	D3D11_TEXTURE2D_DESC	tDepthDesc = {};

	tDepthDesc.Width = iWidth;
	tDepthDesc.Height = iHeight;
	tDepthDesc.MipLevels = 1;
	tDepthDesc.ArraySize = 1;
	tDepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tDepthDesc.SampleDesc.Quality = 0;
	tDepthDesc.SampleDesc.Count = 1;

	tDepthDesc.Usage = D3D11_USAGE_DEFAULT;		// 텍스처의 용도를 뜻하는 필드 
												// -> D3D11_USAGE_DEFAULT : CPU는 자원을 읽거나 쓸수 없고 GPU가 모든 읽고 쓰기를 수행함
	tDepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (FAILED(m_pDevice->CreateTexture2D(&tDepthDesc, NULL,
		&m_pDepthBuffer)))
		return false;

	// 생성한 깊이버퍼를 이용해서 DepthStencilView를 만들어준다.
	if (FAILED(m_pDevice->CreateDepthStencilView(m_pDepthBuffer,
		NULL, &m_pDSView)))
		return false;

	// 위에서 생성한 렌더타겟뷰와 깊이스텐실뷰를 렌더링 파이프라인의
	// 출력병합기 단계에 묶어준다.
	m_pContext->OMSetRenderTargets(1, &m_pRTView, m_pDSView);

	// 뷰포트 설정. 
	// 뷰포트(viewport) : 장면을 그려 넣고자 하는 후면 버퍼의 부분 직사각형 영역
	D3D11_VIEWPORT	tVP = {};

	tVP.Width = iWidth;
	tVP.Height = iHeight;
	tVP.MaxDepth = 1.f;	
	// Direct3D가 사용하는 깊이 버퍼 값의 범위는 0~1임 -> MinDepth = 0, MaxDepth = 1로 설정 해야함

	// Direct3D에게 뷰포트를 알려준다.
	m_pContext->RSSetViewports(1, &tVP);

	return true;
}

void CDevice::ClearTarget()
{
	m_pContext->ClearRenderTargetView(m_pRTView, m_fClearColor);
	m_pContext->ClearDepthStencilView(m_pDSView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CDevice::Present()
{
	m_pSwapChain->Present(0, 0);
}

Vector2 CDevice::GetWindowDeviceResolution() const
{
	RECT	rc;
	GetClientRect(m_hWnd, &rc);
	Vector2	v;
	v.x = m_tRS.iWidth / (float)(rc.right - rc.left);
	v.y = m_tRS.iHeight / (float)(rc.bottom - rc.top);

	return v;
}

Vector2 CDevice::GetWindowResolution() const
{
	RECT	rc;
	GetClientRect(m_hWnd, &rc);

	return Vector2(rc.right - rc.left, rc.bottom - rc.top);
}
