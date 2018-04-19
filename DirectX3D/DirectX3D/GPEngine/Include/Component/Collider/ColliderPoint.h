#pragma once

#include "Collider.h"

GP_BEGIN

class GP_DLL CColliderPoint :
	public CCollider
{
private:
	friend class CGameObject;

private:
	CColliderPoint();
	CColliderPoint(const CColliderPoint& coll);
	~CColliderPoint();

private:
	Vector2		m_vPos;

public:
	void SetPos(const Vector3& vPos);
	void SetPos(const Vector2& vPos);
	void SetPos(float x, float y);
	void SetPos(const POINT& ptPos);

public:
	Vector2 GetPos()	const;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CColliderPoint* Clone();

public:
	virtual bool Collision(CCollider* pDest);
};

GP_END