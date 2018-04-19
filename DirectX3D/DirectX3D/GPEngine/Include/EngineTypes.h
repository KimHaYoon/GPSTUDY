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


// Color Normal Vertex
typedef struct GP_DLL _tagVertexColorNormal
{
	Vector3	vPos;
	Vector3	vNormal;
	Vector4	vColor;

	_tagVertexColorNormal()
	{
	}

	_tagVertexColorNormal(const _tagVertexColorNormal& vtx)
	{
		vPos = vtx.vPos;
		vNormal = vtx.vNormal;
		vColor = vtx.vColor;
	}

	_tagVertexColorNormal(float x, float y, float z, float nx, float ny, float nz,
		float r, float g, float b, float a) :
		vPos(x, y, z),
		vNormal(nx, ny, nz),
		vColor(r, g, b, a)
	{
	}

	_tagVertexColorNormal(const Vector3& _vPos, const Vector3& _vNormal, const Vector4& _vColor) :
		vPos(_vPos),
		vNormal(_vNormal),
		vColor(_vColor)
	{
	}
}VERTEXCOLORNORMAL, *PVERTEXCOLORNORMAL;


// Tex Vertex
typedef struct GP_DLL _tagVertexTex
{
	Vector3	vPos;
	Vector2	vUV;

	_tagVertexTex()
	{
	}

	_tagVertexTex(const _tagVertexTex& vtx)
	{
		vPos = vtx.vPos;
		vUV = vtx.vUV;
	}

	_tagVertexTex(float x, float y, float z, float u, float v) :
		vPos(x, y, z),
		vUV(u, v)
	{
	}

	_tagVertexTex(const Vector3& _vPos, const Vector2& _vUV) :
		vPos(_vPos),
		vUV(_vUV)
	{
	}
}VERTEXTEX, *PVERTEXTEX;

// Constant Buffer
typedef struct GP_DLL _tagConstantBuffer
{
	int				iRegister;
	int				iSize;
	ID3D11Buffer*	pBuffer;
}CONSTANTBUFFER, *PCONSTANTBUFFER;

// Transform
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

// Animation2D Clip
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

// Material Á¤º¸
typedef struct GP_DLL _tagMaterial
{
	Vector4	vDif;
	Vector4	vAmb;
	Vector4	vSpc;
	Vector4	vEmv;
}MATERIAL, *PMATERIAL;

// Rect Info
typedef struct GP_DLL _tagRectInfo
{
	float	l;
	float	t;
	float	r;
	float	b;

	_tagRectInfo() :
		l(0.f),
		t(0.f),
		r(0.f),
		b(0.f)
	{
	}

	_tagRectInfo(float _l, float _t, float _r, float _b) :
		l(_l),
		t(_t),
		r(_r),
		b(_b)
	{
	}
}RECTINFO, *PRECTINFO;

typedef struct GP_DLL	_tagObb2DInfo
{
	Vector3	vCenter;
	Vector3	vAxisX;
	Vector3	vAxisY;
	float	fLengthX;
	float	fLengthY;
}OBB2DINFO, *POBB2DINFO;

// Sphere
typedef struct GP_DLL _tagSphere
{
	Vector3	vCenter;
	float	fRadius;

	_tagSphere() :
		vCenter(0.f, 0.f, 0.f),
		fRadius(0.f)
	{
	}

	_tagSphere(const _tagSphere& rhs)
	{
		*this = rhs;
	}

	_tagSphere(float x, float y, float z, float r) :
		vCenter(x, y, z),
		fRadius(r)
	{
	}

	_tagSphere(const Vector3& vPos, float r) :
		vCenter(vPos),
		fRadius(r)
	{

	}

	void operator =(const _tagSphere& tSphere)
	{
		vCenter = tSphere.vCenter;
		fRadius = tSphere.fRadius;
	}

	_tagSphere operator +(const _tagSphere& tSphere)
	{
		_tagSphere	tSp;

		tSp.vCenter = vCenter + tSphere.vCenter;
		tSp.fRadius = fRadius + tSphere.fRadius;

		return tSp;
	}

	_tagSphere operator +(const Vector3& tC)
	{
		_tagSphere	tSphere;

		tSphere.vCenter = vCenter + tC;
		tSphere.fRadius = fRadius;

		return tSphere;
	}

	_tagSphere operator +(const POINT& pt)
	{
		_tagSphere	tSphere;

		tSphere.vCenter.x = vCenter.x + pt.x;
		tSphere.vCenter.y = vCenter.x + pt.y;
		tSphere.fRadius = fRadius;

		return tSphere;
	}

	void operator +=(const _tagSphere& tSphere)
	{
		vCenter += tSphere.vCenter;
		fRadius += tSphere.fRadius;
	}

	void operator +=(const Vector3& tPos)
	{
		vCenter += tPos;
	}

	void operator +=(const POINT& pt)
	{
		vCenter.x += pt.x;
		vCenter.y += pt.y;
	}

	// -
	_tagSphere operator -(const _tagSphere& tSphere)
	{
		_tagSphere	tSp;

		tSp.vCenter = vCenter - tSphere.vCenter;
		tSp.fRadius = fRadius - tSphere.fRadius;

		return tSp;
	}

	_tagSphere operator -(const POINT& pt)
	{
		_tagSphere	tSphere;

		tSphere.vCenter.x = vCenter.x - pt.x;
		tSphere.vCenter.y = vCenter.y - pt.y;
		tSphere.vCenter.z = vCenter.z;
		tSphere.fRadius = fRadius;

		return tSphere;
	}

	_tagSphere operator -(const Vector3& vPos)
	{
		_tagSphere	tSphere;

		tSphere.vCenter = vCenter - vPos;
		tSphere.fRadius = fRadius;

		return tSphere;
	}

	void operator -=(const POINT& pt)
	{
		vCenter.x -= pt.x;
		vCenter.y -= pt.y;
	}
}SPHERE, *PSPHERE;


// Ellipse
typedef struct GP_DLL _tagEllipseInfo
{
	Vector3	vCenter;
	float fA, fB;
}ELLIPSE, *PELLIPSE;

// 3D
// AABB
typedef struct GP_DLL _tagAABB
{
	Vector3	vMin;
	Vector3	vMax;
}AABB, *PAABB;

// Light
typedef struct GP_DLL _tagLight
{
	Vector4	vDif;		
	Vector4	vAmb;
	Vector4	vSpc;
	int		iType;
	Vector3	vDir;
	Vector3	vPos;
	float	fRange;
	float	fInAngle;
	float	fOutAngle;
	Vector2	vEmpty;
}LIGHT, *PLIGHT;

GP_END