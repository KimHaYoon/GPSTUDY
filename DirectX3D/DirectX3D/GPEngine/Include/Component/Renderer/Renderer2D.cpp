#include "Renderer2D.h"
#include "../Material/Material.h"
#include "../Transform/Transform.h"
#include "../Animation/Animation2D.h"
#include "../Camera/Camera.h"
#include "../../Resources/Mesh/Mesh.h"
#include "../../Resources/ResourcesManager.h"
#include "../../Rendering/RenderManager.h"
#include "../../Rendering/Shader/ShaderManager.h"
#include "../../Rendering/RenderState/RenderState.h"
#include "../../Rendering/Shader/Shader.h"
#include "../../Scene/Scene.h"

GP_USING

CRenderer2D::CRenderer2D() :
	m_pMesh(NULL),
	m_pShader(NULL),
	m_pLayout(NULL),
	m_pMaterial(NULL)
{
	SetTag("Renderer2D");
	SetTypeName("CRenderer2D");
	SetTypeID<CRenderer2D>();
	m_eType = CT_RENDERER2D;
	memset(m_pRenderState, 0, sizeof(CRenderState*) * RST_END);
}

CRenderer2D::CRenderer2D(const CRenderer2D & renderer) :
	CComponent(renderer)
{
	m_pMesh = renderer.m_pMesh;
	m_pShader = renderer.m_pShader;
	m_pLayout = renderer.m_pLayout;
	m_strLayoutKey = renderer.m_strLayoutKey;
	m_pMaterial = NULL;

	if (renderer.m_pMaterial)
		m_pMaterial = renderer.m_pMaterial->Clone();

	if (m_pMesh)
		m_pMesh->AddRef();

	if (m_pShader)
		m_pShader->AddRef();

	for (int i = 0; i < RST_END; ++i)
	{
		m_pRenderState[i] = renderer.m_pRenderState[i];
		if (m_pRenderState[i])
			m_pRenderState[i]->AddRef();
	}

	m_mapCBuffer.clear();

	unordered_map<int, PRENDERERCBUFFER>::const_iterator	iter;
	unordered_map<int, PRENDERERCBUFFER>::const_iterator	iterEnd = renderer.m_mapCBuffer.end();

	for (iter = renderer.m_mapCBuffer.begin(); iter != iterEnd; ++iter)
	{
		PRENDERERCBUFFER	pBuffer = new RENDERERCBUFFER;

		*pBuffer = *iter->second;
		pBuffer->pData = new char[pBuffer->iSize];

		m_mapCBuffer.insert(make_pair(pBuffer->iRegister, pBuffer));
	}

	ANIMATION2DBUFFER	tBuffer = {};
	tBuffer.iType = A2D_NONE;
	UpdateCBuffer("Animation2D", 10, sizeof(ANIMATION2DBUFFER),
		SCT_VERTEX | SCT_PIXEL, &tBuffer);
}

CRenderer2D::~CRenderer2D()
{
	unordered_map<int, PRENDERERCBUFFER>::iterator	iter;
	unordered_map<int, PRENDERERCBUFFER>::iterator	iterEnd = m_mapCBuffer.end();

	for (iter = m_mapCBuffer.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE_ARRAY(iter->second->pData);
		SAFE_DELETE(iter->second);
	}

	m_mapCBuffer.clear();

	for (int i = 0; i < RST_END; ++i)
	{
		SAFE_RELEASE(m_pRenderState[i]);
	}
	SAFE_RELEASE(m_pMaterial);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
}

CMaterial * CRenderer2D::GetMaterial() const
{
	m_pMaterial->AddRef();
	return m_pMaterial;
}

bool CRenderer2D::CreateCBuffer(const string & strName,
	int iRegister, int iSize, int iShaderType)
{
	PRENDERERCBUFFER	pBuffer = FindCBuffer(iRegister);

	if (pBuffer)
		return false;

	pBuffer = new RENDERERCBUFFER;

	pBuffer->iSize = iSize;
	pBuffer->iRegister = iRegister;
	pBuffer->iShaderType = iShaderType;
	pBuffer->strName = strName;
	pBuffer->pData = new char[pBuffer->iSize];

	m_mapCBuffer.insert(make_pair(pBuffer->iRegister, pBuffer));

	return true;
}

PRENDERERCBUFFER CRenderer2D::FindCBuffer(int iRegister)
{
	unordered_map<int, PRENDERERCBUFFER>::iterator	iter = m_mapCBuffer.find(iRegister);

	if (iter == m_mapCBuffer.end())
		return NULL;

	return iter->second;
}

void CRenderer2D::UpdateCBuffer(const string & strName,
	int iRegister, int iSize, int iShaderType, void * pData)
{
	PRENDERERCBUFFER	pBuffer = FindCBuffer(iRegister);

	if (!pBuffer)
	{
		CreateCBuffer(strName, iRegister, iSize, iShaderType);
		pBuffer = FindCBuffer(iRegister);
	}

	memcpy(pBuffer->pData, pData, pBuffer->iSize);
}

void CRenderer2D::SetMesh(const string & strKey)
{
	SAFE_RELEASE(m_pMesh);
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh(strKey);
}

void CRenderer2D::SetMesh(CMesh * pMesh)
{
	SAFE_RELEASE(m_pMesh);
	pMesh->AddRef();
	m_pMesh = pMesh;
}

void CRenderer2D::SetShader(const string & strKey)
{
	SAFE_RELEASE(m_pShader);
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(strKey);
}

void CRenderer2D::SetShader(CShader * pShader)
{
	SAFE_RELEASE(m_pShader);
	pShader->AddRef();
	m_pShader = pShader;
}

void CRenderer2D::SetInputLayout(const string & strKey)
{
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout(strKey);
	m_strLayoutKey = strKey;
}

void CRenderer2D::SetInputLayout(ID3D11InputLayout * pLayout)
{
	m_pLayout = pLayout;
}

CMaterial * CRenderer2D::CreateMaterial(const string & strSmpKey,
	const string & strDifKey, int iRegTex, int iRegSmp,
	const wchar_t * pFileName, const string & strPathKey)
{
	SAFE_RELEASE(m_pMaterial);

	m_pMaterial = new CMaterial;

	if (!m_pMaterial->Init())
	{
		SAFE_RELEASE(m_pMaterial);
		return NULL;
	}

	m_pMaterial->SetDiffuseTexInfo(strSmpKey, strDifKey, iRegTex, iRegSmp,
		pFileName, strPathKey);

	m_pMaterial->AddRef();

	return m_pMaterial;
}

CMaterial * CRenderer2D::CreateMaterial(const string & strSmpKey,
	const string & strDifKey, int iRegTex, int iRegSmp,
	const vector<wstring>* vecFileName, const string & strPathKey)
{
	SAFE_RELEASE(m_pMaterial);

	m_pMaterial = new CMaterial;

	if (!m_pMaterial->Init())
	{
		SAFE_RELEASE(m_pMaterial);
		return NULL;
	}

	m_pMaterial->SetDiffuseTexInfo(strSmpKey, strDifKey, iRegTex, iRegSmp,
		vecFileName, strPathKey);

	m_pMaterial->AddRef();

	return m_pMaterial;
}

void CRenderer2D::SetRenderState(const string & strKey)
{
	CRenderState*	pState = GET_SINGLE(CRenderManager)->FindRenderState(strKey);

	if (!pState)
		return;

	SAFE_RELEASE(m_pRenderState[pState->GetType()]);
	m_pRenderState[pState->GetType()] = pState;
}

bool CRenderer2D::Init()
{
	SetRenderState(DEPTH_DISABLE);

	CreateCBuffer("Animation2D", 10, sizeof(ANIMATION2DBUFFER),
		SCT_VERTEX | SCT_PIXEL);

	ANIMATION2DBUFFER	tBuffer = {};
	tBuffer.iType = A2D_NONE;
	UpdateCBuffer("Animation2D", 10, sizeof(ANIMATION2DBUFFER),
		SCT_VERTEX | SCT_PIXEL, &tBuffer);

	m_pTransform->SetOriginAxis(AXIS_X);

	return true;
}

void CRenderer2D::Input(float fTime)
{
}

int CRenderer2D::Update(float fTime)
{
	return 0;
}

int CRenderer2D::LateUpdate(float fTime)
{
	return 0;
}

void CRenderer2D::Collision(float fTime)
{
}

void CRenderer2D::Render(float fTime)
{
	UpdateTransform();

	unordered_map<int, PRENDERERCBUFFER>::iterator	iter;
	unordered_map<int, PRENDERERCBUFFER>::iterator	iterEnd = m_mapCBuffer.end();

	for (iter = m_mapCBuffer.begin(); iter != iterEnd; ++iter)
	{
		GET_SINGLE(CShaderManager)->UpdateCBuffer(iter->second->strName,
			iter->second->pData, iter->second->iShaderType);
	}


	// �������� ����
	for (int i = 0; i < RST_END; ++i)
	{
		if (m_pRenderState[i])
			m_pRenderState[i]->SetState();
	}

	if (m_pMaterial)
		m_pMaterial->SetMaterial(SCT_PIXEL);

	GET_SINGLE(CShaderManager)->SetInputLayout(m_strLayoutKey);
	//CONTEXT->IASetInputLayout(m_pLayout);
	m_pShader->SetShader();
	m_pMesh->Render(fTime);

	// �������� ����
	for (int i = 0; i < RST_END; ++i)
	{
		if (m_pRenderState[i])
			m_pRenderState[i]->ResetState();
	}
}

CRenderer2D * CRenderer2D::Clone()
{
	return new CRenderer2D(*this);
}

void CRenderer2D::UpdateTransform()
{
	TRANSFORMCBUFFER	tBuffer = {};

	XMMATRIX	matView, matProj;
	CCamera*	pCamera = m_pScene->GetMainCamera();

	matView = pCamera->GetViewMatrix().mat;
	matProj = pCamera->GetProjMatrix().mat;

	SAFE_RELEASE(pCamera);

	tBuffer.matWorld = m_pTransform->GetLocalMatrix() *
		m_pTransform->GetWorldMatrix();
	tBuffer.matView = matView;
	tBuffer.matProj = matProj;
	tBuffer.matWV = tBuffer.matWorld * tBuffer.matView;
	tBuffer.matWVP = tBuffer.matWV * tBuffer.matProj;
	tBuffer.matWP = tBuffer.matWorld * tBuffer.matProj;
	tBuffer.vPivot = m_pTransform->GetPivot();
	tBuffer.vLength = m_pMesh->GetLength();

	tBuffer.matWorld = XMMatrixTranspose(tBuffer.matWorld.mat);
	tBuffer.matView = XMMatrixTranspose(tBuffer.matView.mat);
	tBuffer.matProj = XMMatrixTranspose(tBuffer.matProj.mat);
	tBuffer.matWV = XMMatrixTranspose(tBuffer.matWV.mat);
	tBuffer.matWVP = XMMatrixTranspose(tBuffer.matWVP.mat);
	tBuffer.matWP = XMMatrixTranspose(tBuffer.matWP.mat);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&tBuffer, SCT_VERTEX | SCT_PIXEL);
}
