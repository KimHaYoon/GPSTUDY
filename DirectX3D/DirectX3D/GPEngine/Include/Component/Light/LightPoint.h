#pragma once
#include "Light.h"

GP_BEGIN

class GP_DLL CLightPoint :
	public CLight
{
private:
	friend class CGameObject;

private:
	CLightPoint();
	CLightPoint(const CLightPoint& light);
	~CLightPoint();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CLightPoint* Clone();
};

GP_END