#include "ColliderEllipse.h"
#include "ColliderRect.h"
#include "ColliderPoint.h"
#include "../Camera/Camera.h"
#include "../Transform/Transform.h"
#include "../../Resources/Mesh/Mesh.h"
#include "../../Resources/ResourcesManager.h"
#include "../../Rendering/Shader/Shader.h"
#include "../../Rendering/Shader/ShaderManager.h"
#include "../../Scene/Scene.h"

GP_USING

CColliderEllipse::CColliderEllipse()
{
	SetTag("ColliderEllipse");
	SetTypeName("CColliderEllipse");
	SetTypeID<CColliderEllipse>();
	m_eCollType = CT_ELLIPSE;
}

CColliderEllipse::CColliderEllipse(const CColliderEllipse & coll) : 
	CCollider(coll)
{
	m_tEllipse = coll.m_tEllipse;
	m_tOriginEllipse = coll.m_tOriginEllipse;
	m_eCollType = CT_ELLIPSE;
}


CColliderEllipse::~CColliderEllipse()
{
}

ELLIPSE CColliderEllipse::GetEllipse() const
{
	return m_tEllipse;
}

ELLIPSE CColliderEllipse::GetOriginEllipse() const
{
	return m_tOriginEllipse;
}

void CColliderEllipse::SetEllipse(float x, float y, float z, float a, float b)
{
	ELLIPSE tEllipse;
	tEllipse.vCenter = Vector3(x, y, z);
	tEllipse.fA = a;
	tEllipse.fB = b;

	SetEllipse(tEllipse);
}

void CColliderEllipse::SetEllipse(const Vector3 & vPos, float a, float b)
{
	ELLIPSE tEllipse;
	tEllipse.vCenter = vPos;
	tEllipse.fA = a;
	tEllipse.fB = b;

	SetEllipse(tEllipse);
}

void CColliderEllipse::SetEllipse(const ELLIPSE & tEllipse)
{
	m_tEllipse = tEllipse;
}

void CColliderEllipse::SetOriginEllipse(const Vector3 & vPos, float a, float b)
{
	m_tOriginEllipse.vCenter = vPos;
	m_tOriginEllipse.fA = a;
	m_tOriginEllipse.fA = b;
}

bool CColliderEllipse::Init()
{
	CCollider::Init();

#ifdef _DEBUG
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("EllipseOrthoLine");
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(COLLIDER_SHADER);
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout("VertexColor");
#endif // _DEBUG

	return true;
}

void CColliderEllipse::Input(float fTime)
{
}

int CColliderEllipse::Update(float fTime)
{
	CCollider::Update(fTime);

	m_tEllipse.vCenter += m_vMove;

	return 0;
}

int CColliderEllipse::LateUpdate(float fTime)
{
	return 0;
}

void CColliderEllipse::Collision(float fTime)
{
}

void CColliderEllipse::Render(float fTime)
{
#ifdef _DEBUG
	Matrix matScale, matTrans;

	matScale.mat = XMMatrixScaling(m_tEllipse.fA, m_tEllipse.fB, 1.f);
	matTrans.mat = XMMatrixTranslation(m_tEllipse.vCenter.x, m_tEllipse.vCenter.y, 0.f);

	CCamera* pCamera = m_pScene->GetMainCamera();

	m_tTransform.matWorld = matScale * matTrans;
	m_tTransform.matView = pCamera->GetViewMatrix();
	m_tTransform.matProj = pCamera->GetProjMatrix();
	m_tTransform.matWV = m_tTransform.matWorld * m_tTransform.matView;
	m_tTransform.matWVP = m_tTransform.matWV * m_tTransform.matProj;
	m_tTransform.vPivot = m_pTransform->GetPivot();
	m_tTransform.vLength = Vector3(2.f, 2.f, 0.f);

	m_tTransform.matWorld = XMMatrixTranspose(m_tTransform.matWorld.mat);
	m_tTransform.matView = XMMatrixTranspose(m_tTransform.matView.mat);
	m_tTransform.matProj = XMMatrixTranspose(m_tTransform.matProj.mat);
	m_tTransform.matWV = XMMatrixTranspose(m_tTransform.matWV.mat);
	m_tTransform.matWVP = XMMatrixTranspose(m_tTransform.matWVP.mat);

	SAFE_RELEASE(pCamera);
#endif // _DEBUG
	CCollider::Render(fTime);
}

CColliderEllipse * CColliderEllipse::Clone()
{
	return new CColliderEllipse(*this);
}

bool CColliderEllipse::Collision(CCollider * pDest)
{
	switch (pDest->GetColliderType())
	{
	case CT_POINT:
		return CollisionEllipseToPoint(m_tEllipse, ((CColliderPoint*)pDest)->GetPos());
	}

	return false;
}
