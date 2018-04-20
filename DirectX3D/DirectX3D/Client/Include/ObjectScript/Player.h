#pragma once

#include "Component/Script/Script.h"

GP_USING

class CPlayer :
	public CScript
{
private:
	friend class CGameObject;

public:
	CPlayer();
	CPlayer(const CPlayer& player);
	~CPlayer();

private:
	float	m_fMoveSpeed;
	float	m_fRotSpeed;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual CPlayer* Clone();
};

