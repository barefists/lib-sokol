#define SOKOL_DLL

/* Select a default graphics backend for sokol_gfx */
#if defined(_WIN32)
#define SOKOL_D3D11
#elif defined(__APPLE__)
#define SOKOL_METAL
#else
#define SOKOL_GLCORE33
#endif

/* Implementations */
#define SOKOL_IMPL
#include "sokol_gfx.h"

/* sokol_app should not provide an entry point and shouldn't bind a 3D API */
#define SOKOL_NO_ENTRY
#define SOKOL_NOAPI
#include "sokol_app.h"

#include "sokol_audio.h"
#include "sokol_time.h"
#include "sokol_args.h"
