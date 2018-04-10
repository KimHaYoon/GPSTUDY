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