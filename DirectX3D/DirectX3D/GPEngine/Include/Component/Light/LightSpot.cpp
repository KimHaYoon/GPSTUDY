#include "LightSpot.h"
#include "../Transform/Transform.h"

GP_USING

CLightSpot::CLightSpot()
{
	SetTag("LightSpot");
	SetTypeName("CLightSpot");
	SetTypeID<CLightSpot>();
	m_tInfo.iType = LT_SPOT;
	m_tInfo.fInAngle = PI / 4;
	m_tInfo.fOutAngle = PI / 2;
	m_tInfo.fIntensityPercent = 0.5f;
}


CLightSpot::CLightSpot(const CLightSpot & light) : 
	CLight(light)
{
}

CLightSpot::~CLightSpot()
{
}

bool CLightSpot::Init()
{
	return true;
}

void CLightSpot::Input(float fTime)
{
}

int CLightSpot::Update(float fTime)
{
	return 0;
}

int CLightSpot::LateUpdate(float fTime)
{
	m_tInfo.vPos = m_pTransform->GetWorldPos();
	m_tInfo.vDir = m_pTransform->GetWorldAxis(AXIS_Z);

	return 0;
}

void CLightSpot::Collision(float fTime)
{
}

void CLightSpot::Render(float fTime)
{
}

CLightSpot * CLightSpot::Clone()
{
	return nullptr;
}
