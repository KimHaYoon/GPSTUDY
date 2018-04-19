#include "Collider.h"
#include "../Transform/Transform.h"
#include "../../Resources/Mesh/Mesh.h"
#include "../../Resources/ResourcesManager.h"
#include "../../Rendering/Shader/Shader.h"
#include "../../Rendering/Shader/ShaderManager.h"
#include "../../Scene/Scene.h"

GP_USING

CCollider::CCollider()
{
	m_eType = CT_COLLIDER;

#ifdef _DEBUG
	m_pMesh = NULL;
	m_pShader = NULL;
	m_pLayout = NULL;
	m_vColor = Vector4::Green;
	m_eViewType = VT_NORMAL;
#endif // _DEBUG

	m_strGroup = "Default";
}

CCollider::CCollider(const CCollider & coll) :
	CComponent(coll)
{
	m_eCollType = coll.m_eCollType;
	m_strGroup = coll.m_strGroup;
#ifdef _DEBUG
	m_pMesh = coll.m_pMesh;
	m_pShader = coll.m_pShader;
	m_pLayout = coll.m_pLayout;
	m_eViewType = coll.m_eViewType;

	if (m_pMesh)
		m_pMesh->AddRef();

	if (m_pShader)
		m_pShader->AddRef();
#endif // _DEBUG
}


CCollider::~CCollider()
{
	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_CollList.end();

	for (iter = m_CollList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->EraseCollList(this);
	}

#ifdef _DEBUG
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
#endif // _DEBUG
}

Vector3 CCollider::GetColliderMin() const
{
	return m_vCollMin;
}

Vector3 CCollider::GetColliderMax() const
{
	return m_vCollMax;
}

COLLIDER_TYPE CCollider::GetColliderType() const
{
	return m_eCollType;
}

string CCollider::GetCollisionGroup() const
{
	return m_strGroup;
}

void CCollider::AddCollList(CCollider * pCollider)
{
	if (this == pCollider)
		return;

	m_CollList.push_back(pCollider);
}

bool CCollider::CheckCollList(CCollider * pCollider)
{
	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_CollList.end();

	for (iter = m_CollList.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pCollider)
			return true;
	}

	return false;
}

void CCollider::EraseCollList(CCollider * pCollider)
{
	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_CollList.end();

	for (iter = m_CollList.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pCollider)
		{
			m_CollList.erase(iter);
			return;
		}
	}
}

void CCollider::SetViewType(VIEW_TYPE eType)
{
#ifdef _DEBUG
	m_eViewType = eType;
#endif // _DEBUG

}

void CCollider::SetCollisionGroup(const string & strGroup)
{
	m_strGroup = strGroup;
}

bool CCollider::Init()
{
	return true;
}

void CCollider::Input(float fTime)
{
}

int CCollider::Update(float fTime)
{
	Vector3 vPos = m_pTransform->GetWorldPos();

	m_vMove = vPos - m_vPrevPos;

	m_vPrevPos = vPos;

	return 0;
}

int CCollider::LateUpdate(float fTime)
{
#ifdef _DEBUG
	m_vColor = Vector4::Green;
#endif // _DEBUG

	return 0;
}

void CCollider::Collision(float fTime)
{
}

void CCollider::Render(float fTime)
{
#ifdef _DEBUG
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&m_tTransform, SCT_VERTEX | SCT_PIXEL);
	GET_SINGLE(CShaderManager)->UpdateCBuffer("ColliderColor",
		&m_vColor, SCT_VERTEX);

	m_pShader->SetShader();
	GET_SINGLE(CShaderManager)->SetInputLayout("VertexColor");
	m_pMesh->Render(fTime);
#endif // _DEBUG
}

void CCollider::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
#ifdef _DEBUG
	m_vColor = Vector4::Red;
#endif
}

void CCollider::OnCollisionTrigger(CCollider * pSrc, CCollider * pDest, float fTime)
{
#ifdef _DEBUG
	m_vColor = Vector4::Red;
#endif
}

void CCollider::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
#ifdef _DEBUG
	m_vColor = Vector4::Green;
#endif
}

bool CCollider::CollisionRectToRect(const RECTINFO & rc1, const RECTINFO & rc2)
{
	if (rc1.l > rc2.r)
		return false;

	else if (rc1.r < rc2.l)
		return false;

	else if (rc1.t > rc2.b)
		return false;

	else if (rc1.b < rc2.t)
		return false;

	return true;
}

bool CCollider::CollisionRectToPoint(const RECTINFO & rc, const Vector2 & point)
{
	if (rc.l > point.x)
		return false;

	else if (rc.r < point.x)
		return false;

	else if (rc.t > point.y)
		return false;

	else if (rc.b < point.y)
		return false;

	return true;
}

bool CCollider::CollisionObbToObb(const OBB2DINFO & tObb1, const OBB2DINFO & tObb2)
{
	float r1, r2;
	float fDist;

	Vector3 vCenterDir = tObb1.vCenter - tObb2.vCenter;
	Vector3 vAxis = tObb1.vAxisX;

	r1 = tObb1.fLengthX;
	r2 = abs(tObb2.vAxisX.Dot(vAxis)) * tObb2.fLengthX +
		abs(tObb2.vAxisY.Dot(vAxis)) * tObb2.fLengthY;
	fDist = abs(vCenterDir.Dot(vAxis));
	
	if (r1 + r2 < fDist)
		return false;

	vAxis = tObb1.vAxisY;
	r1 = tObb1.fLengthY;
	r2 = abs(tObb2.vAxisX.Dot(vAxis)) * tObb2.fLengthX +
		abs(tObb2.vAxisY.Dot(vAxis)) * tObb2.fLengthY;
	fDist = abs(vCenterDir.Dot(vAxis));

	if (r1 + r2 < fDist)
		return false;

	vAxis = tObb2.vAxisX;
	r1 = tObb2.fLengthX;
	r2 = abs(tObb1.vAxisX.Dot(vAxis)) * tObb1.fLengthX +
		abs(tObb1.vAxisY.Dot(vAxis)) * tObb1.fLengthY;
	fDist = abs(vCenterDir.Dot(vAxis));

	if (r1 + r2 < fDist)
		return false;

	vAxis = tObb2.vAxisY;
	r1 = tObb2.fLengthY;
	r2 = abs(tObb1.vAxisX.Dot(vAxis)) * tObb1.fLengthX +
		abs(tObb1.vAxisY.Dot(vAxis)) * tObb1.fLengthY;
	fDist = abs(vCenterDir.Dot(vAxis));

	if (r1 + r2 < fDist)
		return false;

	return true;
}

bool CCollider::CollisionObbToRect(const OBB2DINFO & tObb, const RECTINFO & rc)
{
	return false;
}

bool CCollider::CollisionSphereToSphere(const SPHERE & tSphere1, const SPHERE & tSphere2)
{
	return tSphere1.fRadius + tSphere2.fRadius >= tSphere1.vCenter.Distance(tSphere2.vCenter);
}

bool CCollider::CollisionRectToSphere(RECTINFO & rc, const SPHERE & tSphere)
{	
	RECTINFO checkRC = rc;

	// Center가 사각형 가로 범위 혹은 세로 범위 안에 들어오는지를 판단한다.
	if ((rc.l <= tSphere.vCenter.x && tSphere.vCenter.x <= rc.r) ||
		(rc.t <= tSphere.vCenter.y && tSphere.vCenter.y <= rc.b))
	{
		// 사각형을 반지름 크기만큼 확장한다.
		checkRC.l -= tSphere.fRadius;
		checkRC.t -= tSphere.fRadius;
		checkRC.r += tSphere.fRadius;
		checkRC.b += tSphere.fRadius;

		return CollisionRectToPoint(checkRC, Vector2(tSphere.vCenter.x, tSphere.vCenter.y));
	}

	// 꼭지점 4개 바깥 영역에 존재할 경우
	else
	{
		Vector2	tPos(rc.l, rc.t);

		if (CollisionSphereToPoint(tSphere, tPos))
			return true;

		tPos = Vector2(rc.r, rc.t);

		if (CollisionSphereToPoint(tSphere, tPos))
			return true;

		tPos = Vector2(rc.l, rc.b);

		if (CollisionSphereToPoint(tSphere, tPos))
			return true;

		tPos = Vector2(rc.r, rc.b);

		if (CollisionSphereToPoint(tSphere, tPos))
			return true;
	}

	return false; 
}

bool CCollider::CollisionSphereToPoint(const SPHERE & tSphere, const Vector2 & point)
{
	float fDist = tSphere.vCenter.Distance(Vector3(point.x, point.y, 0.f));

	return fDist <= tSphere.fRadius;
}

bool CCollider::CollisionEllipseToPoint(const ELLIPSE & tEllipse, const Vector2 & point)
{
	Vector2 TPos = { tEllipse.vCenter.x,  tEllipse.vCenter.y};
	Vector2 PPos = point;
	Vector2 ESize = { tEllipse.fA, tEllipse.fB };

	PPos -= TPos;
	ESize *= ESize;
	PPos *= PPos;

	return (PPos.x / ESize.x) + (PPos.y / ESize.y) <= 1;
}

bool CCollider::CollisionAABBToAABB(const AABB & src, const AABB & dest)
{
	if (src.vMin.x > dest.vMax.x)
		return false;

	else if (src.vMax.x < dest.vMin.x)
		return false;

	else if (src.vMin.y > dest.vMax.y)
		return false;

	else if (src.vMax.y < dest.vMin.y)
		return false;

	else if (src.vMin.z > dest.vMax.z)
		return false;

	else if (src.vMax.z < dest.vMin.z)
		return false;

	return true;
}
