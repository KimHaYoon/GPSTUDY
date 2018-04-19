#include "Animation2D.h"
#include "../Renderer/Renderer2D.h"
#include "../Material/Material.h"
#include "../../Resources/Texture/Texture.h"
#include "../../Resources/ResourcesManager.h"
#include "../../GameObject/GameObject.h"
#include "../../Rendering/Shader/ShaderManager.h"

GP_USING

CAnimation2D::CAnimation2D() :
	m_pCurClip(NULL),
	m_bClipEnd(false),
	m_bClipFix(false)
{
	SetTag("Animation2D");
	SetTypeName("CAnimation2D");
	SetTypeID<CAnimation2D>();
	m_eType = CT_ANIMATION2D;
}

CAnimation2D::CAnimation2D(const CAnimation2D & ani) :
	CComponent(ani)
{
	unordered_map<string, PANIMATIONCLIP2D>::const_iterator iter;
	unordered_map<string, PANIMATIONCLIP2D>::const_iterator iterEnd = ani.m_mapClip.end();

	for (iter = ani.m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		PANIMATIONCLIP2D pClip = new ANIMATIONCLIP2D;

		*pClip = *iter->second;
		pClip->pTexture->AddRef();

		m_mapClip.insert(make_pair(iter->first, pClip));
	}

	vector<string>::const_iterator	iterN;
	vector<string>::const_iterator	iterNEnd = ani.m_vecNextClip.end();

	for (iterN = ani.m_vecNextClip.begin(); iterN != iterNEnd; ++iterN)
	{
		m_vecNextClip.push_back(*iterN);
	}

	m_strDefaultAnim = ani.m_strDefaultAnim;
	m_pCurClip = NULL;
	m_bClipEnd = false;
	m_bClipFix = false;
	ChangeAnimation(ani.m_strDefaultAnim);
	m_strNextAnim = ani.m_strNextAnim;
	m_iCount = 0;
}


CAnimation2D::~CAnimation2D()
{
	unordered_map<string, PANIMATIONCLIP2D>::iterator iter;
	unordered_map<string, PANIMATIONCLIP2D>::iterator iterEnd = m_mapClip.end();

	for (iter = m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second->pTexture);
	}

	Safe_Delete_Map(m_mapClip);
}

bool CAnimation2D::GetClipEnd() const
{
	return m_bClipEnd;
}

string CAnimation2D::GetCurrentAnimKey() const
{
	return m_strCurrentAnim;
}

bool CAnimation2D::CreateClip(const string & strKey, ANIMATION2D_TYPE eType, ANIMATION2D_OPTION eOption, int iFrameMaxX, int iFrameMaxY, int iLengthX, int iLengthY, int iStartY,
	float fAnimLimitTime, int iCountLimit, float fOptionTimeLimit, const string & strTexKey, TCHAR * pFileName, const string& NextClipKey, const string & strPathKey)
{
	if (FindClip(strKey))
		return false;

	PANIMATIONCLIP2D pClip = new ANIMATIONCLIP2D;

	pClip->eType = eType;
	pClip->eOption = eOption;
	pClip->iFrameMaxX = iFrameMaxX;
	pClip->iFrameMaxY = iFrameMaxY;
	pClip->fAnimLimitTime = fAnimLimitTime;
	pClip->iCountLimit = iCountLimit;
	pClip->fOptionTimeLimit = fOptionTimeLimit;
	pClip->iLengthX = iLengthX;
	pClip->iLengthY = iLengthY;
	pClip->iStartY = iStartY;
	pClip->iFrameY = iStartY;

	if (pFileName)
		pClip->pTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strTexKey,
			pFileName, strPathKey);

	else
		pClip->pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strTexKey);

	if (eOption == A2DO_NEXT)
	{
		m_vecNextClip.push_back(NextClipKey);
	}

	m_mapClip.insert(make_pair(strKey, pClip));

	if (m_strCurrentAnim.empty())
	{
		m_pCurClip = pClip;
		ChangeAnimation(strKey);
		m_strDefaultAnim = strKey;
	}

	return true;
}

bool CAnimation2D::CreateClip(const string & strKey, ANIMATION2D_TYPE eType, ANIMATION2D_OPTION eOption, int iFrameMaxX, int iFrameMaxY, int iLengthX, int iLengthY, int iStartY,
	float fAnimLimitTime, int iCountLimit, float fOptionTimeLimit, const string & strTexKey, const vector<wstring>* vecFileName, const string& NextClipKey, const string & strPathKey)
{
	if (FindClip(strKey))
		return false;

	PANIMATIONCLIP2D pClip = new ANIMATIONCLIP2D;

	pClip->eType = eType;
	pClip->eOption = eOption;
	pClip->iFrameMaxX = iFrameMaxX;
	pClip->iFrameMaxY = iFrameMaxY;
	pClip->fAnimLimitTime = fAnimLimitTime;
	pClip->iCountLimit = iCountLimit;
	pClip->fOptionTimeLimit = fOptionTimeLimit;
	pClip->iLengthX = iLengthX;
	pClip->iLengthY = iLengthY;
	pClip->iStartY = iStartY;
	pClip->iFrameY = iStartY;

	if (vecFileName)
		pClip->pTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strTexKey,
			*vecFileName, strPathKey);

	else
		pClip->pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strTexKey);

	if (eOption == A2DO_NEXT)
	{
		m_vecNextClip.push_back(NextClipKey);
	}

	m_mapClip.insert(make_pair(strKey, pClip));

	if (m_strCurrentAnim.empty())
	{
		m_pCurClip = pClip;
		ChangeAnimation(strKey);
		m_strDefaultAnim = strKey;
	}

	return true;
}

bool CAnimation2D::AddClip(const string & strKey, PANIMATIONCLIP2D pClip)
{
	if (FindClip(strKey))
		return false;

	m_mapClip.insert(make_pair(strKey, pClip));

	if (m_strCurrentAnim.empty())
	{
		m_pCurClip = pClip;
		ChangeAnimation(strKey);
		m_strDefaultAnim = strKey;
	}

	return true;
}

bool CAnimation2D::ChangeAnimation(const string & strKey)
{
	if (m_strCurrentAnim == strKey)
		return false;

	PANIMATIONCLIP2D pClip = FindClip(strKey);

	if (m_pCurClip)
	{
		m_pCurClip->iFrameX = 0;
		m_pCurClip->iFrameY = m_pCurClip->iStartY;
		m_pCurClip->iCount = 0;
		m_pCurClip->fAnimTime = 0.f;
		m_pCurClip->fOptionTime = 0.f;
	}

	m_pCurClip = pClip;

	m_pCurClip->iFrameY = m_pCurClip->iStartY;

	m_strCurrentAnim = strKey;

	// 오브젝트의 Render를 가지고 오고
	CRenderer2D* pRender = m_pGameObject->FindComponentFromTypeID<CRenderer2D>();

	// 재료인대.. 랜더의 재료를 받아온다.
	CMaterial* pMaterial = pRender->GetMaterial();

	pMaterial->SetDiffuseTex(m_pCurClip->pTexture);

	SAFE_RELEASE(pMaterial);

	// 상수버퍼 갱신
	m_tCBuffer.iType = m_pCurClip->eType;
	m_tCBuffer.iFrameX = m_pCurClip->iFrameX;
	m_tCBuffer.iFrameY = m_pCurClip->iFrameY;
	m_tCBuffer.iLengthX = m_pCurClip->iFrameMaxX;
	m_tCBuffer.iLengthY = m_pCurClip->iFrameMaxY;

	pRender->UpdateCBuffer("Animation2D", 10, sizeof(ANIMATION2DBUFFER), SCT_VERTEX | SCT_PIXEL, &m_tCBuffer);

	SAFE_RELEASE(pRender);
}

void CAnimation2D::SetDefaultAnim(const string & strKey)
{
	m_strDefaultAnim = strKey;
}

void CAnimation2D::ReturnDefaultAnimation()
{
	ChangeAnimation(m_strDefaultAnim);
}

void CAnimation2D::SetShader()
{
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Animation2D", &m_tCBuffer,
		SCT_VERTEX | SCT_PIXEL);
}

void CAnimation2D::AddNextClip(const string & strKey)
{
}

void CAnimation2D::SetFrameX(int iFrameX)
{
	m_pCurClip->iFrameX = iFrameX;
	m_bClipFix = true;
}

void CAnimation2D::SetFrameY(int iFrameY)
{
	m_pCurClip->iFrameY = iFrameY;
	m_bClipFix = true;
}

PANIMATIONCLIP2D CAnimation2D::FindClip(const string & strKey)
{
	unordered_map<string, PANIMATIONCLIP2D>::iterator iter = m_mapClip.find(strKey);

	if (iter == m_mapClip.end())
		return NULL;

	return iter->second;
}

bool CAnimation2D::ChangeAnimation()
{
	PANIMATIONCLIP2D pClip = FindClip(m_strCurrentAnim);

	if (m_pCurClip)
	{
		m_pCurClip->iFrameX = 0;
		m_pCurClip->iFrameY = m_pCurClip->iStartY;
		m_pCurClip->iCount = 0;
		m_pCurClip->fAnimTime = 0.f;
		m_pCurClip->fOptionTime = 0.f;
	}

	m_pCurClip = pClip;

	m_pCurClip->iFrameY = m_pCurClip->iStartY;

	// 오브젝트의 Render를 가지고 오고
	CRenderer2D* pRender = m_pGameObject->FindComponentFromTypeID<CRenderer2D>();

	// 재료인대.. 랜더의 재료를 받아온다.
	CMaterial* pMaterial = pRender->GetMaterial();

	pMaterial->SetDiffuseTex(m_pCurClip->pTexture);

	SAFE_RELEASE(pMaterial);

	// 상수버퍼 갱신
	m_tCBuffer.iType = m_pCurClip->eType;
	m_tCBuffer.iFrameX = m_pCurClip->iFrameX;
	m_tCBuffer.iFrameY = m_pCurClip->iFrameY;
	m_tCBuffer.iLengthX = m_pCurClip->iFrameMaxX;
	m_tCBuffer.iLengthY = m_pCurClip->iFrameMaxY;

	pRender->UpdateCBuffer("Animation2D", 10, sizeof(ANIMATION2DBUFFER), SCT_VERTEX | SCT_PIXEL, &m_tCBuffer);

	SAFE_RELEASE(pRender);

	return true;
}

bool CAnimation2D::Init()
{
	return true;
}

void CAnimation2D::Input(float fTime)
{
}

int CAnimation2D::Update(float fTime)
{
	m_bClipEnd = false;

	if (!m_pCurClip)
		return 0;

	if (m_bClipFix)
		return 0;

	m_pCurClip->fAnimTime += fTime;
	m_pCurClip->fOptionTime += fTime;

	int iMaxFrame = m_pCurClip->iLengthX * m_pCurClip->iLengthY;
	float fLimitTime = m_pCurClip->fAnimLimitTime / iMaxFrame;


	if (m_pCurClip->fAnimTime >= fLimitTime)
	{
		m_pCurClip->fAnimTime -= fLimitTime;
		++m_pCurClip->iFrameX;

		if (m_pCurClip->iFrameX >= m_pCurClip->iLengthX)
		{
			m_pCurClip->iFrameX = 0;
			++m_pCurClip->iFrameY;

			if (m_pCurClip->iFrameY >= m_pCurClip->iStartY + m_pCurClip->iLengthY)
			{
				m_pCurClip->iFrameY = m_pCurClip->iStartY;
				m_bClipEnd = true;

				switch (m_pCurClip->eOption)
				{
				case A2DO_ONCE:
					m_pCurClip->iFrameX = m_pCurClip->iFrameMaxX;
					break;
				case A2DO_ONCE_RETURN:
					ChangeAnimation(m_strDefaultAnim);
					break;
				case A2DO_ONCE_DESTROY:
					m_pGameObject->Die();
					break;
				case A2DO_COUNT_RETURN:
					break;
				case A2DO_COUNT_DESTROY:
					break;
				case A2DO_TIME_RETURN:
					break;
				case A2DO_TIME_DESTROY:
					if (m_pCurClip->fOptionTime >= m_pCurClip->fOptionTimeLimit)
					{
						m_pGameObject->Die();
					}
					break;
				case A2DO_NEXT:
					if (m_bClipEnd && !m_vecNextClip.empty())
					{
						ChangeAnimation(m_vecNextClip[m_iCount]);
						++m_iCount;
					}
					break;

				}
			}
		}
	}


	return 0;
}

int CAnimation2D::LateUpdate(float fTime)
{
	if (!m_pCurClip)
		return 0;

	// 오브젝트의 Render를 가지고 오고
	CRenderer2D* pRender = m_pGameObject->FindComponentFromTypeID<CRenderer2D>();

	// 재료인대.. 랜더의 재료를 받아온다.
	CMaterial* pMaterial = pRender->GetMaterial();

	pMaterial->SetDiffuseTex(m_pCurClip->pTexture);

	SAFE_RELEASE(pMaterial);

	// 상수버퍼 갱신
	m_tCBuffer.iType = m_pCurClip->eType;
	m_tCBuffer.iFrameX = m_pCurClip->iFrameX;
	m_tCBuffer.iFrameY = m_pCurClip->iFrameY;
	m_tCBuffer.iLengthX = m_pCurClip->iFrameMaxX;
	m_tCBuffer.iLengthY = m_pCurClip->iFrameMaxY;

	pRender->UpdateCBuffer("Animation2D", 10, sizeof(ANIMATION2DBUFFER), SCT_VERTEX | SCT_PIXEL, &m_tCBuffer);

	SAFE_RELEASE(pRender);

	return 0;
}

void CAnimation2D::Collision(float fTime)
{
}

void CAnimation2D::Render(float fTime)
{
}

CAnimation2D * CAnimation2D::Clone()
{
	return new CAnimation2D(*this);
}
