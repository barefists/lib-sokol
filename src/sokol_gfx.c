#if defined(_WIN32)
#define SOKOL_D3D11
#elif defined(__APPLE__)
#define SOKOL_METAL
#else
#define SOKOL_GLCORE33
#endif
#define SOKOL_IMPL
#define SOKOL_DLL
#include "sokol_gfx.h"
