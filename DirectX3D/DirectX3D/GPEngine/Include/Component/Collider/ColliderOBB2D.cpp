#include "ColliderOBB2D.h"
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

CColliderOBB2D::CColliderOBB2D()
{
	SetTag("ColliderOBB2D");
	SetTypeName("CColliderOBB2D");
	SetTypeID<CColliderOBB2D>();
	m_eCollType = CT_OBB2D;
}

CColliderOBB2D::CColliderOBB2D(const CColliderOBB2D & coll) :
	CCollider(coll)
{
	m_eCollType = CT_OBB2D;
	m_tInfo = coll.m_tInfo;
	m_tOriginInfo = coll.m_tOriginInfo;
}


CColliderOBB2D::~CColliderOBB2D()
{
}

OBB2DINFO CColliderOBB2D::GetInfo() const
{
	return m_tInfo;
}

OBB2DINFO CColliderOBB2D::GetOriginInfo() const
{
	return m_tOriginInfo;
}

void CColliderOBB2D::SetInfo(const Vector3 & vCenter, const Vector3 & vAxisX, const Vector3 & vAxisY, float fLengthX, float fLengthY)
{
	m_tInfo.vCenter	= vCenter; 
	m_tInfo.vAxisX = vAxisX; 
	m_tInfo.vAxisY = vAxisY; 
	m_tInfo.fLengthX = fLengthX;
	m_tInfo.fLengthY = fLengthY;
}

void CColliderOBB2D::SetOriginInfo(const Vector3 & vCenter, const Vector3 & vAxisX, const Vector3 & vAxisY, float fLengthX, float fLengthY)
{
	m_tOriginInfo.vCenter = vCenter;
	m_tOriginInfo.vAxisX = vAxisX;
	m_tOriginInfo.vAxisY = vAxisY;
	m_tOriginInfo.fLengthX = fLengthX;
	m_tOriginInfo.fLengthY = fLengthY;
	m_tInfo.fLengthX = fLengthX;
	m_tInfo.fLengthY = fLengthY;
}

bool CColliderOBB2D::Init()
{
	CCollider::Init();

#ifdef _DEBUG
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("RectOrthoLTLine");
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(COLLIDER_SHADER);
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout("VertexColor");
#endif // _DEBUG

	return true;
}

void CColliderOBB2D::Input(float fTime)
{
}

int CColliderOBB2D::Update(float fTime)
{
	return 0;
}

int CColliderOBB2D::LateUpdate(float fTime)
{
	Matrix	matRot = m_pTransform->GetWorldRotMatrix();
	Matrix	matPos = m_pTransform->GetWorldPosMatrix();
	Matrix	matWorld = matRot * matPos;

	m_tInfo.vCenter = m_tOriginInfo.vCenter.TransformCoord(matWorld.mat);
	m_tInfo.vAxisX = m_tOriginInfo.vAxisX.TransformNormal(matWorld.mat).Normalize();
	m_tInfo.vAxisY = m_tOriginInfo.vAxisY.TransformNormal(matWorld.mat).Normalize();

	return 0;
}

void CColliderOBB2D::Collision(float fTime)
{
}

void CColliderOBB2D::Render(float fTime)
{
#ifdef _DEBUG
	Matrix	matScale, matTrans;
	matScale.mat = XMMatrixScaling(m_tInfo.fLengthX * 2.f,
		m_tInfo.fLengthY * 2.f, 1.f);
	matTrans.mat = XMMatrixTranslation(m_tInfo.vCenter.x,
		m_tInfo.vCenter.y, 0.f);

	CCamera*	pCamera = m_pScene->GetMainCamera();

	m_tTransform.matWorld = matScale *
		m_pTransform->GetWorldRotMatrix() * matTrans;

	if (m_eViewType == VT_NORMAL)
		m_tTransform.matView = pCamera->GetViewMatrix();

	else
		m_tTransform.matView = XMMatrixIdentity();

	m_tTransform.matProj = pCamera->GetProjMatrix();
	m_tTransform.matWV = m_tTransform.matWorld * m_tTransform.matView;
	m_tTransform.matWVP = m_tTransform.matWV * m_tTransform.matProj;
	m_tTransform.matWP = m_tTransform.matWorld * m_tTransform.matProj;
	m_tTransform.vPivot = Vector3(0.5f, 0.5f, 0.f);
	m_tTransform.vLength = Vector3::One;
	m_tTransform.vLength.z = 0.f;

	m_tTransform.matWorld = XMMatrixTranspose(m_tTransform.matWorld.mat);
	m_tTransform.matView = XMMatrixTranspose(m_tTransform.matView.mat);
	m_tTransform.matProj = XMMatrixTranspose(m_tTransform.matProj.mat);
	m_tTransform.matWV = XMMatrixTranspose(m_tTransform.matWV.mat);
	m_tTransform.matWVP = XMMatrixTranspose(m_tTransform.matWVP.mat);
	m_tTransform.matWP = XMMatrixTranspose(m_tTransform.matWP.mat);

	SAFE_RELEASE(pCamera);
#endif // _DEBUG

	CCollider::Render(fTime);
}

CColliderOBB2D * CColliderOBB2D::Clone()
{
	return new CColliderOBB2D(*this);
}

bool CColliderOBB2D::Collision(CCollider * pDest)
{
	switch (pDest->GetColliderType())
	{
	case CT_OBB2D:
		return CollisionObbToObb(m_tInfo, ((CColliderOBB2D*)pDest)->m_tInfo);
	case CT_RECT:
		return CollisionObbToRect(m_tInfo, ((CColliderRect*)pDest)->GetInfo());
	default:
		break;
	}
	return false;
}
