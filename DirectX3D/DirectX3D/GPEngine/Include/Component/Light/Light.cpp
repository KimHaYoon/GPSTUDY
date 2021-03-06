#include "Light.h"
#include "../../Rendering/Shader/ShaderManager.h"

GP_USING

CLight::CLight()
{
	m_eType = CT_LIGHT;
	m_tInfo.vDif = Vector4::White;
	m_tInfo.vAmb = Vector4(0.2f, 0.2f, 0.2f, 1.f);
	m_tInfo.vSpc = Vector4::White;
}

CLight::CLight(const CLight & light) : 
	CComponent(light)
{
	m_tInfo = light.m_tInfo;
}

CLight::~CLight()
{
}

void CLight::Input(float fTime)
{
}

int CLight::Update(float fTime)
{
	return 0;
}

int CLight::LateUpdate(float fTime)
{
	return 0;
}

void CLight::Collision(float fTime)
{
}

void CLight::Render(float fTime)
{
}

void CLight::SetLightColor(const Vector4 & vDif, const Vector4 & vAmb, const Vector4 & vSpc)
{
	m_tInfo.vDif = vDif;
	m_tInfo.vAmb = vAmb;
	m_tInfo.vSpc = vSpc;
}

void CLight::SetShader()
{
	GET_SINGLE(CShaderManager)->UpdateCBuffer(LIGHT_BUFFER, &m_tInfo, SCT_PIXEL);
}