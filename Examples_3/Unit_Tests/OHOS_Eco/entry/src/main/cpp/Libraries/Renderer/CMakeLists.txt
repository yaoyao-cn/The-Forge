add_library(Renderer STATIC "")

target_include_directories(Renderer PRIVATE ${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/shaderc_ohos/include)

target_link_libraries(Renderer PRIVATE
    OS
    SpirvTools
    vulkan
#    ${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/shaderc_ohos/libs/${OHOS_ARCH}/$<CONFIG>/libshaderc_combined.a
    ${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/shaderc_ohos/libs/${OHOS_ARCH}/Release/libshaderc_combined.a
)

target_sources(Renderer PRIVATE
# MARK: -  meshoptimizer
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/meshoptimizer/src/allocator.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/meshoptimizer/src/indexgenerator.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/meshoptimizer/src/overdrawoptimizer.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/meshoptimizer/src/vcacheoptimizer.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/meshoptimizer/src/vfetchoptimizer.cpp"

# MARK: -  Interface
#    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/Renderer/IRay.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/Renderer/IRenderer.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/Renderer/IResourceLoader.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/Renderer/IShaderReflection.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/Renderer/RendererConfig.h"

# MARK: -  Renderer
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/Renderer/CommonShaderReflection.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/Renderer/Renderer.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/Renderer/ResourceLoader.cpp"

# MARK: -  Vulkan
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/Renderer/Vulkan/Vulkan.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/Renderer/Vulkan/VulkanCapsBuilder.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/Renderer/Vulkan/VulkanConfig.h"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/Renderer/Vulkan/VulkanRaytracing.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/Renderer/Vulkan/VulkanShaderReflection.cpp"
)
