#pragma once

#include "../Engine.h"

GP_BEGIN

class GP_DLL CDevice
{
private:
	ID3D11Device*			m_pDevice;		// Direct3D 11 장치 : 기능 지원 점검과 자원 할당에 사용됨
	ID3D11DeviceContext*	m_pContext;		// Direct3D 11 문맥 : 렌더 대상을 설정, 자원을 그래픽 파이프라인에 묶고, GPU가 수행할 렌더링 명령들을 지시하는데 쓰임
	/* 이 두 인터페이스는 Direct3D의 주된 인터페이스로, 물리적인 그래픽 장치 하드웨어에 대한 소프트웨어 제어기
		응용 프로그램은 이 인터페이스 들을 통해서 하드웨어에게 할 일(GPU 메모리에 자원 할당, 후면 버퍼 지우기, 자원을 여러 파이프라인 단계에 묶기,
		기하구조 그리기 등)을 지시함*/
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView*	m_pRTView;
	ID3D11Texture2D*		m_pDepthBuffer;
	ID3D11DepthStencilView*	m_pDSView;
	float	m_fClearColor[4];
	RESOLUTION				m_tRS;
	HWND					m_hWnd;

public:
	ID3D11Device* GetDevice()	const;
	ID3D11DeviceContext* GetContext()	const;
	IDXGISwapChain* GetSwapChain()	const;
	RESOLUTION GetResolution()	const;

public:
	bool Init(HWND hWnd, UINT iWidth, UINT iHeight,	bool bWindowMode);
	void ClearTarget();
	void Present();

public:
	Vector2 GetWindowDeviceResolution()	const;
	Vector2 GetWindowResolution()	const;

	DECLARE_SINGLE(CDevice)
};

GP_END