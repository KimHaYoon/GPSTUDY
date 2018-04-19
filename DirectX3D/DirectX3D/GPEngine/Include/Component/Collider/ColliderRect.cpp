#include "ColliderRect.h"
#include "ColliderPoint.h"
#include "ColliderOBB2D.h"
#include "ColliderSphere.h"
#include "../Camera/Camera.h"
#include "../Transform/Transform.h"
#include "../../Resources/Mesh/Mesh.h"
#include "../../Resources/ResourcesManager.h"
#include "../../Rendering/Shader/Shader.h"
#include "../../Rendering/Shader/ShaderManager.h"
#include "../../Scene/Scene.h"


GP_USING

CColliderRect::CColliderRect()
{
	SetTag("ColliderRect");
	SetTypeName("CColliderRect");
	SetTypeID<CColliderRect>();
	m_eCollType = CT_RECT;
	m_bMove = true;
}

CColliderRect::CColliderRect(const CColliderRect & coll) :
	CCollider(coll)
{
	m_tInfo = coll.m_tInfo;
}

CColliderRect::~CColliderRect()
{
}

void CColliderRect::SetRect(float l, float t, float w, float h)
{
	m_tInfo = RECTINFO(l, t, l + w, t + h);
}

void CColliderRect::SetMove(bool bMove)
{
	m_bMove = bMove;
}

RECTINFO CColliderRect::GetInfo() const
{
	return m_tInfo;
}

bool CColliderRect::Init()
{
	CCollider::Init();

#ifdef _DEBUG
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("RectOrthoLTLine");
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(COLLIDER_SHADER);
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout("VertexColor");
#endif // _DEBUG

	return true;
}

void CColliderRect::Input(float fTime)
{
}

int CColliderRect::Update(float fTime)
{
	CCollider::Update(fTime);

	if (m_bMove)
	{
		m_tInfo.l += m_vMove.x;
		m_tInfo.r += m_vMove.x;
		m_tInfo.t += m_vMove.y;
		m_tInfo.b += m_vMove.y;
	}

	return 0;
}

int CColliderRect::LateUpdate(float fTime)
{
	return 0;
}

void CColliderRect::Collision(float fTime)
{
}

void CColliderRect::Render(float fTime)
{
#ifdef _DEBUG
	Matrix matScale, matTrans;
	float x = m_tInfo.r - m_tInfo.l;
	float y = m_tInfo.b - m_tInfo.t;

	matScale.mat = XMMatrixScaling(x, y, 1.f);
	matTrans.mat = XMMatrixTranslation(m_tInfo.l, m_tInfo.t, 0.f);

	CCamera* pCamera = m_pScene->GetMainCamera();

	m_tTransform.matWorld = matScale * matTrans;
	m_tTransform.matView = pCamera->GetViewMatrix();
	m_tTransform.matProj = pCamera->GetProjMatrix();
	m_tTransform.matWV = m_tTransform.matWorld * m_tTransform.matView;
	m_tTransform.matWVP = m_tTransform.matWV * m_tTransform.matProj;
	m_tTransform.vPivot = m_pTransform->GetPivot();
	m_tTransform.vLength = Vector3::One;
	m_tTransform.vLength.z = 0.f;

	m_tTransform.matWorld = XMMatrixTranspose(m_tTransform.matWorld.mat);
	m_tTransform.matView = XMMatrixTranspose(m_tTransform.matView.mat);
	m_tTransform.matProj = XMMatrixTranspose(m_tTransform.matProj.mat);
	m_tTransform.matWV = XMMatrixTranspose(m_tTransform.matWV.mat);
	m_tTransform.matWVP = XMMatrixTranspose(m_tTransform.matWVP.mat);

	SAFE_RELEASE(pCamera);
#endif // _DEBUG
	CCollider::Render(fTime);
}

CColliderRect * CColliderRect::Clone()
{
	return new CColliderRect(*this);
}

bool CColliderRect::Collision(CCollider * pDest)
{
	switch (pDest->GetColliderType())
	{
	case CT_RECT:
		return CollisionRectToRect(m_tInfo, ((CColliderRect*)pDest)->m_tInfo);
	case CT_POINT:
		return CollisionRectToPoint(m_tInfo, ((CColliderPoint*)pDest)->GetPos());
	/*case CT_OBB2D:
		return CollisionObbToRect(((CColliderOBB2D*)pDest)->GetInfo(), m_tInfo);*/
	case CT_SPHERE:
		return CollisionRectToSphere(m_tInfo, ((CColliderSphere*)pDest)->GetSphere());
	}

	return false;
}
