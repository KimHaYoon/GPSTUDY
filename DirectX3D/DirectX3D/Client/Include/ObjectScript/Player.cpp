#include "Player.h"

#include "Core/Input/Input.h"
#include "Component/Transform/Transform.h"
#include "Component/Renderer/Renderer.h"
#include "Component/Material/Material.h"
#include "Component/Collider/ColliderAABB.h"
#include "GameObject/GameObject.h"

CPlayer::CPlayer()
{
	SetTag("Player");
	SetTypeName("CPlayer");
	SetTypeID<CPlayer>();
}

CPlayer::CPlayer(const CPlayer & player) : 
	CScript(player)
{
	m_fMoveSpeed = player.m_fMoveSpeed;
	m_fRotSpeed = player.m_fRotSpeed;
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	m_pTransform->SetOriginAxis(AXIS_Y);
	m_fMoveSpeed = 2.f;
	m_fRotSpeed = PI;

	CRenderer*	pRenderer = m_pGameObject->AddComponent<CRenderer>("PlayerRenderer");
	pRenderer->SetMesh("Pyramid");
	pRenderer->SetShader(STANDARD_COLOR_NORMAL_SHADER);
	pRenderer->SetInputLayout("VectexColorNormal");

	CMaterial* pMaterial = pRenderer->CreateMaterial();
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	CColliderAABB* pAABB = m_pGameObject->AddComponent<CColliderAABB>("PlayerBody");
	pAABB->SetAABB(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));
	SAFE_RELEASE(pAABB);



	return true;
}

void CPlayer::Input(float fTime)
{
}

int CPlayer::Update(float fTime)
{
	return 0;
}

int CPlayer::LateUpdate(float fTime)
{
	return 0;
}

CPlayer * CPlayer::Clone()
{
	return nullptr;
}
