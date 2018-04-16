#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"

GP_USING

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager() : 
	m_pCurScene(NULL),
	m_pNextScene(NULL)
{
}

CSceneManager::~CSceneManager()
{
}

CScene * CSceneManager::CreateScene(const string & strTag)
{
	return nullptr;
}

CScene * CSceneManager::CreateNextScene(const string & strTag)
{
	return nullptr;
}

CScene * CSceneManager::GetCurrentScene() const
{
	return nullptr;
}

void CSceneManager::AddDontDestroyObj(CGameObject * pObj, const string & strLayerTag, int iZOrder)
{
}

void CSceneManager::DeleteDontDestroyObj(CGameObject * pObj)
{
}

CGameObject * CSceneManager::FindDontDestroyObj(const string & strTag)
{
	return nullptr;
}

void CSceneManager::AddDontDestroyPrototype(CGameObject * pObj, const string & strTag)
{
}

void CSceneManager::DeleteDontDestroyPrototype(CGameObject * pObj)
{
}

CGameObject * CSceneManager::FindDontDestroyPrototype(const string & strTag)
{
	return nullptr;
}

void CSceneManager::Start()
{
}

bool CSceneManager::Init()
{
	return false;
}

void CSceneManager::Input(float fTime)
{
}

SCENE_CHANGE CSceneManager::Update(float fTime)
{
	return SCENE_CHANGE();
}

SCENE_CHANGE CSceneManager::LateUpdate(float fTime)
{
	return SCENE_CHANGE();
}

void CSceneManager::Collision(float fTime)
{
}

void CSceneManager::Render(float fTime)
{
}

SCENE_CHANGE CSceneManager::ChangeScene()
{
	return SCENE_CHANGE();
}

void CSceneManager::AddDontDestroyObj()
{
}

void CSceneManager::AddDontDestroyPrototype()
{
}
