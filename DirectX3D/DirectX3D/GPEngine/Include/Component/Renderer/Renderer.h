#pragma once
#include "../Component.h"

GP_BEGIN

class GP_DLL CRenderer :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CRenderer();
	CRenderer(const CRenderer& renderer);
	~CRenderer();

private:
	class CMesh*		m_pMesh;
	class CShader*		m_pShader;
	ID3D11InputLayout*	m_pInputLayout;
	class CMaterial*	m_pMaterial;

public:
	void SetMesh(const string& strKey);
	void SetMesh(class CMesh* pMesh);
	void SetShader(const string& strKey);
	void SetShader(class CShader* pShader);
	void SetInputLayout(const string& strKey);
	void SetInputLayout(ID3D11InputLayout* pLayout);
	class CMaterial*	CreateMaterial();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CRenderer* Clone();

private:
	void UpdateTransform();
};

GP_END