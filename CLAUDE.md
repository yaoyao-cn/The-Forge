# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**The Forge** is a cross-platform rendering framework supporting:
- PC: Windows (DirectX 11/12, Vulkan), Linux (Vulkan)
- Mobile: Android (Vulkan, OpenGL ES), iOS/iPadOS (Metal)
- macOS (Metal)
- Consoles: Xbox, PlayStation, Switch (for accredited developers)
- VR: Quest 2 (Vulkan)

Key technologies:
- Written in C/C++ (moving towards C99)
- Custom shader language: FSL (Forge Shading Language) - a superset of HLSL
- Multi-threaded rendering and resource loading
- Cross-platform abstraction for graphics APIs

## Directory Structure

### Common_3/
Core framework code shared across all platforms:
- **Application/** - IApp interface and app lifecycle management
- **OS/** - Platform abstraction layer (Interfaces/ contains IApp.h, IOperatingSystem.h, ILog.h, etc.)
- **Renderer/** - Graphics API abstraction
  - `IRenderer.h` - Main renderer interface
  - `Direct3D11/`, `Direct3D12/`, `Vulkan/`, `Metal/`, `OpenGLES/`, `Quest/` - Backend implementations
  - `RendererConfig.h` - Configuration for supported graphics APIs
  - `ResourceLoader.cpp` - Async resource loading
- **Resources/** - Asset loading (textures, meshes, etc.)
- **ThirdParty/** - Third-party libraries (EASTL, ozz-animation, flecs, imgui, etc.)
- **Tools/** - Build tools
  - `ForgeShadingLanguage/` - FSL shader compiler (Python-based)
  - `AssetPipeline/` - Asset processing tools
  - `SpirvTools/` - SPIR-V utilities
- **Utilities/** - Common utilities (math, memory management, etc.)

### Examples_3/
Example applications and unit tests:
- **Unit_Tests/** - Numbered test applications (01_Transformations, 03_MultiThread, etc.)
  - `PC Visual Studio 2017/` - Windows project files (.sln)
  - `Android_VisualStudio2017/` - Android AGDE projects
  - `Quest_VisualStudio2017/` - Quest VR projects
  - `src/` - Source code for all unit tests
- **Visibility_Buffer/** - Advanced visibility buffer rendering example
- **Aura/** - Light propagation volumes example
- **Ephemeris/** - Sky/atmosphere rendering example

### Art/
Downloaded art assets (textures, models, etc.) - obtained by running PRE_BUILD script

### Tools/
Helper executables (wget, 7z for downloading/extracting assets)

### Scripts/
Build automation and utility scripts

## Build System

### Setup
1. **First time setup**: Run `PRE_BUILD.bat` (Windows) or `PRE_BUILD.command` (macOS/Linux)
   - Downloads and extracts art assets (~2GB)
   - Required before building any projects

2. **Platform-specific IDEs**:
   - **Windows**: Visual Studio 2017, projects in `Examples_3/*/PC Visual Studio 2017/`
   - **Android**: Visual Studio 2017 with AGDE, projects in `Examples_3/*/Android_VisualStudio2017/`
   - **macOS/iOS**: Xcode 12.1+, projects in `Examples_3/*/macOS Xcode/` or `iOS Xcode/`
   - **Linux**: CodeLite 12.0.6, workspaces in `Examples_3/*/Linux CodeLite/`

### Building Projects
- **Windows**: Open `.sln` file in Visual Studio 2017, select configuration (Debug/Release), build
- **Main solution for all unit tests**: `Examples_3/Unit_Tests/PC Visual Studio 2017/Unit_Tests.sln`
- **Individual examples**: Each example has its own solution file

### Renderer Selection
- Configured in `Common_3/Renderer/RendererConfig.h`
- Define macros like `DIRECT3D12`, `VULKAN`, `METAL` to enable backends
- Most platforms support multiple backends (e.g., Windows can use D3D12 or Vulkan)

### Shader Building
- Shaders are written in FSL (.fsl files)
- Build process automatically compiles FSL to target platform shaders
- Python tool: `Common_3/Tools/ForgeShadingLanguage/fsl.py`
- Manual shader compilation: `python PyBuildShaders.py` (in project root)

## Core Architecture

### Application Lifecycle (IApp interface)
All applications implement `IApp` interface defined in `Common_3/OS/Interfaces/IApp.h`:

```cpp
class IApp {
    virtual bool Init() = 0;      // Initialize app (load geometry, etc.)
    virtual void Exit() = 0;      // Cleanup
    virtual bool Load() = 0;      // Load GPU resources (shaders, textures, buffers)
    virtual void Unload() = 0;    // Unload GPU resources
    virtual void Update(float deltaTime) = 0;  // CPU update (input, logic)
    virtual void Draw() = 0;      // GPU rendering (command buffer generation)
    virtual const char* GetName() = 0;
};
```

**Lifecycle flow**:
1. `Init()` - Once at startup, device-independent initialization
2. `Load()` - Load GPU resources (can be called multiple times if device changes)
3. `Update()` → `Draw()` - Main loop
4. `Unload()` - Before device change or exit
5. `Exit()` - Once at shutdown

### Renderer Abstraction
- Main interface: `Common_3/Renderer/IRenderer.h` (very large file ~103KB)
- Key structures:
  - `Renderer` - Main renderer object
  - `Queue` - Command queue
  - `CmdPool` - Command buffer pool
  - `Cmd` - Command buffer
  - `Pipeline` - Graphics/compute pipeline state
  - `DescriptorSet` - Descriptor/resource bindings
  - `Buffer`, `Texture` - GPU resources

- Typical rendering flow:
  ```
  initRenderer() → addQueue() → addCmdPool() → addCmd()
  → load resources → acquire frame → begin cmd → bind pipeline/descriptors → draw → end cmd → submit
  ```

### Platform Abstraction
- OS abstraction: `Common_3/OS/Interfaces/IOperatingSystem.h`
- File system: `Common_3/OS/Interfaces/IFileSystem.h`
- Cross-platform window management in `{Platform}Window.cpp` files
- Input: gainput library (extended version in `ThirdParty/OpenSource/gainput/`)

## Forge Shading Language (FSL)

FSL is a superset of HLSL that compiles to:
- HLSL (DirectX)
- SPIR-V (Vulkan)
- Metal Shading Language (Metal)
- GLSL (OpenGL ES)

### FSL Basics
Key macros/keywords:
- `STRUCT(name)` - Define structure
- `DATA(type, name, semantic)` - Struct member
- `CBUFFER(name, reg, space)` - Constant buffer
- `RES(type, name, reg, space)` - Resource declaration
- `VS_MAIN`, `PS_MAIN`, `CS_MAIN` - Shader entry points
- `INIT_MAIN` - Initialize shader (required at start of entry point)
- `RETURN(value)` - Return from shader
- `Get(resource)` - Access resource from descriptor

### Example FSL Shader
```hlsl
#include "resources.h.fsl"

STRUCT(VSInput)
{
    DATA(float3, Position, POSITION);
    DATA(float3, Normal, NORMAL);
};

STRUCT(VSOutput)
{
    DATA(float4, Position, SV_Position);
    DATA(float4, Color, COLOR);
};

VSOutput VS_MAIN(VSInput In, SV_InstanceID(uint) InstanceID)
{
    INIT_MAIN;
    VSOutput Out;

    float4x4 mvp = Get(mvpMatrix);
    Out.Position = mul(mvp, float4(In.Position, 1.0));
    Out.Color = float4(In.Normal * 0.5 + 0.5, 1.0);

    RETURN(Out);
}
```

### FSL File Naming
- `.vert.fsl` - Vertex shader
- `.frag.fsl` - Fragment/pixel shader
- `.comp.fsl` - Compute shader
- `.tese.fsl` - Tessellation evaluation shader
- `.h.fsl` - Header/include file

### Building Shaders
FSL shaders are compiled automatically during project build, or manually via:
```bash
python PyBuildShaders.py
```

For more details: https://github.com/ConfettiFX/The-Forge/wiki/The-Forge-Shading-Language-(FSL)

## Unit Tests

Unit tests demonstrate various rendering techniques. Start with these:

### 01_Transformations
- Simplest example - renders a solar system
- Good starting point for understanding the framework
- Location: `Examples_3/Unit_Tests/src/01_Transformations/`

### 03_MultiThread
- Multi-threaded command buffer generation
- Demonstrates parallel rendering

### 04_ExecuteIndirect
- GPU-driven rendering with ExecuteIndirect
- Shows CPU vs GPU updates for indirect arguments

### 06_MaterialPlayground
- Advanced materials (hair, metal, wood)
- TressFX hair rendering

### 09_LightShadowPlayground
- Various shadow techniques (ESM, ASM, SDF shadows)
- Visibility buffer architecture

### 16_Raytracing
- Cross-platform path tracer
- Supports DXR, Vulkan RTX, Metal ray tracing

### Unit Test Structure
Each unit test typically contains:
- `{TestName}.cpp` - Main application code (implements IApp)
- `Shaders/FSL/` - FSL shader source files
- `GPUCfg/` - GPU-specific configuration (optional)

## Common Development Workflows

### Adding a New Unit Test
1. Copy an existing unit test (e.g., 01_Transformations) as template
2. Add project to solution file for target platform
3. Implement IApp interface in main .cpp file
4. Write FSL shaders in `Shaders/FSL/` directory
5. Configure resource loading in `Load()` method

### Modifying Shaders
1. Edit .fsl files in `Examples_3/{Example}/src/Shaders/FSL/`
2. Build project - FSL compiler runs automatically
3. If manual compilation needed: `python PyBuildShaders.py`
4. Compiled shaders output to `CompiledShaders/` in binary directory

### Working with Renderer
1. Include `Common_3/Renderer/IRenderer.h`
2. Initialize renderer in app:
   ```cpp
   RendererDesc settings = {};
   initRenderer(GetName(), &settings, &pRenderer);
   ```
3. Create command structures:
   ```cpp
   addQueue(pRenderer, &queueDesc, &pGraphicsQueue);
   addCmdPool(pRenderer, &cmdPoolDesc, &pCmdPool);
   addCmd(pRenderer, &cmdDesc, &pCmd);
   ```
4. Main render loop calls `Draw()` which records command buffers

### Modifying Core Renderer
- Backend implementations in `Common_3/Renderer/{API}/`
  - `Vulkan/Vulkan.cpp` - Vulkan implementation
  - `Direct3D12/Direct3D12.cpp` - D3D12 implementation
  - `Metal/MetalRenderer.mm` - Metal implementation
- Interface defined in `IRenderer.h`
- Changes to interface require updating ALL backends

## Key Files Reference

### Critical Headers
- `Common_3/OS/Interfaces/IApp.h` - Application interface
- `Common_3/Renderer/IRenderer.h` - Renderer interface (103KB, comprehensive)
- `Common_3/Renderer/IResourceLoader.h` - Resource loading
- `Common_3/OS/Interfaces/IFileSystem.h` - File I/O
- `Common_3/OS/Interfaces/ILog.h` - Logging
- `Common_3/Utilities/Math/MathTypes.h` - Math types (vectormath)
- `Common_3/ThirdParty/OpenSource/EASTL/` - Container library

### Renderer Implementations
- `Common_3/Renderer/Vulkan/Vulkan.cpp` (~30K lines)
- `Common_3/Renderer/Direct3D12/Direct3D12.cpp` (~25K lines)
- `Common_3/Renderer/Metal/MetalRenderer.mm` (~20K lines)
- `Common_3/Renderer/Direct3D11/Direct3D11.cpp` (~15K lines)

### Build Scripts
- `PRE_BUILD.bat` / `PRE_BUILD.command` - Download art assets
- `PyBuildShaders.py` - Build all FSL shaders
- `Common_3/Tools/ForgeShadingLanguage/fsl.py` - FSL compiler

## Important Notes for AI Assistants

### Search Strategy
- When looking for renderer features, search `Common_3/Renderer/IRenderer.h` first
- For platform-specific code, check backend folders: `Vulkan/`, `Direct3D12/`, `Metal/`
- Unit test examples are in `Examples_3/Unit_Tests/src/{TestNumber}_{TestName}/`

### Common Pitfalls
- **Don't** modify generated shader files in `CompiledShaders/` - edit .fsl source instead
- **Always** run PRE_BUILD script before first build (downloads required assets)
- **Remember** to update ALL renderer backends when modifying IRenderer interface
- **Be aware** that the project is transitioning from C++ to C99 - prefer C-style code
- **EASTL** is being phased out - avoid adding new EASTL dependencies

### Preprocessor Macros
Common platform/API detection macros:
- `_WINDOWS`, `__APPLE__`, `__linux__`, `__ANDROID__`
- `DIRECT3D12`, `VULKAN`, `METAL`, `GLES`
- `XBOX`, `ORBIS` (PS4), `PROSPERO` (PS5)
- `QUEST_VR`

### Code Style
- Follow Orthodox C++ guidelines (moving to C99)
- Limit to C++11 features
- Prefer stack allocation over heap
- Use The Forge's memory allocators (tf_malloc, tf_free)
- Minimal use of STL/EASTL containers

### Testing Locations
When modifying renderer:
- Test on Windows with both D3D12 and Vulkan backends
- Unit test 01_Transformations is simplest validation
- Unit test 03_MultiThread tests multi-threading
- Unit test 16_Raytracing tests ray tracing features

### Resource Management
- Resources loaded asynchronously via `ResourceLoader`
- Synchronization required between Load/Unload and rendering
- Each resource type has `add{Type}` and `remove{Type}` functions
- Example: `addTexture()`, `removeTexture()`, `addBuffer()`, `removeBuffer()`

## Third-Party Libraries

Key dependencies in `Common_3/ThirdParty/OpenSource/`:
- **ozz-animation** - Animation system
- **flecs** - Entity Component System (ECS)
- **gainput** - Input handling
- **imgui** - UI rendering
- **EASTL** - EA Standard Template Library (being phased out)
- **meshoptimizer** - Mesh optimization
- **VulkanMemoryAllocator** - Vulkan memory management
- **D3D12MemoryAllocator** - D3D12 memory management
- **SPIRV-Cross** - SPIR-V reflection/conversion
- **basis_universal** - Texture compression

## Additional Resources

- Wiki: https://github.com/ConfettiFX/The-Forge/wiki
- FSL Documentation: https://github.com/ConfettiFX/The-Forge/wiki/The-Forge-Shading-Language-(FSL)
- Descriptor Management: https://github.com/ConfettiFX/The-Forge/wiki/Descriptor-Management
- Discord: https://discord.gg/hJS54bz
