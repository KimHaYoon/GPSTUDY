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

// Constant Buffer
typedef struct GP_DLL _tagConstantBuffer
{
	int				iRegister;
	int				iSize;
	ID3D11Buffer*	pBuffer;
}CONSTANTBUFFER, *PCONSTANTBUFFER;

typedef struct GP_DLL _tagTransformCBuffer
{
	Matrix	matWorld;
	Matrix	matView;
	Matrix	matProj;
	Matrix	matWV;
	Matrix	matWVP;
	Matrix	matWP;
	Vector3	vPivot;
	float	fEmpty;
	Vector3	vLength;
	float	fEmpty1;
}TRANSFORMCBUFFER, *PTRANSFORMCBUFFER;

typedef struct GP_DLL _tagAnimationClip2D
{
	ANIMATION2D_TYPE	eType;
	ANIMATION2D_OPTION	eOption;
	class CTexture*		pTexture;
	int		iFrameX;
	int		iFrameY;
	int		iFrameMaxX;
	int		iFrameMaxY;
	int		iLengthX;
	int		iLengthY;
	int		iStartY;
	float	fAnimTime;
	float	fAnimLimitTime;
	int		iCount;
	int		iCountLimit;
	float	fOptionTime;
	float	fOptionTimeLimit;

	_tagAnimationClip2D() :
		eType(A2D_ATLAS),
		eOption(A2DO_LOOP),
		pTexture(NULL),
		iFrameX(0),
		iFrameY(0),
		iFrameMaxX(1),
		iFrameMaxY(1),
		fAnimTime(0.f),
		fAnimLimitTime(1.f),
		iCount(0),
		iCountLimit(0),
		fOptionTime(0.f),
		fOptionTimeLimit(0.f),
		iLengthX(0),
		iLengthY(0),
		iStartY(0)
	{
	}
}ANIMATIONCLIP2D, *PANIMATIONCLIP2D;

typedef struct GP_DLL _tagAnimation2DCBuffer
{
	int		iType;
	int		iFrameX;
	int		iFrameY;
	int		iLengthX;
	int		iLengthY;
	Vector3	vEmpty;
}ANIMATION2DBUFFER, *PANIMATION2DBUFFER;

// Button ConstantBuffer
typedef struct GP_DLL _tagButtonCBuffer
{
	Vector4	vColor;
	float	fLight;
	Vector3	vEmpty;
}BUTTONCBUFFER, *PBUTTONCBUFFER;

GP_END