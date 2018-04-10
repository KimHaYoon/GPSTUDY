#pragma once

#include "resource.h"
#include "Engine.h"

#ifdef _DEBUG
#pragma comment(lib, "GPEngine_Debug")
#else
#pragma comment(lib, "GPEngine")
#endif // _DEBUG