#include "Layer.h"

GP_USING

CLayer::CLayer() : 
	m_iZOrder(0)
{
	SetTag("Layer");
	SetTypeName("CLayer");
	SetTypeID<CLayer>();
}


CLayer::~CLayer()
{
}

void CLayer::SetZOrder(int iZOrder)
{
	m_iZOrder = iZOrder;
}

int CLayer::GetZOrder() const
{
	return 0;
}

bool CLayer::Init()
{
	return true;
}

void CLayer::Input(float fTime)
{
}

int CLayer::Update(float fTime)
{
	return 0;
}

int CLayer::LateUpdate(float fTime)
{
	return 0;
}

void CLayer::Collision(float fTime)
{
}

void CLayer::Render(float fTime)
{
}
