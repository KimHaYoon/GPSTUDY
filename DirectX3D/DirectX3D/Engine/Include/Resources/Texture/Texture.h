#pragma once
#include "../../Core/Obj.h"
#include "DirectXTex.h"

GP_BEGIN

class GP_DLL CTexture :
	public CObj
{
private:
	friend class CResourcesManager;

private:
	CTexture();
	~CTexture();

private:
	vector<ScratchImage*>		m_vecImage;		// ScratchImage : DirectX에서 이미지를 띄우는 class
	ID3D11ShaderResourceView*	m_pSRView;
	string		m_strKey;
	bool		m_bArray;
	vector<string>	m_vecFullPath;

public:
	bool LoadTexture(const string& strKey, const wchar_t* pFileName,
		const string& strPathKey = TEXTURE_PATH);
	bool LoadTexture(const string& strKey, const char* pFileName,
		const string& strPathKey = TEXTURE_PATH);
	bool LoadTexture(const string& strKey, const vector<wstring>& vecFileName,
		const string& strPathKey = TEXTURE_PATH);
	bool LoadTextureFromFullPath(const string& strKey, const char* pFullPath);
	bool LoadTextureFromFullPath(const string& strKey, const vector<string>& vecFullPath);
	void SetTexture(int iRegister, int iShaderConstantType);
};

GP_END