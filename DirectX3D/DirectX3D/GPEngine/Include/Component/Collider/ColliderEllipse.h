#pragma once
#include "Collider.h"

GP_BEGIN

class GP_DLL CColliderEllipse :
	public CCollider
{
private:
	friend class CGameObject;

private:
	CColliderEllipse();
	CColliderEllipse(const CColliderEllipse& coll);
	~CColliderEllipse();

private:
	ELLIPSE			m_tEllipse;
	ELLIPSE			m_tOriginEllipse;

public:
	ELLIPSE	GetEllipse() const;
	ELLIPSE	GetOriginEllipse() const;

public:
	void SetEllipse(float x, float y, float z, float a, float b);
	void SetEllipse(const Vector3& vPos, float a, float b);
	void SetEllipse(const ELLIPSE& tEllipse);

	void SetOriginEllipse(const Vector3& vPos, float a, float b);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CColliderEllipse* Clone();

public:
	virtual bool Collision(CCollider* pDest);
};

GP_END