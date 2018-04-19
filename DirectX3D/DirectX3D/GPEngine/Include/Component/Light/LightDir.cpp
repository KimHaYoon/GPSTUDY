#include "LightDir.h"
#include "../Transform/Transform.h"

GP_USING

CLightDir::CLightDir()
{
	SetTag("LightDir");
	SetTypeName("CLightDir");
	SetTypeID<CLightDir>();
	m_tInfo.iType = LT_DIR;
}

CLightDir::CLightDir(const CLightDir & light) : 
	CLight(light)
{
}


CLightDir::~CLightDir()
{
}

bool CLightDir::Init()
{
	return true;
}

void CLightDir::Input(float fTime)
{
}

int CLightDir::Update(float fTime)
{
	return 0;
}

int CLightDir::LateUpdate(float fTime)
{
	/*������ ������ ���´�.
	������ ���ӿ�����Ʈ�� �����Ǿ��ִ�.
	���ӿ�����Ʈ�� ������ �ִ� transform���� z�� ������ ���ͼ� ������ �⺻���� �������� ����Ѵ�*/

	m_tInfo.vDir = m_pTransform->GetWorldAxis(AXIS_Z);

	return 0;
}

void CLightDir::Collision(float fTime)
{
}

void CLightDir::Render(float fTime)
{
}

CLightDir * CLightDir::Clone()
{
	return new CLightDir(*this);
}