#include "ColliderSphere.h"
#include "ColliderPoint.h"
#include "ColliderRect.h"
#include "../Camera/Camera.h"
#include "../Transform/Transform.h"
#include "../../Resources/Mesh/Mesh.h"
#include "../../Resources/ResourcesManager.h"
#include "../../Rendering/Shader/Shader.h"
#include "../../Rendering/Shader/ShaderManager.h"
#include "../../Scene/Scene.h"

GP_USING

CColliderSphere::CColliderSphere()
{
	SetTag("ColliderSphere");
	SetTypeName("CColliderSphere");
	SetTypeID<CColliderSphere>();
	m_eCollType = CT_SPHERE;
}

CColliderSphere::CColliderSphere(const CColliderSphere & coll) :
	CCollider(coll)
{
	m_tSphere = coll.m_tSphere;
	m_eCollType = CT_SPHERE;
}

CColliderSphere::~CColliderSphere()
{
}

SPHERE CColliderSphere::GetSphere() const
{
	return m_tSphere;
}

void CColliderSphere::SetSphere(float x, float y, float z, float r)
{
	SetSphere(Vector3(x, y, z), r);
}

void CColliderSphere::SetSphere(const Vector3 & vPos, float r)
{
	SPHERE tSphere;

	tSphere.vCenter = vPos;
	tSphere.fRadius = r;

	SetSphere(tSphere);
}

void CColliderSphere::SetSphere(const SPHERE & tSphere)
{
	m_tSphere = tSphere;
}

bool CColliderSphere::Init()
{
	CCollider::Init();

#ifdef _DEBUG
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("SphereOrthoLine");
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(COLLIDER_SHADER);
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout("VertexColor");
#endif // _DEBUG

	return true;
}

void CColliderSphere::Input(float fTime)
{
}

int CColliderSphere::Update(float fTime)
{
	CCollider::Update(fTime);

	m_tSphere.vCenter.x += m_vMove.x;
	m_tSphere.vCenter.y += m_vMove.y;

	return 0;
}

int CColliderSphere::LateUpdate(float fTime)
{
	return 0;
}

void CColliderSphere::Collision(float fTime)
{
}

void CColliderSphere::Render(float fTime)
{
#ifdef _DEBUG
	Matrix matScale, matTrans;

	matScale.mat = XMMatrixScaling(m_tSphere.fRadius, m_tSphere.fRadius, 1.f);
	matTrans.mat = XMMatrixTranslation(m_tSphere.vCenter.x, m_tSphere.vCenter.y, 0.f);

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

CColliderSphere * CColliderSphere::Clone()
{
	return new CColliderSphere(*this);
}

bool CColliderSphere::Collision(CCollider * pDest)
{
	switch (pDest->GetColliderType())
	{
	case CT_SPHERE:
		return	CollisionSphereToSphere(m_tSphere, ((CColliderSphere*)pDest)->m_tSphere);

	case CT_RECT:
		return CollisionRectToSphere(((CColliderRect*)pDest)->GetInfo(), m_tSphere);

	case CT_POINT:
		return CollisionSphereToPoint(m_tSphere, ((CColliderPoint*)pDest)->GetPos());
	}

	return false;
}
