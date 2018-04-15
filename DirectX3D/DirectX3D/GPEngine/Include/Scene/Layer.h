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

};

GP_END