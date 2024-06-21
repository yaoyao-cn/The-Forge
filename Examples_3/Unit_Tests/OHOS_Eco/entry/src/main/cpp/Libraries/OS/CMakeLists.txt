add_library(OS STATIC "")

target_link_libraries(OS PRIVATE
libace_napi.z.so
hilog_ndk.z
librawfile.z.so
ace_ndk.z
)

target_sources(OS PRIVATE
# MARK: -  OS
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Core/Config.h"
# MARK: - OS__Math
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Math/MathTypes.h"
# MARK: - OS__Interfaces
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Interfaces/IApp.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Interfaces/ICameraController.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Interfaces/IFileSystem.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Interfaces/IFont.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Interfaces/IInput.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Interfaces/ILog.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Interfaces/IMemory.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Interfaces/IMiddleware.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Interfaces/IOperatingSystem.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Interfaces/IProfiler.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Interfaces/IScripting.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Interfaces/IThread.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Interfaces/IUI.h"
# MARK: - Dependencies__EASTL
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/EASTL/assert.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/EASTL/fixed_pool.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/EASTL/hashtable.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/EASTL/intrusive_list.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/EASTL/numeric_limits.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/EASTL/red_black_tree.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/EASTL/string.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/EASTL/thread_support.cpp"
# MARK: - OS__Logging
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Logging/Log.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Logging/Log.h"
# MARK: - Dependencies__minizip__brg
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/lib/brg/aes.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/lib/brg/aescrypt.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/lib/brg/aeskey.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/lib/brg/aesopt.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/lib/brg/aestab.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/lib/brg/aestab.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/lib/brg/brg_endian.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/lib/brg/brg_types.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/lib/brg/hmac.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/lib/brg/hmac.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/lib/brg/sha1.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/lib/brg/sha1.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/lib/brg/sha2.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/lib/brg/sha2.h"
# MARK: - Dependencies__minizip
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/mz.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/mz_crypt.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/mz_crypt.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/mz_crypt_brg.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/mz_os.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/mz_os.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/mz_strm.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/mz_strm_raw.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/mz_strm_wzaes.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/mz_strm_wzaes.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/mz_strm_zlib.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/mz_strm_zlib.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/mz_zip.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/mz_zip.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/minizip/zip.h"
# MARK: - Dependencies__minizip__libzip28zip29
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/zip/miniz.h"
# MARK: - OS__FileSystem
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/FileSystem/FileSystem.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/FileSystem/SystemRun.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/FileSystem/UnixFileSystem.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/FileSystem/ZipFileSystem.c"
# MARK: - OS__MemoryTracking
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/MemoryTracking/MemoryTracking.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/MemoryTracking/NoMemoryDefines.h"
# MARK: - Dependencies__EASTL__FORGEEXT
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/EASTL/allocator_forge.cpp"
# MARK: - Dependencies__cpu_features
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/cpu_features/src/hwcaps.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/cpu_features/src/impl_aarch64_linux_or_android.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/cpu_features/src/impl_x86_linux_or_android.c"
# MARK: - Dependencies__EASTL__EAStdC
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/EASTL/EAStdC/EAMemory.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/EASTL/EAStdC/EASprintf.cpp"
# MARK: - Dependencies__LUA
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lapi.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lauxlib.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lbaselib.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lbitlib.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lcode.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lcorolib.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lctype.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/ldblib.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/ldebug.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/ldo.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/ldump.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lfunc.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lgc.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/linit.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/liolib.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/llex.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lmathlib.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lmem.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/loadlib.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lobject.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lopcodes.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/loslib.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lparser.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lstate.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lstring.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lstrlib.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/ltable.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/ltablib.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/ltm.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lundump.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lutf8lib.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lvm.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/lua-5.3.5/src/lzio.c"
# MARK: - Dependencies__basisu
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/basis_universal/transcoder/basisu_transcoder.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/basis_universal/zstd/zstddeclib.c"
# MARK: - OS__Dependencies_Core
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Core/Atomics.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Core/CPUConfig.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Core/GPUConfig.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Core/RingBuffer.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Core/Timer.c"
# MARK: - OS__Platform
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/OHOS/OHOSBase.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/OHOS/OHOSFileSystem.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/OHOS/OHOSLog.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/OHOS/OHOSThread.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/OHOS/OHOSTime.c"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/OHOS/OHOSWindow.cpp"
# MARK: - Dependencies__imgui
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Fonts/stbtt.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/imgui/imconfig.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/imgui/imgui.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/imgui/imgui.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/imgui/imgui_demo.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/imgui/imgui_draw.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/imgui/imgui_internal.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/imgui/imgui_widgets.cpp"
# MARK: - OS__Fonts__Shaders__FSL
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Fonts/Shaders/FSL/resources.h"
# MARK: - OS__Fonts
   # "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Fonts/FontSystem.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Fonts/stbtt.cpp"
# MARK: - OS__UI
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/UI/UI.cpp"
# MARK: - OS__WindowSystem
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/WindowSystem/WindowSystem.cpp"
# MARK: - OS__Profiler
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Profiler/GpuProfiler.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Profiler/GpuProfiler.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Profiler/ProfilerBase.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Profiler/ProfilerBase.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Profiler/ProfilerHTML.h"
# MARK: - OS__Input
   # "../../../../../Common_3/OS/Input/InputSystem.cpp"
# MARK: - OS__Scripting
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Scripting/LuaManager.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Scripting/LuaManagerImpl.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Scripting/LuaSystem.cpp"
# MARK: - OS__Core
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Core/Screenshot.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Core/ThreadSystem.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Core/ThreadSystem.h"
# MARK: - OS__Camera
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Camera/CameraController.cpp"
# MARK: - OS__Input
    #"${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/OS/Input/InputSystem.cpp"
)
