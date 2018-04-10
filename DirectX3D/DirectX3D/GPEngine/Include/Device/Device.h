#pragma once

#include "../Engine.h"

GP_BEGIN

class GP_DLL CDevice
{
private:
	ID3D11Device*			m_pDevice;		// Direct3D 11 ��ġ : ��� ���� ���˰� �ڿ� �Ҵ翡 ����
	ID3D11DeviceContext*	m_pContext;		// Direct3D 11 ���� : ���� ����� ����, �ڿ��� �׷��� ���������ο� ����, GPU�� ������ ������ ��ɵ��� �����ϴµ� ����
	/* �� �� �������̽��� Direct3D�� �ֵ� �������̽���, �������� �׷��� ��ġ �ϵ��� ���� ����Ʈ���� �����
		���� ���α׷��� �� �������̽� ���� ���ؼ� �ϵ����� �� ��(GPU �޸𸮿� �ڿ� �Ҵ�, �ĸ� ���� �����, �ڿ��� ���� ���������� �ܰ迡 ����,
		���ϱ��� �׸��� ��)�� ������*/
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