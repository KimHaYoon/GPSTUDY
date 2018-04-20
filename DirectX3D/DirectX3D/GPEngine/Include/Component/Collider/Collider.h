#pragma once
#include "../Component.h"

GP_BEGIN

class GP_DLL CCollider :
	public CComponent
{
protected:
	CCollider();
	CCollider(const CCollider& coll);
	virtual ~CCollider() = 0;

protected:
	COLLIDER_TYPE		m_eCollType;
	list<CCollider*>	m_CollList;
	Vector3				m_vPrevPos;
	Vector3				m_vMove;

	string				m_strGroup;
	Vector3				m_vCollMin;
	Vector3				m_vCollMax;
	vector<int>			m_vecSectionIndex;

#ifdef _DEBUG
	class CMesh*		m_pMesh;
	class CShader*		m_pShader;
	ID3D11InputLayout*	m_pLayout;
	Vector4				m_vColor;
	TRANSFORMCBUFFER	m_tTransform;
	VIEW_TYPE			m_eViewType;
#endif // _DEBUG

public:
	Vector3 GetColliderMin()	const;
	Vector3 GetColliderMax()	const;
	COLLIDER_TYPE GetColliderType() const;
	string GetCollisionGroup()	const;

public:
	void AddCollList(CCollider* pCollider);
	bool CheckCollList(CCollider* pCollider);
	void EraseCollList(CCollider* pCollider);
	void SetViewType(VIEW_TYPE eType);
	void SetCollisionGroup(const string& strGroup);
	void AddSectionIndex(int iIndex);
	void ClearSectionIndex();
	void CheckCollisionSection(float fTime);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CCollider* Clone() = 0;

public:
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest, float fTime);
	virtual void OnCollisionTrigger(class CCollider* pSrc, class CCollider* pDest, float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest, float fTime);

public:
	virtual bool Collision(CCollider* pDest) = 0;

protected:
	// 2D
	bool CollisionRectToRect(const RECTINFO& rc1, const RECTINFO& rc2);
	bool CollisionRectToPoint(const RECTINFO& rc, const Vector2& point);
	bool CollisionObbToObb(const OBB2DINFO& tObb1, const OBB2DINFO& tObb2);
	bool CollisionObbToRect(const OBB2DINFO& tObb, const RECTINFO& rc);
	bool CollisionSphereToSphere(const SPHERE& tSphere1, const SPHERE& tSphere2);
	bool CollisionRectToSphere(RECTINFO& rc, const SPHERE& tSphere);
	bool CollisionSphereToPoint(const SPHERE& tSphere, const Vector2& point);
	bool CollisionEllipseToPoint(const ELLIPSE& tEllipse, const Vector2& point);

	//3D
	bool CollisionAABBToAABB(const AABB& src, const AABB& dest);
};

GP_END