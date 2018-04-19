#include "Input.h"
#include "../../Device/Device.h"
#include "../../GameObject/GameObject.h"
#include "../../Component/Renderer/Renderer2D.h"
#include "../../Component/Material/Material.h"
#include "../../Component/Transform/Transform.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneManager.h"

GP_USING

DEFINITION_SINGLE(CInput)

CInput::CInput() :
	m_pCreate(NULL),
	m_pMouseObj(NULL),
	m_pMouseTr(NULL),
	m_bMouseClip(false)
{
}

CInput::~CInput()
{
	CGameObject::EraseObj(m_pMouseObj);
	SAFE_RELEASE(m_pMouseTr);
	SAFE_RELEASE(m_pMouseObj);
	unordered_map<string, PKEYINFO>::iterator	iter;
	unordered_map<string, PKEYINFO>::iterator	iterEnd = m_mapKey.end();
	for (iter = m_mapKey.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	m_mapKey.clear();
}

POINT CInput::GetMousePos() const
{
	return m_ptMousePos;
}

Vector2 CInput::GetWorldMousePos() const
{
	return m_vWorldMousePos;
}

POINT CInput::GetMouseMove() const
{
	return m_ptMouseMove;
}

CGameObject * CInput::GetMouseObj() const
{
	m_pMouseObj->AddRef();
	return m_pMouseObj;
}

void CInput::SetMouseClip(bool bClip)
{
	m_bMouseClip = bClip;
}

bool CInput::Init(HWND hWnd, bool bOnMouseRenderer)
{
	m_hWnd = hWnd;
	m_bOnMouseRenderer = bOnMouseRenderer;

	CreateKey("MoveFront", 'W');
	CreateKey("MoveBack", 'S');
	CreateKey("RotInvY", 'A');
	CreateKey('D', "RotY");
	CreateKey(VK_CONTROL, '1', "Skill1");
	CreateKey(VK_CONTROL, "Ctrl");
	CreateKey(VK_LBUTTON, "MouseLButton");
	CreateKey(VK_RBUTTON, "MouseRButton");

	// �Ʒ� �Լ��� ��ũ����ǥ�� ���´�.
	GetCursorPos(&m_ptMousePos);
	// ��ũ�� ��ǥ�� Ŭ���̾�Ʈ ��ǥ�� �ٲ۴�.
	ScreenToClient(m_hWnd, &m_ptMousePos);

	Vector2	vRS = GET_SINGLE(CDevice)->GetWindowDeviceResolution();

	m_ptMousePos.x *= vRS.x;
	m_ptMousePos.y *= vRS.y;

	return true;
}

void CInput::Update(float fTime)
{
	unordered_map<string, PKEYINFO>::iterator	iter;
	unordered_map<string, PKEYINFO>::iterator	iterEnd = m_mapKey.end();

	for (iter = m_mapKey.begin(); iter != iterEnd; ++iter)
	{
		int	iPushCount = 0;
		for (size_t i = 0; i < iter->second->vecKey.size(); ++i)
		{
			if (GetAsyncKeyState(iter->second->vecKey[i]) & 0x8000)
				++iPushCount;
		}

		// ���Ϳ� �ִ� Ű�� ��� ������ ���
		if (iPushCount == iter->second->vecKey.size())
		{
			if (!iter->second->bDown && !iter->second->bPush)
			{
				iter->second->bPush = true;
				iter->second->bDown = true;
			}

			else
			{
				iter->second->bDown = false;
			}
		}

		// ���� �����ӿ� �� Ű�� ������ �־��� ���
		else if (iter->second->bDown || iter->second->bPush)
		{
			iter->second->bUp = true;
			iter->second->bDown = false;
			iter->second->bPush = false;
		}

		else if (iter->second->bUp)
		{
			iter->second->bUp = false;
		}
	}

	POINT	ptMousePos;
	// �Ʒ� �Լ��� ��ũ����ǥ�� ���´�.
	GetCursorPos(&ptMousePos);
	// ��ũ�� ��ǥ�� Ŭ���̾�Ʈ ��ǥ�� �ٲ۴�.
	ScreenToClient(m_hWnd, &ptMousePos);

	Vector2	vRS = GET_SINGLE(CDevice)->GetWindowDeviceResolution();

	ptMousePos.x *= vRS.x;
	ptMousePos.y *= vRS.y;

	m_ptMouseMove.x = ptMousePos.x - m_ptMousePos.x;
	m_ptMouseMove.y = ptMousePos.y - m_ptMousePos.y;
}

PKEYINFO CInput::FindKey(const string & strKey)	const
{
	unordered_map<string, PKEYINFO>::const_iterator	iter = m_mapKey.find(strKey);

	if (iter == m_mapKey.end())
		return NULL;

	return iter->second;
}

bool CInput::KeyDown(const string & strKey) const
{
	PKEYINFO pKey = FindKey(strKey);

	if (!pKey)
		return false;

	return pKey->bDown;
}

bool CInput::KeyPush(const string & strKey) const
{
	PKEYINFO pKey = FindKey(strKey);

	if (!pKey)
		return false;

	return pKey->bPush;
}

bool CInput::KeyUp(const string & strKey) const
{
	PKEYINFO pKey = FindKey(strKey);

	if (!pKey)
		return false;

	return pKey->bUp;
}
