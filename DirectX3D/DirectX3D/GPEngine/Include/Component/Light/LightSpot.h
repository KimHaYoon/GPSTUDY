#pragma once
#include "Light.h"

GP_BEGIN

class GP_DLL CLightSpot :
	public CLight
{
private:
	friend class CGameObject;

private:
	CLightSpot();
	CLightSpot(const CLightSpot& light);
	~CLightSpot();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CLightSpot* Clone();
};

GP_END