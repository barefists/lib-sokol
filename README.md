# lib-sokol dynamic libraries

This project builds a single shared library for the [sokol](https://github.com/floooh/sokol) headers.

Target:
- sokol (combines sokol_gfx, sokol_app, sokol_audio, sokol_time, sokol_args)

Outputs a platform-specific dynamic library: `.dll` on Windows, `.dylib` on macOS, `.so` on Linux.

## Configure

Headers are vendored under `third_party/sokol` and used by default. You can override with `-DSOKOL_DIR=...` if desired.

All platforms (choose your preferred generator):

```
cmake -S . -B build
cmake --build build --config Release
```

Artifacts will be in `build/Release/` (MSVC multi-config) or `build/` for single-config generators.

## Notes

- This follows guidance from https://github.com/floooh/sokol/issues/160 and https://github.com/afterschoolstudio/sokol-dll-builder, defining `SOKOL_DLL` and compiling a single translation unit per header with `SOKOL_IMPL`.
- Backends are selected per platform in the stub sources:
  - Windows: D3D11
  - macOS: Metal
  - Linux/Other: GL 3.3 Core
- You can override backends by adding compile definitions on the CMake targets if needed.
- Internally, `sokol_app` is built with `SOKOL_NO_ENTRY` and `SOKOL_NOAPI` to avoid a default entry point and graphics dependency.

## Consuming the DLLs

- In your application, include the headers normally and define `SOKOL_DLL` (but do not define `SOKOL_IMPL`).
- Link against the single import library produced by this project (e.g., `sokol.lib` on Windows) and ship the corresponding `sokol` shared library (`sokol.dll/.dylib/.so`) alongside your executable.
