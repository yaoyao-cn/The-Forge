add_library(SpirvTools STATIC "")
target_sources(SpirvTools PRIVATE
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/SPIRV_Cross/spirv.hpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/SPIRV_Cross/spirv_cfg.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/SPIRV_Cross/spirv_cfg.hpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/SPIRV_Cross/spirv_common.hpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/SPIRV_Cross/spirv_cpp.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/SPIRV_Cross/spirv_cross.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/SPIRV_Cross/spirv_cross.hpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/SPIRV_Cross/spirv_cross_parsed_ir.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/SPIRV_Cross/spirv_cross_util.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/SPIRV_Cross/spirv_glsl.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/SPIRV_Cross/spirv_parser.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/ThirdParty/OpenSource/SPIRV_Cross/spirv_reflect.cpp"

    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/Tools/SpirvTools/SpirvTools.cpp"
    "${CMAKE_SOURCE_DIR}/../../../../../../../Common_3/Tools/SpirvTools/SpirvTools.h"
)
