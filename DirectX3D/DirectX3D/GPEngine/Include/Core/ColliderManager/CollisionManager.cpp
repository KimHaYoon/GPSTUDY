#include "CollisionManager.h"
#include "../../Device/Device.h"
#include "../../GameObject/GameObject.h"
#include "../../Component/Transform/Transform.h"
#include "../../Component/Camera/Camera.h"
#include "../../Component/Collider/Collider.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneManager.h"

GP_USING

DEFINITION_SINGLE(CCollisionManager)

CCollisionManager::CCollisionManager()
{
}


CCollisionManager::~CCollisionManager()
{
	Safe_Delete_Map(m_mapCollisionGroup);
}

bool CCollisionManager::CreateGroup(const string & strKey, int iSectionX, int iSectionY, int iSectionZ, const Vector3 & vSize, COLLIST_TYPE eType)
{
	PCOLLISIONGROUP	pGroup = FindGroup(strKey);

	if (pGroup)
		return false;

	pGroup = new COLLISIONGROUP;

	pGroup->iSectionX = iSectionX;
	pGroup->iSectionY = iSectionY;
	pGroup->iSectionZ = iSectionZ;
	pGroup->vSize = vSize;

	Vector3	vGroupMin = Vector3(vSize.x * iSectionX / -2.f, vSize.y * iSectionY / -2.f,
		vSize.z * iSectionZ / -2.f);
	Vector3	vGroupMax = Vector3(vSize.x * iSectionX / 2.f, vSize.y * iSectionY / 2.f,
		vSize.z * iSectionZ / 2.f);
	pGroup->vMin = vGroupMin;
	pGroup->vMax = vGroupMax;
	pGroup->iSectionMax = iSectionX * iSectionY * iSectionZ;

	pGroup->pSectionList = new PCOLLISIONSECTION[pGroup->iSectionMax];

	for (int i = 0; i < iSectionZ; ++i)
	{
		for (int j = 0; j < iSectionY; ++j)
		{
			for (int k = 0; k < iSectionX; ++k)
			{
				int iIndex = i * iSectionX * iSectionY + j * iSectionX + k;

				pGroup->pSectionList[iIndex] = new COLLISIONSECTION;
			}
		}
	}

	m_mapCollisionGroup.insert(make_pair(strKey, pGroup));

	return true;
}

bool CCollisionManager::ModifyGroup(const string & strKey, int iSectionX, int iSectionY, int iSectionZ, const Vector3 & vSize, COLLIST_TYPE eType)
{
	PCOLLISIONGROUP	pGroup = FindGroup(strKey);

	if (!pGroup)
		return false;

	EraseGroup(strKey);

	pGroup = new COLLISIONGROUP;

	pGroup->iSectionX = iSectionX;
	pGroup->iSectionY = iSectionY;
	pGroup->iSectionZ = iSectionZ;
	pGroup->vSize = vSize;

	Vector3	vGroupMin = Vector3(vSize.x * iSectionX / -2.f, vSize.y * iSectionY / -2.f,
		vSize.z * iSectionZ / -2.f);
	Vector3	vGroupMax = Vector3(vSize.x * iSectionX / 2.f, vSize.y * iSectionY / 2.f,
		vSize.z * iSectionZ / 2.f);
	pGroup->vMin = vGroupMin;
	pGroup->vMax = vGroupMax;
	pGroup->iSectionMax = iSectionX * iSectionY * iSectionZ;

	pGroup->pSectionList = new PCOLLISIONSECTION[pGroup->iSectionMax];

	for (int i = 0; i < iSectionZ; ++i)
	{
		for (int j = 0; j < iSectionY; ++j)
		{
			for (int k = 0; k < iSectionX; ++k)
			{
				int iIndex = i * iSectionX * iSectionY + j * iSectionX + k;

				pGroup->pSectionList[iIndex] = new COLLISIONSECTION;
			}
		}
	}

	m_mapCollisionGroup.insert(make_pair(strKey, pGroup));

	return true;
}

bool CCollisionManager::EraseGroup(const string & strKey)
{
	unordered_map<string, PCOLLISIONGROUP>::iterator	iter = m_mapCollisionGroup.find(strKey);

	if (iter == m_mapCollisionGroup.end())
		return false;

	SAFE_DELETE(iter->second);

	m_mapCollisionGroup.erase(iter);

	return true;
}

PCOLLISIONGROUP CCollisionManager::FindGroup(const string & strKey)
{
	unordered_map<string, PCOLLISIONGROUP>::iterator	iter = m_mapCollisionGroup.find(strKey);

	if (iter == m_mapCollisionGroup.end())
		return NULL;

	return iter->second;
}

bool CCollisionManager::Init()
{
	CreateGroup("Default", 5, 5, 5, Vector3(10.f, 10.f, 10.f));
	CreateGroup("UI", 5, 5, 1,
		Vector3(DEVICE_RESOLUTION.iWidth / 5.f, DEVICE_RESOLUTION.iHeight / 5.f, 1.f), CLT_2D);

	return true;
}

bool CCollisionManager::AddCollider(CGameObject * pObj)
{
	if (!pObj->CheckComponentFromType(CT_COLLIDER))
		return false;

	// 카메라를 얻어온다.
	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CTransform*	pCameraTr = pScene->GetMainCameraTr();

	Vector3	vCameraPos = pCameraTr->GetWorldPos();

	SAFE_RELEASE(pCameraTr);

	SAFE_RELEASE(pScene);

	// 충돌체 리스트를 얻어온다.
	const list<CComponent*>* pList = pObj->FindComponentsFromType(CT_COLLIDER);

	list<CComponent*>::const_iterator	iter;
	list<CComponent*>::const_iterator	iterEnd = pList->end();

	for (iter = pList->begin(); iter != iterEnd; ++iter)
	{
		CCollider*	pCollider = (CCollider*)*iter;
		PCOLLISIONGROUP	pGroup = FindGroup(pCollider->GetCollisionGroup());

		if (!pGroup)
		{
			assert(false);
		}

		// 어떤 영역에 속해 있는지를 구해준다.(영역 인덱스를 구해야 한다.)
		// 충돌체의 Min, Max 값을 얻어온다.
		Vector3	vColliderMin = pCollider->GetColliderMin();
		Vector3	vColliderMax = pCollider->GetColliderMax();

		// 로컬 중심의 영역을 월드공간으로 변환해준다.(카메라 더해주면 끝)
		Vector3	vSectionMin = pGroup->vMin + vCameraPos;
		Vector3	vSectionMax = pGroup->vMax + vCameraPos;

		if (vSectionMin.x > vColliderMax.x || vSectionMax.x < vColliderMin.x ||
			vSectionMin.y > vColliderMax.y || vSectionMax.y < vColliderMin.y ||
			vSectionMin.z > vColliderMax.z || vSectionMax.z < vColliderMin.z)
			continue;

		vColliderMin -= vSectionMin;
		vColliderMax -= vSectionMin;

		// 가로, 세로 크기로 나눠준다.
		vColliderMin /= pGroup->vSize;
		vColliderMax /= pGroup->vSize;

		// 8개의 정점 정보를 구해준다.
		Vector3	vPos[8];

		vPos[0] = Vector3(vColliderMin.x, vColliderMax.y, vColliderMin.z);
		vPos[1] = Vector3(vColliderMax.x, vColliderMax.y, vColliderMin.z);
		vPos[2] = Vector3(vColliderMin.x, vColliderMin.y, vColliderMin.z);
		vPos[3] = Vector3(vColliderMax.x, vColliderMin.y, vColliderMin.z);

		vPos[4] = Vector3(vColliderMin.x, vColliderMax.y, vColliderMax.z);
		vPos[5] = Vector3(vColliderMax.x, vColliderMax.y, vColliderMax.z);
		vPos[6] = Vector3(vColliderMin.x, vColliderMin.y, vColliderMax.z);
		vPos[7] = Vector3(vColliderMax.x, vColliderMin.y, vColliderMax.z);

		vector<int>	vecIndex;

		// 8개 정점정보의 인덱스를 구해준다.
		for (int i = 0; i < 8; ++i)
		{
			int	iIndexX = (int)vPos[i].x;
			int	iIndexY = (int)vPos[i].y;
			int	iIndexZ = (int)vPos[i].z;

			if (iIndexX < 0 || iIndexX >= pGroup->iSectionX)
				continue;

			else if (iIndexY < 0 || iIndexY >= pGroup->iSectionY)
				continue;

			else if (iIndexZ < 0 || iIndexZ >= pGroup->iSectionZ)
				continue;

			int	iIndex = iIndexZ * pGroup->iSectionX * pGroup->iSectionY +
				iIndexY * pGroup->iSectionX + iIndexX;

			// 구해준 인덱스가 중복인지를 판단한다.
			bool	bAcc = false;
			for (size_t j = 0; j < vecIndex.size(); ++j)
			{
				if (vecIndex[j] == iIndex)
				{
					bAcc = true;
					break;
				}
			}

			if (!bAcc)
				vecIndex.push_back(iIndex);
		}

		// 구해준 인덱스 섹션에 충돌체를 추가해준다.
		for (size_t i = 0; i < vecIndex.size(); ++i)
		{
			int	iIndex = vecIndex[i];
			if (pGroup->pSectionList[iIndex]->iSize == pGroup->pSectionList[iIndex]->iCapasity)
			{
				pGroup->pSectionList[iIndex]->iCapasity *= 1.5f;
				CCollider**	pArray = new CCollider*[pGroup->pSectionList[iIndex]->iCapasity];
				memcpy(pArray, pGroup->pSectionList[iIndex]->pArray,
					sizeof(CCollider*) * pGroup->pSectionList[iIndex]->iSize);

				SAFE_DELETE_ARRAY(pGroup->pSectionList[iIndex]->pArray);

				pGroup->pSectionList[iIndex]->pArray = pArray;
			}

			pGroup->pSectionList[iIndex]->pArray[pGroup->pSectionList[iIndex]->iSize] = pCollider;
			++pGroup->pSectionList[iIndex]->iSize;
		}
	}

	return true;
}

void CCollisionManager::Collision(float fTime)
{
	unordered_map<string, PCOLLISIONGROUP>::iterator	iter;
	unordered_map<string, PCOLLISIONGROUP>::iterator	iterEnd = m_mapCollisionGroup.end();

	bool	bCollision = false;

	for (iter = m_mapCollisionGroup.begin(); iter != iterEnd; ++iter)
	{
		PCOLLISIONGROUP	pGroup = iter->second;

		for (int i = 0; i < pGroup->iSectionMax; ++i)
		{
			if (pGroup->pSectionList[i]->iSize < 2)
			{
				pGroup->pSectionList[i]->iSize = 0;
				continue;
			}

			for (int j = 0; j < pGroup->pSectionList[i]->iSize - 1; ++j)
			{
				for (int k = j + 1; k < pGroup->pSectionList[i]->iSize; ++k)
				{
					CGameObject*	pSrc = pGroup->pSectionList[i]->pArray[j]->GetGameObject();
					CGameObject*	pDest = pGroup->pSectionList[i]->pArray[k]->GetGameObject();

					if (pSrc == pDest)
					{
						SAFE_RELEASE(pSrc);
						SAFE_RELEASE(pDest);
						continue;
					}

					if (pGroup->pSectionList[i]->pArray[j]->Collision(pGroup->pSectionList[i]->pArray[k]))
					{
						bCollision = true;

						if (!pGroup->pSectionList[i]->pArray[j]->CheckCollList(pGroup->pSectionList[i]->pArray[k]))
						{
							pGroup->pSectionList[i]->pArray[j]->AddCollList(pGroup->pSectionList[i]->pArray[k]);
							pGroup->pSectionList[i]->pArray[k]->AddCollList(pGroup->pSectionList[i]->pArray[j]);

							pSrc->OnCollisionEnter(pGroup->pSectionList[i]->pArray[j], pGroup->pSectionList[i]->pArray[k], fTime);
							pDest->OnCollisionEnter(pGroup->pSectionList[i]->pArray[k], pGroup->pSectionList[i]->pArray[j], fTime);
						}

						else
						{
							pSrc->OnCollision(pGroup->pSectionList[i]->pArray[j], pGroup->pSectionList[i]->pArray[k], fTime);
							pDest->OnCollision(pGroup->pSectionList[i]->pArray[k], pGroup->pSectionList[i]->pArray[j], fTime);
						}
					}

					else
					{
						if (pGroup->pSectionList[i]->pArray[j]->CheckCollList(pGroup->pSectionList[i]->pArray[k]))
						{
							pGroup->pSectionList[i]->pArray[j]->EraseCollList(pGroup->pSectionList[i]->pArray[k]);
							pGroup->pSectionList[i]->pArray[k]->EraseCollList(pGroup->pSectionList[i]->pArray[j]);

							pSrc->OnCollisionLeave(pGroup->pSectionList[i]->pArray[j], pGroup->pSectionList[i]->pArray[k], fTime);
							pDest->OnCollisionLeave(pGroup->pSectionList[i]->pArray[k], pGroup->pSectionList[i]->pArray[j], fTime);
						}
					}

					SAFE_RELEASE(pSrc);
					SAFE_RELEASE(pDest);
				}
			}

			pGroup->pSectionList[i]->iSize = 0;
		}
	}
}