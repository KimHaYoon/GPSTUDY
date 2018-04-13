#include "ShaderManager.h"
#include "Shader.h"
#include "../../Device/Device.h"

GP_USING

DEFINITION_SINGLE(CShaderManager)

CShaderManager::CShaderManager()
{
	m_iInputSize = 0;
}

CShaderManager::~CShaderManager()
{
	unordered_map<string, PCONSTANTBUFFER>::iterator	iter;
	unordered_map<string, PCONSTANTBUFFER>::iterator	iterEnd = m_mapCBuffer.end();

	for (iter = m_mapCBuffer.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second->pBuffer);
	}

	Safe_Delete_Map(m_mapCBuffer);
	Safe_Release_Map(m_mapLayout);
	Safe_Release_Map(m_mapShader);
}

bool CShaderManager::Init()
{
	char*	pEntry[ST_MAX] = { "StandardColorVS", "StandardColorPS" };
	CShader*	pShader = LoadShader(STANDARD_COLOR_SHADER, L"Standard.fx",
		pEntry);

	SAFE_RELEASE(pShader);

	pEntry[ST_VERTEX] = "StandardTexVS";
	pEntry[ST_PIXEL] = "StandardTexPS";
	pShader = LoadShader(STANDARD_TEX_SHADER, L"Standard.fx",
		pEntry);

	SAFE_RELEASE(pShader);

	pEntry[ST_VERTEX] = "ColliderVS";
	pEntry[ST_PIXEL] = "ColliderPS";
	pShader = LoadShader(COLLIDER_SHADER, L"Collider.fx",
		pEntry);

	SAFE_RELEASE(pShader);

	pEntry[ST_VERTEX] = "UIVS";
	pEntry[ST_PIXEL] = "UIPS";
	pShader = LoadShader(UI_SHADER, L"UI.fx",
		pEntry);

	SAFE_RELEASE(pShader);

	pEntry[ST_VERTEX] = "UIVS";
	pEntry[ST_PIXEL] = "UIColorPS";
	pShader = LoadShader(UI_COLOR_SHADER, L"UI.fx",
		pEntry);

	SAFE_RELEASE(pShader);

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
		16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	CreateInputLayout("VertexColor", STANDARD_COLOR_SHADER);

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	CreateInputLayout("VertexTex", STANDARD_TEX_SHADER);

	CreateCBuffer("Transform", 0, sizeof(TRANSFORMCBUFFER));

	CreateCBuffer("Animation2D", 10, sizeof(ANIMATION2DBUFFER));

	CreateCBuffer("ColliderColor", 13, sizeof(Vector4));

	CreateCBuffer("Button", 11, sizeof(BUTTONCBUFFER));

	return true;
}

CShader * CShaderManager::LoadShader(const string & strKey,
	TCHAR * pFileName, char * pEntry[ST_MAX], const string & strPathKey)
{
	CShader*	pShader = FindShader(strKey);

	if (pShader)
		return pShader;

	pShader = new CShader;

	if (!pShader->LoadShader(strKey, pFileName, pEntry, strPathKey))
	{
		SAFE_RELEASE(pShader);
		return NULL;
	}

	pShader->AddRef();

	m_mapShader.insert(make_pair(strKey, pShader));

	return pShader;
}

CShader * CShaderManager::FindShader(const string & strKey)
{
	unordered_map<string, class CShader*>::iterator	iter = m_mapShader.find(strKey);

	if (iter == m_mapShader.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

void CShaderManager::AddInputDesc(char * pName, int iSemanticIdx,
	DXGI_FORMAT eFmt, int iInputSlot, int iSize,
	D3D11_INPUT_CLASSIFICATION eClass, int iInstanceDataStep)
{
	D3D11_INPUT_ELEMENT_DESC	tDesc = {};
	tDesc.SemanticName = pName;
	tDesc.SemanticIndex = iSemanticIdx;
	tDesc.Format = eFmt;
	tDesc.InputSlot = iInputSlot;
	tDesc.AlignedByteOffset = m_iInputSize;
	tDesc.InputSlotClass = eClass;
	tDesc.InstanceDataStepRate = iInstanceDataStep;

	m_iInputSize += iSize;

	m_vecInputDesc.push_back(tDesc);
}

bool CShaderManager::CreateInputLayout(const string & strKey,
	const string& strShaderKey)
{
	if (FindInputLayout(strKey))
		return false;

	CShader*	pShader = FindShader(strShaderKey);

	ID3D11InputLayout*	pInputLayout = NULL;

	if (FAILED(DEVICE->CreateInputLayout(&m_vecInputDesc[0], m_vecInputDesc.size(),
		pShader->GetShaderByteCode(), pShader->GetShaderByteCodeLength(),
		&pInputLayout)))
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);

	m_vecInputDesc.clear();
	m_iInputSize = 0;

	m_mapLayout.insert(make_pair(strKey, pInputLayout));

	return true;
}

void CShaderManager::SetInputLayout(const string & strKey)
{
	ID3D11InputLayout*	pInputLayout = FindInputLayout(strKey);

	if (!pInputLayout)
		return;

	CONTEXT->IASetInputLayout(pInputLayout);
}

ID3D11InputLayout * CShaderManager::FindInputLayout(const string & strKey)
{
	unordered_map<string, ID3D11InputLayout*>::iterator	iter = m_mapLayout.find(strKey);

	if (iter == m_mapLayout.end())
		return NULL;

	return iter->second;
}

bool CShaderManager::CreateCBuffer(const string & strKey, int iRegister, int iSize)
{
	if (FindCBuffer(strKey))
		return false;

	PCONSTANTBUFFER	pBuffer = new CONSTANTBUFFER;

	pBuffer->iRegister = iRegister;
	pBuffer->iSize = iSize;

	D3D11_BUFFER_DESC	tDesc = {};

	tDesc.ByteWidth = iSize;
	tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, NULL, &pBuffer->pBuffer)))
		return false;

	m_mapCBuffer.insert(make_pair(strKey, pBuffer));

	return true;
}

void CShaderManager::UpdateCBuffer(const string & strKey, void * pData,
	int iShaderConstantType)
{
	PCONSTANTBUFFER	pBuffer = FindCBuffer(strKey);

	if (!pBuffer)
		return;

	D3D11_MAPPED_SUBRESOURCE	tMap = {};

	CONTEXT->Map(pBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD,
		0, &tMap);

	memcpy(tMap.pData, pData, pBuffer->iSize);

	CONTEXT->Unmap(pBuffer->pBuffer, 0);

	if (iShaderConstantType & SCT_VERTEX)
		CONTEXT->VSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	if (iShaderConstantType & SCT_PIXEL)
		CONTEXT->PSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);
}

PCONSTANTBUFFER CShaderManager::FindCBuffer(const string & strKey)
{
	unordered_map<string, PCONSTANTBUFFER>::iterator	iter = m_mapCBuffer.find(strKey);

	if (iter == m_mapCBuffer.end())
		return NULL;

	return iter->second;
}
