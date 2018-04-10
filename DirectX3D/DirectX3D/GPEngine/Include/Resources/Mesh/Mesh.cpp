#include "Mesh.h"
#include "../../Device/Device.h"

GP_USING

CMesh::CMesh() :
	m_pVB(NULL),
	m_pIB(NULL)
{
	SetTag("Mesh");
	SetTypeName("CMesh");
	SetTypeID<CMesh>();
}

CMesh::~CMesh()
{
	if (m_pVB)
		SAFE_RELEASE(m_pVB->pBuffer);

	if (m_pIB)
		SAFE_RELEASE(m_pIB->pBuffer);

	SAFE_DELETE(m_pVB);
	SAFE_DELETE(m_pIB);
}

Vector3 CMesh::GetLength() const
{
	return m_vLength;
}

string CMesh::GetKey() const
{
	return m_strKey;
}

void CMesh::SetKey(const string & strKey)
{
	m_strKey = strKey;
}

bool CMesh::CreateMesh(UINT iVtxCount, UINT iVtxSize, D3D11_USAGE eVtxUsage,
	D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void * pVtxData, UINT iIdxCount,
	UINT iIdxSize, D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt, void * pIdxData)
{
	if (!CreateVertexBuffer(iVtxCount, iVtxSize, eVtxUsage, ePrimitive,
		pVtxData))
		return false;

	if (!CreateIndexBuffer(iIdxCount, iIdxSize, eIdxUsage, eFmt,
		pIdxData))
		return false;

	m_vLength = m_vMax - m_vMin;

	return true;
}

bool CMesh::CreateVertexBuffer(UINT iVtxCount, UINT iVtxSize,
	D3D11_USAGE eVtxUsage, D3D11_PRIMITIVE_TOPOLOGY ePrimitive,
	void * pVtxData)
{
	if (m_pVB)
		SAFE_RELEASE(m_pVB->pBuffer);

	SAFE_DELETE(m_pVB);

	m_pVB = new VERTEXBUFFER;

	m_pVB->iVtxCount = iVtxCount;
	m_pVB->iVtxSize = iVtxSize;
	m_pVB->eUsage = eVtxUsage;
	m_pVB->ePrimitive = ePrimitive;

	D3D11_BUFFER_DESC	tDesc = {};
	tDesc.ByteWidth = iVtxCount * iVtxSize;
	tDesc.Usage = eVtxUsage;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	if (eVtxUsage == D3D11_USAGE_DYNAMIC)				// GPU가 자원을 읽을 수 있으며 CPU는 매핑API(ID3D11DeviceContext::Map)를 통해서 자료를 기록할 수 있음
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU가 자료를 기록해서 버퍼를 갱신
							
	D3D11_SUBRESOURCE_DATA	tData = {};
	tData.pSysMem = pVtxData;			// 정점 버퍼를 초기화할 자료를 담은 시스템 메모리 배열을 가리키는 포인터

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &m_pVB->pBuffer)))
		return false;

	char*	pVtx = (char*)pVtxData;

	// 정점의 Min, Max 값을 구한다.
	for (int i = 0; i < iVtxCount; ++i)
	{
		Vector3	vPos;
		memcpy(&vPos, pVtx + iVtxSize * i, sizeof(Vector3));

		if (vPos.x < m_vMin.x)
			m_vMin.x = vPos.x;

		if (vPos.y < m_vMin.y)
			m_vMin.y = vPos.y;

		if (vPos.z < m_vMin.z)
			m_vMin.z = vPos.z;

		if (vPos.x > m_vMax.x)
			m_vMax.x = vPos.x;

		if (vPos.y > m_vMax.y)
			m_vMax.y = vPos.y;

		if (vPos.z > m_vMax.z)
			m_vMax.z = vPos.z;

	}

	return true;
}

bool CMesh::CreateIndexBuffer(UINT iIdxCount, UINT iIdxSize,
	D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt, void * pIdxData)
{
	if (!pIdxData)
		return true;

	if (m_pIB)
		SAFE_RELEASE(m_pIB->pBuffer);

	SAFE_DELETE(m_pIB);

	m_pIB = new INDEXBUFFER;

	m_pIB->iIdxCount = iIdxCount;
	m_pIB->iIdxSize = iIdxSize;
	m_pIB->eUsage = eIdxUsage;
	m_pIB->eFmt = eFmt;

	D3D11_BUFFER_DESC	tDesc = {};
	tDesc.ByteWidth = iIdxCount * iIdxSize;
	tDesc.Usage = eIdxUsage;
	tDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	if (eIdxUsage == D3D11_USAGE_DYNAMIC)
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA	tData = {};
	tData.pSysMem = pIdxData;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &m_pIB->pBuffer)))
		return false;

	return true;
}

void CMesh::Render(float fTime)
{
	// 도형 위상구조를 설정한다.
	CONTEXT->IASetPrimitiveTopology(m_pVB->ePrimitive);
	// 버텍스버퍼를 지정한다.
	UINT	iStride = m_pVB->iVtxSize;
	UINT	iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, &m_pVB->pBuffer, &iStride, &iOffset);

	if (m_pIB)
	{
		// 인덱스버퍼를 지정한다.
		CONTEXT->IASetIndexBuffer(m_pIB->pBuffer, m_pIB->eFmt, 0);
		CONTEXT->DrawIndexed(m_pIB->iIdxCount, 0, 0);
	}

	else
	{
		CONTEXT->Draw(m_pVB->iVtxCount, 0);
	}
}
