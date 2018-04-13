#pragma once

#include "EngineMacro.h"

// รเ
enum AXIS
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_END
};

// Shader Type
enum SHADER_TYPE
{
	ST_VERTEX,
	ST_PIXEL,
	ST_MAX
};

// Shader Constant Type
enum SHADER_CONSTANT_TYPE
{
	SCT_VERTEX = 0x1,
	SCT_PIXEL = 0x2
};

// Camera Type
enum CAMERA_TYPE
{
	CT_PERSPECTIVE,
	CT_ORTHO
};

// RenderState Type
enum RENDERSTATE_TYPE
{
	RST_BLEND,
	RST_RASTERIZER,
	RST_DEPTH,
	RST_END
};

// Animation2D Type
enum ANIMATION2D_TYPE
{
	A2D_NONE = -1,
	A2D_ATLAS,
	A2D_FRAME,
	A2D_END
};

// Animation2D Option
enum ANIMATION2D_OPTION
{
	A2DO_LOOP,
	A2DO_ONCE_RETURN,
	A2DO_ONCE_DESTROY,
	A2DO_COUNT_RETURN,
	A2DO_COUNT_DESTROY,
	A2DO_TIME_RETURN,
	A2DO_TIME_DESTROY,
	A2DO_END
};
