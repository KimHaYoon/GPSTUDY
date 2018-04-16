#include "GameObject.h"
#include "../Scene/Layer.h"

GP_USING

list<CGameObject*> CGameObject::m_ObjList;
list<CGameObject*> CGameObject::m_FindObjectList;
unordered_map<string, CGameObject*> CGameObject::m_mapPrototype;

CGameObject * CGameObject::CreateObject(const string & strTag, CLayer * pLayer)
{
	CGameObject* pObj = new CGameObject;

	pObj->SetTag(strTag);

	return nullptr;
}

CGameObject * CGameObject::CreatePrototype(const string & strKey, CScene * pScene)
{
	return nullptr;
}

CGameObject * CGameObject::CreateClone(const string & strKey, CLayer * pLayer)
{
	return nullptr;
}

CGameObject * CGameObject::CreateObjectDontDestroy(const string & strTag, CLayer * pLayer)
{
	return nullptr;
}

CGameObject * CGameObject::CreatePrototypeDontDestroy(const string & strKey, CScene * pScene)
{
	return nullptr;
}

void CGameObject::ChangePrototypeScene(const string & strkey, CScene * pScene)
{
}

void CGameObject::AddPrototype(const string & strTag, CGameObject * pPrototype)
{
}

void CGameObject::AddObjList(CGameObject * pObj)
{
}

void CGameObject::ChangeLayer(const string & strTag, CLayer * pLayer)
{
}

void CGameObject::DontDestroy(const string & strTag, bool bDontDestroy)
{
}

void CGameObject::EraseObj(CGameObject * pObj)
{
}

void CGameObject::EraseObj(const string & strTag)
{
}

void CGameObject::EraseObj()
{
}

void CGameObject::ErasePrototype(CScene * pScene)
{
}

void CGameObject::ErasePrototype(CGameObject * pObj)
{
}

void CGameObject::ErasePrototype(const string & strKey)
{
}

void CGameObject::ErasePrototype()
{
}

CGameObject * CGameObject::FindObject(const string & strTag)
{
	return nullptr;
}

CGameObject * CGameObject::FindObject(COMPONENT_TYPE eType)
{
	return nullptr;
}

const list<CGameObject*>* CGameObject::FindObjects(const string & strTag)
{
	return nullptr;
}

CGameObject * CGameObject::FindPrototype(const string & strKey)
{
	return nullptr;
}

CGameObject::CGameObject()
{
}

CGameObject::CGameObject(const CGameObject & obj)
{
}

CGameObject::~CGameObject()
{
}

void CGameObject::DontDestroyOnLoad(bool bDontDestroy)
{
}

bool CGameObject::IsDontDestroy() const
{
	return false;
}

void CGameObject::AddChild(CGameObject * pChild)
{
}

void CGameObject::DeleteChild(const string & strTag)
{
}

void CGameObject::DeleteChild(CGameObject * pChild)
{
}

void CGameObject::DeleteParent()
{
}

CGameObject * CGameObject::GetParent() const
{
	return nullptr;
}

void CGameObject::ChangeLayer(CLayer * pLayer)
{
}

CTransform * CGameObject::GetTransform() const
{
	return nullptr;
}

void CGameObject::SetScene(CScene * pScene)
{
}

void CGameObject::SetLayer(CLayer * pLayer)
{
}

bool CGameObject::Init()
{
	return false;
}

void CGameObject::Input(float fTime)
{
}

int CGameObject::Update(float fTime)
{
	return 0;
}

int CGameObject::LateUpdate(float fTime)
{
	return 0;
}

void CGameObject::Collision(float fTime)
{
}

void CGameObject::Render(float fTime)
{
}

CGameObject * CGameObject::Clone()
{
	return nullptr;
}

void CGameObject::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CGameObject::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CGameObject::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CGameObject::UpdateTransformHierarchy()
{
}

const list<CComponent*>* CGameObject::FindComponentsFromTag(const string & strTag)
{
	return nullptr;
}

const list<CComponent*>* CGameObject::FindComponentsFromTypeName(const string & strTypeName)
{
	return nullptr;
}

const list<CComponent*>* CGameObject::FindComponentsFromType(COMPONENT_TYPE eType)
{
	return nullptr;
}

bool CGameObject::CheckComponentFromTag(const string & strTag)
{
	return false;
}

bool CGameObject::CheckComponentFromTypeName(const string & strTypeName)
{
	return false;
}

bool CGameObject::CheckComponentFromType(COMPONENT_TYPE eType)
{
	return false;
}

CComponent * CGameObject::AddComponent(CComponent * pComponent)
{
	return nullptr;
}
