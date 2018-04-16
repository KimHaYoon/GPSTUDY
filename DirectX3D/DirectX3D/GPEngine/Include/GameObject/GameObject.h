#pragma once
#include "../Core/Obj.h"

GP_BEGIN

class GP_DLL CGameObject :
	public CObj
{
private:
	static list<CGameObject*>	m_ObjList;
	static list<CGameObject*>	m_FindObjectList;
	static unordered_map<string, CGameObject*>	m_mapPrototype;

public:
	static CGameObject* CreateObject(const string& strTag, class CLayer* pLayer = NULL);
	static CGameObject* CreatePrototype(const string& strKey, class CScene* pScene = NULL);
	static CGameObject* CreateClone(const string& strKey, class CLayer* pLayer = NULL);
	static CGameObject* CreateObjectDontDestroy(const string& strTag, class CLayer* pLayer);
	static CGameObject* CreatePrototypeDontDestroy(const string& strKey, class CScene* pScene);

	static void ChangePrototypeScene(const string& strkey, class CScene* pScene);
	static void AddPrototype(const string& strTag, CGameObject* pPrototype);
	static void AddObjList(CGameObject* pObj);
	static void ChangeLayer(const string& strTag, class CLayer* pLayer);
	static void DontDestroy(const string& strTag, bool bDontDestroy = true);

	static void EraseObj(CGameObject* pObj);
	static void EraseObj(const string& strTag);
	static void EraseObj();
	static void ErasePrototype(class CScene* pScene);
	static void ErasePrototype(CGameObject* pObj);
	static void ErasePrototype(const string& strKey);
	static void ErasePrototype();

	static CGameObject* FindObject(const string& strTag);
	static CGameObject* FindObject(COMPONENT_TYPE eType);
	static const list<CGameObject*>* FindObjects(const string& strTag);

private:
	static CGameObject* FindPrototype(const string& strKey);

private:
	CGameObject();
	CGameObject(const CGameObject& obj);
	~CGameObject();

private:
	class CScene*			m_pScene;
	class CLayer*			m_pLayer;
	class CTransform*		m_pTransform;
	list<class CComponent*>	m_ComList;
	list<class CComponent*>	m_FindList;
	list<CGameObject*>		m_ChildList;
	CGameObject*			m_pParent;
	bool					m_bDontDestroy;

public:
	void DontDestroyOnLoad(bool bDontDestroy = true);
	bool IsDontDestroy()	const;
	void AddChild(CGameObject* pChild);
	void DeleteChild(const string& strTag);
	void DeleteChild(CGameObject* pChild);
	void DeleteParent();
	CGameObject* GetParent() const;
	void ChangeLayer(class CLayer* pLayer);

public:
	class CTransform* GetTransform() const;

public:
	void SetScene(class CScene* pScene);
	void SetLayer(class CLayer* pLayer);

public:
	bool Init();
	void Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);
	CGameObject* Clone();

public:
	void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void OnCollision(class CCollider* pSrc, class CCollider* pDest,	float fTime);
	void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest, float fTime);

public:
	void UpdateTransformHierarchy();

public:
	template<typename T>
	T* FindComponentFromTag(const string& strTag)
	{
		list<CComponent*>::iterator	iter;
		list<CComponent*>::iterator	iterEnd = m_ComList.end();

		for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTag() == strTag)
			{
				(*iter)->AddRef();
				return (T*)*iter;
			}
		}

		return NULL;
	}

	template<typename T>
	T* FindComponentFromTypeName(const string& strTypeName)
	{

	}
	
};

GP_END