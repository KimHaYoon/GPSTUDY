#pragma once

#include "../Engine.h"

GP_BEGIN

class GP_DLL CResourcesManager
{
private:
	unordered_map<string, class CMesh*>		m_mapMesh;
	unordered_map<string, class CSampler*>	m_mapSampler;
	unordered_map<string, class CTexture*>	m_mapTexture;

public:
	bool Init();
	// Mesh
	class CMesh* CreateMesh(const string& strKey, UINT iVtxCount, UINT iVtxSize, D3D11_USAGE eVtxUsage,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void* pVtxData,
		UINT iIdxCount = 0, UINT iIdxSize = 0,
		D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_R32_UINT,
		void* pIdxData = NULL);

	class CMesh* FindMesh(const string& strKey);

	// Sampler
	class CSampler* CreateSampler(const string& strKey, D3D11_FILTER eFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_MODE eAddrU = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE eAddrV = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE eAddrW = D3D11_TEXTURE_ADDRESS_WRAP);

	class CSampler* FindSampler(const string& strKey);

	// Texture
	class CTexture* LoadTexture(const string& strKey, const wchar_t* pFileName,
		const string& strPathKey = TEXTURE_PATH);
	class CTexture* LoadTexture(const string& strKey, const char* pFileName,
		const string& strPathKey = TEXTURE_PATH);
	class CTexture* LoadTexture(const string& strKey, const vector<wstring>& vecFileName,
		const string& strPathKey = TEXTURE_PATH);
	class CTexture* LoadTextureFromFullPath(const string& strKey,
		const char* pFullPath);
	class CTexture* LoadTextureFromFullPath(const string& strKey,
		const vector<string>& vecFullPath);

	class CTexture* FindTexture(const string& strKey);	

	DECLARE_SINGLE(CResourcesManager)
};

GP_END