#include "Component.h"
#include "Transform/Transform.h"
#include "../GameObject/GameObject.h"
#include "../Scene/Scene.h"
#include "../Scene/Layer.h"

GP_USING

CComponent::CComponent() : 
	m_pScene(NULL),
	m_pLayer(NULL),
	m_pGameObject(NULL),
	m_pTransform(NULL)
{
}

CComponent::CComponent(const CComponent & com)
{
	*this = com;
	m_iRefCount = 1;
}

CComponent::~CComponent()
{
}

COMPONENT_TYPE CComponent::GetComponentType() const
{
	return m_eType;
}

void CComponent::SetScene(CScene * pScene)
{
	m_pScene = pScene;
}

void CComponent::SetLayer(CLayer * pLayer)
{
	m_pLayer = pLayer;
}

void CComponent::SetGameObject(CGameObject * pObj)
{
	m_pGameObject = pObj;
}

void CComponent::SetTransform(CTransform * pTransform)
{
	m_pTransform = pTransform;
}

CScene * CComponent::GetScene() const
{
	if (m_pScene)
		m_pScene->AddRef();

	return m_pScene;
}

CLayer * CComponent::GetLayer() const
{
	if (m_pLayer)
		m_pLayer->AddRef();

	return m_pLayer;
}

CGameObject * CComponent::GetGameObject() const
{
	if (m_pGameObject)
		m_pGameObject->AddRef();

	return m_pGameObject;
}

CTransform * CComponent::GetTransform() const
{
	if (m_pTransform)
		m_pTransform->AddRef();

	return m_pTransform;
}

bool CComponent::Init()
{
	return true;
}

void CComponent::Input(float fTime)
{
}

int CComponent::Update(float fTime)
{
	return 0;
}

int CComponent::LateUpdate(float fTime)
{
	return 0;
}

void CComponent::Collision(float fTime)
{
}

void CComponent::Render(float fTime)
{
}

CComponent * CComponent::Clone()
{
	return nullptr;
}

void CComponent::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CComponent::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CComponent::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

const list<CComponent*>* CComponent::FindComponentsFromTag(const string & strTag)
{
	return nullptr;
}

const list<CComponent*>* CComponent::FindComponentsFromTypeName(const string & strTypeName)
{
	return nullptr;
}

const list<CComponent*>* CComponent::FindComponentsFromType(COMPONENT_TYPE eType)
{
	return nullptr;
}
