#include "Scene.h"
#include "Layer.h"

GP_USING

CScene::CScene()
{
}


CScene::~CScene()
{
}

bool CScene::Init()
{
	return false;
}

void CScene::Input(float fTime)
{
}

int CScene::Update(float fTime)
{
	return 0;
}

int CScene::LateUpdate(float fTime)
{
	return 0;
}

void CScene::Collision(float fTime)
{
}

void CScene::Render(float fTime)
{
}

CLayer * CScene::CreateLayer(const string & strTag, int iZOrder)
{
	return nullptr;
}

CLayer * CScene::GetLayer(const string & strTag)
{
	return nullptr;
}

bool CScene::SortZ(CLayer * p1, CLayer * p2)
{
	return p1->GetZOrder() < p2->GetZOrder();
}
