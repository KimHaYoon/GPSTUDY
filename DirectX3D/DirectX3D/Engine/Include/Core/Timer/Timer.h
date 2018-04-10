#pragma once
#include "../Obj.h"

GP_BEGIN

class GP_DLL CTimer :
	public CObj
{
private:
	friend class CTimerManager;

private:
	CTimer();
	~CTimer();

private:
	LARGE_INTEGER	m_tTime;
	LARGE_INTEGER	m_tSecond;
	float	m_fDeltaTime;

	float	m_fFPS;
	float	m_fFPSTime;
	int		m_iFrame;
	int		m_iFrameMax;

public:
	float GetDeltaTime()	const;
	float GetFPS()	const;

public:
	bool Init();
	void Update();
};

GP_END