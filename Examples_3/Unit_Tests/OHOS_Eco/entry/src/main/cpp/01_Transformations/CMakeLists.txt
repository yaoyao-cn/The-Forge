ADD_DEFINITIONS(-DVK_USE_PLATFORM_OHOS=1)
add_library(entry SHARED
"../../../../../../../../Examples_3/Unit_Tests/src/01_Transformations/01_Transformations.cpp"
"../module.cpp"
)

target_link_libraries(entry PRIVATE
Renderer
)


#find_package(Python3 COMPONENTS Interpreter)
#if(NOT Python3_Interpreter_FOUND)
#	message(FATAL_ERROR "Python not found !")
#endif()
set(FSLWorkSpace "${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../Common_3/Tools/ForgeShadingLanguage/")
set(FSLCompiler "${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../Common_3/Tools/ForgeShadingLanguage/fsl.py")

if(NOT EXISTS "${FSLCompiler}")
	message(FATAL_ERROR "Missing ${FSLCompiler} !")
endif()
set(GPUCfgDIR   "${CMAKE_CURRENT_LIST_DIR}/../../resources/rawfile/GPUCfg")
set(ShadersVDIR "${CMAKE_CURRENT_LIST_DIR}/../../resources/rawfile/Shaders/VULKAN")
set(ShadersBDIR "${CMAKE_CURRENT_LIST_DIR}/../../resources/rawfile/Shaders/Binary")
set(TexturesDIR "${CMAKE_CURRENT_LIST_DIR}/../../resources/rawfile/Textures")

file(MAKE_DIRECTORY ${ShadersVDIR})
file(MAKE_DIRECTORY ${TexturesDIR})
file(MAKE_DIRECTORY ${GPUCfgDIR})
message(STATUS "Created directory: ${ShadersVDIR}")
message(STATUS "Created directory: ${TexturesDIR}")
message(STATUS "Created directory: ${GPUCfgDIR}")

## Copy Textures
file(GLOB TextureFiles "${CMAKE_CURRENT_LIST_DIR}/../../../../../../UnitTestResources/Textures/Skybox_*.ktx")
list(APPEND TextureFiles "${CMAKE_CURRENT_LIST_DIR}/../../../../../../UnitTestResources/Textures/circlepad.ktx")
file(COPY ${TextureFiles} DESTINATION "${TexturesDIR}")

#buildShaders
set(ShaderFiles
"${CMAKE_CURRENT_LIST_DIR}/../../../../../../src/01_Transformations/Shaders/FSL/basic.frag.fsl"
"${CMAKE_CURRENT_LIST_DIR}/../../../../../../src/01_Transformations/Shaders/FSL/basic.vert.fsl"
"${CMAKE_CURRENT_LIST_DIR}/../../../../../../src/01_Transformations/Shaders/FSL/crash.vert.fsl"
"${CMAKE_CURRENT_LIST_DIR}/../../../../../../src/01_Transformations/Shaders/FSL/skybox.frag.fsl"
"${CMAKE_CURRENT_LIST_DIR}/../../../../../../src/01_Transformations/Shaders/FSL/skybox.vert.fsl"
)

foreach (f ${ShaderFiles})
	    get_filename_component(fname ${f} NAME_WLE)
		execute_process(
				COMMAND "python.exe"
				    "${FSLCompiler}"
					-d "${ShadersVDIR}"
					-b "${ShadersBDIR}"
					-l "VULKAN"
					--noAPISubFolder
					"${f}"
			)
			message(STATUS "end: add_custom_command")

endforeach()