#pragma once

#include "../Component.h"

GP_BEGIN

class GP_DLL CAnimation2D :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CAnimation2D();
	CAnimation2D(const CAnimation2D& ani);
	~CAnimation2D();

private:
	unordered_map<string, PANIMATIONCLIP2D>	m_mapClip;
	PANIMATIONCLIP2D		m_pCurClip;
	string					m_strCurrentAnim;
	string					m_strDefaultAnim;
	string					m_strNextAnim;
	ANIMATION2DBUFFER		m_tCBuffer;
	bool					m_bClipEnd;
	bool					m_bClipFix;
	vector<string>			m_vecNextClip;
	int						m_iCount;

public:
	bool GetClipEnd() const;
	string GetCurrentAnimKey()	const;

public:
	bool CreateClip(const string& strKey, ANIMATION2D_TYPE eType,
		ANIMATION2D_OPTION eOption, int iFrameMaxX, int iFrameMaxY,
		int iLengthX, int iLengthY, int iStartY, float fAnimLimitTime,
		int iCountLimit, float fOptionTimeLimit, const string& strTexKey,
		TCHAR* pFileName = NULL, const string& NextClipKey = " ", const string& strPathKey = TEXTURE_PATH);
	bool CreateClip(const string& strKey, ANIMATION2D_TYPE eType,
		ANIMATION2D_OPTION eOption, int iFrameMaxX, int iFrameMaxY,
		int iLengthX, int iLengthY, int iStartY, float fAnimLimitTime,
		int iCountLimit, float fOptionTimeLimit, const string& strTexKey,
		const vector<wstring>* vecFileName = NULL, const string& NextClipKey = " ", const string& strPathKey = TEXTURE_PATH);
	bool AddClip(const string& strKey, PANIMATIONCLIP2D pClip);

	bool ChangeAnimation(const string& strKey);
	void SetDefaultAnim(const string& strKey);
	void ReturnDefaultAnimation();
	void SetShader();

	void AddNextClip(const string& strKey);
	void SetFrameX(int iFrameX);
	void SetFrameY(int iFrameY);

private:
	PANIMATIONCLIP2D FindClip(const string& strKey);
	bool ChangeAnimation();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CAnimation2D* Clone();
};

GP_END