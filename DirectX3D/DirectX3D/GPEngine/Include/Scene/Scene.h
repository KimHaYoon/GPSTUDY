#pragma once
#include "../Core/Obj.h"

GP_BEGIN

class GP_DLL CScene :
	public CObj
{
private:
	friend class CSceneManager;

private:
	CScene();
	~CScene();

private:
	vector<class CLayer*>			m_vecLayer;
	vector<class CSceneManager*>	m_vecSceneScript;
	class CGameObject*				m_pMainCameraObj;
	class CTransform*				m_pMainCameraTr;
	class CCamera*					m_pMainCamera;
	unordered_map<string, class CGameObject*>	m_mapCamera;

public:
	bool Init();
	void Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);

public:
	class CLayer* CreateLayer(const string& strTag = "", int  iZOrder = 0);
	class CLayer* GetLayer(const string& strTag);

	template<typename T>
	T* CreateSceneScript(const string& strTag)
	{
		T* pScript = new T;
		
		pScript->SetScene(this);
		pScript->SetTag(strTag);

		if (!pScript->Init())
		{
			SAFE_RELEASE(pScript);
			return NULL;
		}

		pScript->AddRef();
		m_vecSceneScript.push_back(pScript);

		return pScript;
	}

private:
	bool SortZ(CLayer* p1, CLayer* p2);
};

GP_END