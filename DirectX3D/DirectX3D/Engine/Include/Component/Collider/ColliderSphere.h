#pragma once

#include "Collider.h"

GP_BEGIN

class GP_DLL CColliderSphere :
	public CCollider
{
private:
	friend class CGameObject;

private:
	CColliderSphere();
	CColliderSphere(const CColliderSphere& coll);
	~CColliderSphere();

private:
	SPHERE		m_tSphere;

public:
	SPHERE	GetSphere() const;

public:
	void SetSphere(float x, float y, float z, float r);
	void SetSphere(const Vector3& vPos, float r);
	void SetSphere(const SPHERE& tSphere);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CColliderSphere* Clone();

public:
	virtual bool Collision(CCollider* pDest);
};

GP_END