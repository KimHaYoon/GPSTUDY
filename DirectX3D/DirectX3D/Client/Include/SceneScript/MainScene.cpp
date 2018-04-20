#include "MainScene.h"
#include "../ObjectScript/Player.h"

#include "Scene/Layer.h"
#include "Scene/Scene.h"
#include "GameObject/GameObject.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	CLayer*	pLayer = m_pScene->GetLayer("Default");

	CGameObject* pPlayer = CGameObject::CreateObject("Player", pLayer);
	CPlayer* pPlayerCom = pPlayer->AddComponent<CPlayer>("PlayerCom");
	SAFE_RELEASE(pPlayerCom);
	SAFE_RELEASE(pPlayer);
	SAFE_RELEASE(pLayer);
	return true;
}
int CMainScene::Update(float fTime)
{
	return 0;
}
