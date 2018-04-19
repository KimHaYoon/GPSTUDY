#pragma once

#include "../../Engine.h"

GP_BEGIN

enum COLLIST_TYPE
{
	CLT_2D,
	CLT_3D
};

typedef struct GP_DLL _tagCollisionSection
{
	class CCollider**	pArray;
	int		iSize;
	int		iCapasity;
	COLLIST_TYPE	eType;
	Vector3		vMin;
	Vector3		vMax;

	_tagCollisionSection() :
		iSize(0),
		iCapasity(1000),
		eType(CLT_3D)
	{
		pArray = new CCollider*[iCapasity];
		memset(pArray, 0, sizeof(CCollider*) * iCapasity);
	}

	~_tagCollisionSection()
	{
		delete[]	pArray;
	}
}COLLISIONSECTION, *PCOLLISIONSECTION;

typedef struct GP_DLL _tagCollisionGroup
{
	PCOLLISIONSECTION* pSectionList;
	int			iSectionX;
	int			iSectionY;
	int			iSectionZ;
	int			iSectionMax;
	Vector3		vSize;
	Vector3		vMin;
	Vector3		vMax;

	_tagCollisionGroup() :
		iSectionX(1),
		iSectionY(1),
		iSectionZ(1),
		vSize(1000.f, 1000.f, 1000.f)
	{
	}

	~_tagCollisionGroup()
	{
		for (int z = 0; z < iSectionZ; ++z)
		{
			for (int y = 0; y < iSectionY; ++y)
			{
				for (int x = 0; x < iSectionX; ++x)
				{
					int  iIndex = z * iSectionX * iSectionY + y * iSectionX + x;

					delete pSectionList[iIndex];
				}
			}
		}
		
		delete[]	pSectionList;
	}
}COLLISIONGROUP, *PCOLLISIONGROUP;

class GP_DLL CCollisionManager
{
private:
	unordered_map<string, PCOLLISIONGROUP>	m_mapCollisionGroup;

public:
	bool CreateGroup(const string& strKey, int iSectionX, int iSectionY, int iSectionZ,
		const Vector3& vSize, COLLIST_TYPE eType = CLT_3D);
	bool ModifyGroup(const string& strKey, int iSectionX, int iSectionY, int iSectionZ,
		const Vector3& vSize, COLLIST_TYPE eType = CLT_3D);
	bool EraseGroup(const string& strKey);

private:
	PCOLLISIONGROUP FindGroup(const string& strKey);

public:
	bool Init();
	bool AddCollider(class CGameObject* pObj);
	void Collision(float fTime);

	DECLARE_SINGLE(CCollisionManager)
};

GP_END