#include "GameObject.h"
#include "../Scene/Layer.h"
#include "../Scene/SceneManager.h"
#include "../Component/Transform/Transform.h"

GP_USING

list<CGameObject*> CGameObject::m_ObjList;
list<CGameObject*> CGameObject::m_FindObjectList;
unordered_map<string, CGameObject*> CGameObject::m_mapPrototype;

CGameObject * CGameObject::CreateObject(const string & strTag, CLayer * pLayer)
{
	CGameObject* pObj = new CGameObject;

	pObj->SetTag(strTag);

	if (pLayer)
		pLayer->AddObject(pObj);

	if (!pObj->Init())
	{
		if (pLayer)
			pLayer->EraseObject(pObj);

		CGameObject::EraseObj(pObj);
		SAFE_RELEASE(pObj);
		return NULL;
	}

	CGameObject::AddObjList(pObj);

	return pObj;
}

CGameObject * CGameObject::CreatePrototype(const string & strKey, CScene * pScene)
{
	CGameObject* pProto = FindPrototype(strKey);

	if (pProto)
		return NULL;

	pProto = new CGameObject;

	if (!pScene)
		pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	pProto->SetScene(pScene);
	pProto->SetTag(strKey);

	if (!pProto->Init())
	{
		SAFE_RELEASE(pProto);
		return NULL;
	}

	pProto->AddRef();
	m_mapPrototype.insert(make_pair(strKey, pProto));

	return pProto;
}

CGameObject * CGameObject::CreateClone(const string & strKey, CLayer * pLayer)
{
	CGameObject* pProto = FindPrototype(strKey);

	if (!pProto)
		return NULL;

	CGameObject* pClone = pProto->Clone();

	if (pLayer)
		pLayer->AddObject(pClone);

	CGameObject::AddObjList(pClone);

	return pClone;
}

CGameObject * CGameObject::CreateObjectDontDestroy(const string & strTag, CLayer * pLayer)
{
	CGameObject* pObj = GET_SINGLE(CSceneManager)->FindDontDestroyObj(strTag);

	if (pObj)
	{
		pObj->SetLayer(pLayer);
		pObj->SetScene(pLayer->GetScene());

		return pObj;
	}

	pObj = new CGameObject;

	pObj->SetTag(strTag);

	if (pLayer)
		pLayer->AddObject(pObj);

	if (!pObj->Init())
	{
		if (pLayer)
			pLayer->EraseObject(pObj);

		CGameObject::EraseObj(pObj);
		SAFE_RELEASE(pObj);

		return NULL;
	}

	CGameObject::AddObjList(pObj);

	pObj->m_bDontDestroy = true;

	GET_SINGLE(CSceneManager)->AddDontDestroyObj(pObj, pLayer->GetTag(), pLayer->GetZOrder());

	return pObj;
}

CGameObject * CGameObject::CreatePrototypeDontDestroy(const string & strKey, CScene * pScene)
{
	CGameObject* pPrototype = GET_SINGLE(CSceneManager)->FindDontDestroyPrototype(strKey);

	if (pPrototype)
		return pPrototype;

	pPrototype = new CGameObject;

	pPrototype->SetTag(strKey);

	pPrototype->SetScene(pScene);

	if (!pPrototype->Init())
	{
		SAFE_RELEASE(pPrototype);
		return NULL;
	}

	pPrototype->m_bDontDestroy = true;

	GET_SINGLE(CSceneManager)->AddDontDestroyPrototype(pPrototype, strKey);

	pPrototype->AddRef();

	m_mapPrototype.insert(make_pair(strKey, pPrototype));

	return pPrototype;
}

void CGameObject::ChangePrototypeScene(const string & strKey, CScene * pScene)
{
	CGameObject* pProto = FindPrototype(strKey);

	if (!pProto)
		return;

	pProto->SetScene(pScene);
}
	
void CGameObject::AddPrototype(const string & strTag, CGameObject * pPrototype)
{
	if (FindPrototype(strTag))
	{
		assert(false);
	}

	pPrototype->AddRef();
	m_mapPrototype.insert(make_pair(strTag, pPrototype));
}

void CGameObject::AddObjList(CGameObject * pObj)
{
	pObj->AddRef();
	
	m_ObjList.push_back(pObj);
}

void CGameObject::ChangeLayer(const string & strTag, CLayer * pLayer)
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->ChangeLayer(pLayer);
			break;
		}
	}
}

void CGameObject::DontDestroy(const string & strTag, bool bDontDestroy)
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter != m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->DontDestroyOnLoad(bDontDestroy);

			break;
		}
	}
}

void CGameObject::EraseObj(CGameObject * pObj)
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pObj)
		{
			SAFE_RELEASE((*iter));
			m_ObjList.erase(iter);
			return;
		}
	}
}

void CGameObject::EraseObj(const string & strTag)
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			SAFE_RELEASE((*iter));
			m_ObjList.erase(iter);
			return;
		}
	}
}

void CGameObject::EraseObj()
{
	Safe_Release_VecList(m_FindObjectList);
	Safe_Release_VecList(m_ObjList);
}

void CGameObject::ErasePrototype(CScene * pScene)
{
	unordered_map<string, CGameObject*>::iterator	iter;
	unordered_map<string, CGameObject*>::iterator	iterEnd = m_mapPrototype.end();

	for (iter = m_mapPrototype.begin(); iter != iterEnd;)
	{
		if (iter->second->m_pScene == pScene)
		{
			SAFE_RELEASE(iter->second);
			iter = m_mapPrototype.erase(iter);
			iterEnd = m_mapPrototype.end();
		}

		else
			++iter;
	}
}

void CGameObject::ErasePrototype(CGameObject * pObj)
{
	unordered_map<string, CGameObject*>::iterator	iter;
	unordered_map<string, CGameObject*>::iterator	iterEnd = m_mapPrototype.end();

	for (iter = m_mapPrototype.begin(); iter != iterEnd; ++iter)
	{
		if (pObj == iter->second)
		{
			SAFE_RELEASE(iter->second);
			m_mapPrototype.erase(iter);
			return;
		}
	}
}

void CGameObject::ErasePrototype(const string & strKey)
{
	unordered_map<string, CGameObject*>::iterator iter = m_mapPrototype.find(strKey);

	if (iter == m_mapPrototype.end())
		return;

	SAFE_RELEASE(iter->second);

	m_mapPrototype.erase(iter);
}

void CGameObject::ErasePrototype()
{
	Safe_Release_Map(m_mapPrototype);
}

CGameObject * CGameObject::FindObject(const string & strTag)
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return NULL;
}

CGameObject * CGameObject::FindObject(COMPONENT_TYPE eType)
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->CheckComponentFromType(eType))
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return NULL;
}

const list<CGameObject*>* CGameObject::FindObjects(const string & strTag)
{
	Safe_Release_VecList(m_FindObjectList);

	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			m_FindObjectList.push_back(*iter);
		}
	}

	return &m_FindObjectList;
}

CGameObject * CGameObject::FindPrototype(const string & strKey)
{
	unordered_map<string, CGameObject*>::iterator	iter = m_mapPrototype.find(strKey);

	if (iter == m_mapPrototype.end())
		return NULL;

	return iter->second;
}

CGameObject::CGameObject() : 
	m_pScene(NULL),
	m_pLayer(NULL),
	m_pTransform(NULL),
	m_pParent(NULL),
	m_bDontDestroy(false)
{
	SetTag("GameObject");
	SetTypeName("CGameObject");
	SetTypeID<CGameObject>();
}

CGameObject::CGameObject(const CGameObject & obj)
{
	*this = obj;

	m_iRefCount = 1;

	m_pTransform = obj.m_pTransform->Clone();
	m_pTransform->SetGameObject(this);
	m_pTransform->SetTransform(m_pTransform);

	m_ComList.clear();

	list<CComponent*>::const_iterator	iter;
	list<CComponent*>::const_iterator	iterEnd = obj.m_ComList.end();

	for (iter = obj.m_ComList.begin(); iter != iterEnd; ++iter)
	{
		CComponent* pClone = (*iter)->Clone();

		pClone->SetGameObject(this);
		pClone->SetTransform(m_pTransform);

		m_ComList.push_back(pClone);
	}

	m_ChildList.clear();

	list<CGameObject*>::const_iterator	iterC;
	list<CGameObject*>::const_iterator	iterCEnd = obj.m_ChildList.end();

	for (iterC = obj.m_ChildList.begin(); iterC != iterCEnd; ++iterC)
	{
		CGameObject*	pChild = (*iterC)->Clone();

		AddChild(pChild);

		SAFE_RELEASE(pChild);
	}
}

CGameObject::~CGameObject()
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ChildList.end();

	for (iter = m_ChildList.begin(); iter != iterEnd; ++iter)
	{
		CGameObject::EraseObj(*iter);
	}

	Safe_Release_VecList(m_ChildList);
	Safe_Release_VecList(m_FindList);
	SAFE_RELEASE(m_pTransform);
	Safe_Release_VecList(m_ComList);
}

void CGameObject::DontDestroyOnLoad(bool bDontDestroy)
{
	m_bDontDestroy = bDontDestroy;
}

bool CGameObject::IsDontDestroy() const
{
	return m_bDontDestroy;
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
