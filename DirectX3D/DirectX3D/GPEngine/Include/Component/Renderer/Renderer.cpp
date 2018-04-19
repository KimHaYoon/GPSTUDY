#include "Renderer.h"

GP_USING

CRenderer::CRenderer()
{
}

CRenderer::CRenderer(const CRenderer & renderer) : 
	CComponent(renderer)
{
}


CRenderer::~CRenderer()
{
}

void CRenderer::SetMesh(const string & strKey)
{
}

void CRenderer::SetMesh(CMesh * pMesh)
{
}

void CRenderer::SetShader(const string & strKey)
{
}

void CRenderer::SetShader(CShader * pShader)
{
}

void CRenderer::SetInputLayout(const string & strKey)
{
}

void CRenderer::SetInputLayout(ID3D11InputLayout * pLayout)
{
}

CMaterial * CRenderer::CreateMaterial()
{
	return nullptr;
}

bool CRenderer::Init()
{
	return false;
}

void CRenderer::Input(float fTime)
{
}

int CRenderer::Update(float fTime)
{
	return 0;
}

int CRenderer::LateUpdate(float fTime)
{
	return 0;
}

void CRenderer::Collision(float fTime)
{
}

void CRenderer::Render(float fTime)
{
}

CRenderer * CRenderer::Clone()
{
	return nullptr;
}

void CRenderer::UpdateTransform()
{
}
