#include "Shader.h"
#include "../../Core/File/PathManager.h"
#include "../../Device/Device.h"

GP_USING

CShader::CShader() :
	m_pVS(NULL),
	m_pVSBlob(NULL),
	m_pPS(NULL),
	m_pPSBlob(NULL)
{
	SetTag("Shader");
	SetTypeName("CShader");
	SetTypeID<CShader>();
}

CShader::~CShader()
{
	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pPS);
	SAFE_RELEASE(m_pPSBlob);
}

string CShader::GetKey() const
{
	return m_strKey;
}

void * CShader::GetShaderByteCode()
{
	return m_pVSBlob->GetBufferPointer();
}

int CShader::GetShaderByteCodeLength()
{
	return m_pVSBlob->GetBufferSize();
}

bool CShader::LoadShader(const string & strKey, TCHAR * pFileName,
	char * pEntry[ST_MAX], const string & strPathKey)
{
	m_strKey = strKey;

	if (!LoadVertexShader(strKey, pFileName, pEntry[ST_VERTEX],
		strPathKey))
		return false;

	if (!LoadPixelShader(strKey, pFileName, pEntry[ST_PIXEL],
		strPathKey))
		return false;

	return true;
}

bool CShader::LoadVertexShader(const string & strKey, TCHAR * pFileName,
	char * pEntry, const string & strPathKey)
{
	UINT	iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	const wchar_t* pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);
	wstring	strFile;

	if (pPath)
		strFile = pPath;

	strFile += pFileName;

	char*	pTarget = "vs_5_0";
	ID3DBlob*	pErr = NULL;

	if (FAILED(D3DCompileFromFile(strFile.c_str(), NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntry, pTarget, iFlag,
		0, &m_pVSBlob, &pErr)))
		return false;

	if (FAILED(DEVICE->CreateVertexShader(m_pVSBlob->GetBufferPointer(),
		m_pVSBlob->GetBufferSize(), NULL, &m_pVS)))
		return false;

	return true;
}

bool CShader::LoadPixelShader(const string & strKey, TCHAR * pFileName,
	char * pEntry, const string & strPathKey)
{
	UINT	iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	const wchar_t* pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);
	wstring	strFile;

	if (pPath)
		strFile = pPath;

	strFile += pFileName;

	char*	pTarget = "ps_5_0";
	ID3DBlob*	pErr = NULL;

	if (FAILED(D3DCompileFromFile(strFile.c_str(), NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntry, pTarget, iFlag,
		0, &m_pPSBlob, &pErr)))
		return false;

	if (FAILED(DEVICE->CreatePixelShader(m_pPSBlob->GetBufferPointer(),
		m_pPSBlob->GetBufferSize(), NULL, &m_pPS)))
		return false;

	return true;
}

void CShader::SetShader()
{
	// 렌더링 파이프라인에 셰이더를 지정한다.
	CONTEXT->VSSetShader(m_pVS, NULL, 0);
	CONTEXT->PSSetShader(m_pPS, NULL, 0);
}