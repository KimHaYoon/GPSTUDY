#pragma once

#include "EngineFlag.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"

GP_BEGIN

typedef struct GP_DLL _tagResolution
{
	UINT	iWidth;
	UINT	iHeight;

	_tagResolution() :
		iWidth(0),
		iHeight(0)
	{
	}

	_tagResolution(UINT w, UINT h) :
		iWidth(w),
		iHeight(h)
	{
	}
}RESOLUTION, *PRESOLUTION;

// Color Vertex
typedef struct GP_DLL _tagVertexColor
{
	Vector3	vPos;
	Vector4	vColor;

	_tagVertexColor()
	{
	}

	_tagVertexColor(const _tagVertexColor& vtx)
	{
		vPos = vtx.vPos;
		vColor = vtx.vColor;
	}

	_tagVertexColor(float x, float y, float z, float r, float g, float b,
		float a) :
		vPos(x, y, z),
		vColor(r, g, b, a)
	{
	}

	_tagVertexColor(const Vector3& _vPos, const Vector4& _vColor) :
		vPos(_vPos),
		vColor(_vColor)
	{
	}
}VERTEXCOLOR, *PVERTEXCOLOR;

GP_END