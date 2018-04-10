#include "ResourcesManager.h"
#include "Mesh/Mesh.h"
#include "Texture/Sampler.h"

GP_USING

DEFINITION_SINGLE(CResourcesManager);

CResourcesManager::CResourcesManager()
{
}


CResourcesManager::~CResourcesManager()
{
	Safe_Release_Map(m_mapSampler);
	Safe_Release_Map(m_mapMesh);
}

bool CResourcesManager::Init()
{
	return true;
}

CMesh * CResourcesManager::CreateMesh(const string & strKey, UINT iVtxCount, UINT iVtxSize, D3D11_USAGE eVtxUsage, D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void * pVtxData, UINT iIdxCount, UINT iIdxSize, D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt, void * pIdxData)
{
	CMesh*	pMesh = FindMesh(strKey);

	if (pMesh)
		return pMesh;

	pMesh = new CMesh;

	if (!pMesh->CreateMesh(iVtxCount, iVtxSize, eVtxUsage, ePrimitive, pVtxData,
		iIdxCount, iIdxSize, eIdxUsage, eFmt, pIdxData))
	{
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	pMesh->SetKey(strKey);

	pMesh->AddRef();

	m_mapMesh.insert(make_pair(strKey, pMesh));

	return pMesh;
}

CMesh * CResourcesManager::FindMesh(const string & strKey)
{
	unordered_map<string, class CMesh*>::iterator	iter = m_mapMesh.find(strKey);

	if (iter == m_mapMesh.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

CSampler * CResourcesManager::CreateSampler(const string & strKey, D3D11_FILTER eFilter, D3D11_TEXTURE_ADDRESS_MODE eAddrU, D3D11_TEXTURE_ADDRESS_MODE eAddrV, D3D11_TEXTURE_ADDRESS_MODE eAddrW)
{
	CSampler*	pSampler = FindSampler(strKey);

	if (pSampler)
		return pSampler;

	pSampler = new CSampler;

	if (!pSampler->CreateSampler(strKey, eFilter, eAddrU, eAddrV, eAddrW))
	{
		SAFE_RELEASE(pSampler);
		return NULL;
	}

	pSampler->AddRef();

	m_mapSampler.insert(make_pair(strKey, pSampler));

	return pSampler;
}

CSampler * CResourcesManager::FindSampler(const string & strKey)
{
	unordered_map<string, CSampler*>::iterator	iter = m_mapSampler.find(strKey);

	if (iter == m_mapSampler.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}