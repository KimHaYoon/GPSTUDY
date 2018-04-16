#pragma once
#include "../Core/Obj.h"

GP_BEGIN

class GP_DLL CLayer :
	public CObj
{
private:
	friend class CScene;

private:
	CLayer();
	~CLayer();

private:
	list<class CGameObject*>		m_ObjList;
	class CScene*					m_pScene;
	int								m_iZOrder;

public:
	void SetZOrder(int iZOrder);
	
public:
	int	 GetZOrder()	const;

public:
	bool Init();
	void Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);
};

GP_END