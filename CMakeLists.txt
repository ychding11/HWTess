cmake_minimum_required (VERSION 3.6)

set(PROJECT_NAME HWTesselationDemo)

project (${PROJECT_NAME})

message(STATUS "Building '${PROJECT_NAME}' ")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo" CACHE STRING "Configuration Types" FORCE)

set(_SHADOWLAB_SHADER_LOCATION_ "${CMAKE_CURRENT_SOURCE_DIR}/shaders/" CACHE STRING "Shadow Lab Shader Location" FORCE)

set(ENABLE_HIDECONSOLE_BUILD TRUE CACHE BOOL "TRUE to hide console on Windows.")

set(VERBOSE_MODE FALSE CACHE BOOL "TRUE to output verbose info.")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

set(_SHADER_LOCATION_ ${CMAKE_CURRENT_SOURCE_DIR}/shader/ CACHE STRING "Shader Location" FORCE)
set(_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE STRING "UGS Binary Dir" FORCE) #force update even if it in cache
message(STATUS "_BINARY_DIR='${_BINARY_DIR}' ")

add_compile_definitions(SHADER_LOCATION="${_SHADER_LOCATION_}")

function(group_files src_files)
	foreach(f ${src_files})
        # replace "a/b/c" --> "a\\b\\c"
		string(REPLACE / \\\\ f ${f})
		# Get the path of the file relative to ${CMAKE_SOURCE_DIR}
		file(RELATIVE_PATH relative_path ${CMAKE_SOURCE_DIR} ${f})
		# Extract the folder, ie remove the filename part
		string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" relative_path ${relative_path})

		set(SRCGR "${relative_path}")
		# Source_group expects \\ (double antislash), not / (slash)
		string(REPLACE / \\\\ SRCGR ${SRCGR})
		#message(STATUS "1. group name=${SRCGR}, file=${f}")

        # regular expression https://cmake.org/cmake/help/latest/command/string.html#regex-specification
	    if ("${f}" MATCHES ".*\\.cpp$")
		   set(SRCGR "${SRCGR}\\\\cpp")
        elseif("${f}" MATCHES ".*\\.h$")
		   set(SRCGR "${SRCGR}\\\\header")
        endif()	

		if (VERBOSE_MODE)
				message(STATUS "2. group name=${SRCGR}, file=${f}")
		endif (VERBOSE_MODE)

		source_group(${SRCGR} FILES ${f})
	endforeach()
endfunction()

find_program(FXC NAMES fxc.exe PATHS "C:\\Program Files (x86)\\Windows Kits\\10\\bin\\10.0.17763.0\\x64" )

function(shader_compile shaderfile dependencies)
	file(RELATIVE_PATH name ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${shaderfile})
	set(tempfile ${_BINARY_DIR}/temp/${name}.temp)
	#message(STATUS "shaderfile='${shaderfile}' tempfile='${tempfile}' ")
	add_custom_command(
		OUTPUT ${tempfile}
		COMMAND ${FXC} /T vs_4_1 /E VSMain /Fo ${tempfile}  ${shaderfile}
		MAIN_DEPENDENCY ${shaderfile}
		DEPENDS ${dependencies}
	)
endfunction(shader_compile)

############################################################################
# Custom Lib & Link directory
############################################################################
link_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/bin
)

set(ShaderDependencies
	# Add explicit dependencies here
)

file(GLOB _SHADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/shader/*.hlsl
)

file(GLOB _SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB _EXT_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui/*.cpp
)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/src/
	${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui/
)

group_files("${_SHADER_FILES}")
group_files("${_SRC_FILES}")

if (VERBOSE_MODE)
	# Print include directory for debug
	get_property(includes DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
	foreach(dir ${includes})
		message(STATUS "${PROJECT_NAME} include dir='${dir}'")
	endforeach()
endif(VERBOSE_MODE)

#######################################################################################
# Build Shader files
# shader_compile() is defined in sub-cmakelists.txt
#######################################################################################
foreach(shaderfile ${_SHADER_FILES})
	shader_compile(${shaderfile} "${shaderDependencies}")
endforeach()

#######################################################################################
# defined custom cmake function
# set target property
# reference https://cmake.org/cmake/help/v3.0/manual/cmake-properties.7.html 
#######################################################################################
function(my_default_target_property target_name)
  #message(STATUS "ARGC=${ARGC} target_name='${target_name}' ")
  if (NOT ${ARGC} EQUAL 1)
    message(STATUS "ARGC is NOT expected.") 
  endif()
  if (NOT ${ARGV0} STREQUAL ${target_name})
    message(STATUS "ARGV0 is NOT expected. ARGV0='${ARGV0}' target_name='${target_name}'") 
  endif()  
 
  set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin )
  set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin )
  set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin )
  set_target_properties(${target_name} PROPERTIES DEBUG_POSTFIX "_d")
  set_target_properties(${target_name} PROPERTIES RELWITHDEBINFO_POSTFIX "RelWithDebInfo")
  set_target_properties(${target_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
endfunction()

#######################################################################################
## preprocess macro definition
#######################################################################################
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D__STDC_FORMAT_MACROS)
add_definitions(-DBX_CONFIG_ENABLE_MSVC_LEVEL4_WARNINGS=1)
add_definitions(-D__STDC_LIMIT_MACROS)
add_definitions(-D__STDC_CONSTANT_MACROS)
add_definitions(-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS)
add_definitions(-DUSE_DL_PREFIX)

if(MSVC)
	add_definitions(-DWIN32)
	add_definitions(-D_WIN32)
	add_definitions(-DUNICODE)
	add_definitions(-D_UNICODE)
	#add_definitions(-DNOMINMAX)
endif(MSVC)

if(VERBOSE_MODE)
	message(STATUS "CMAKE_CXX_FLAGS='${CMAKE_CXX_FLAGS}' ")
	message(STATUS "LINK_OPTIONS='${LINK_OPTIONS}' ")
endif(VERBOSE_MODE)

find_package(DirectX)

#######################################################################################
# Shadow Lab
#######################################################################################
set(EXE_NAME "HWTesselationDemo")
add_executable( ${EXE_NAME} ${_SRC_FILES}  ${_SHADER_FILES} ${_EXT_SRC_FILES})
target_link_libraries( ${EXE_NAME} ${DirectX_D3D11_LIBRARY} ${DirectX_D3D11_COMPILER}  )
my_default_target_property(${EXE_NAME})

# copy shaders in framework into dst
add_custom_command(TARGET ${EXE_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${_SHADER_LOCATION_} ${CMAKE_CURRENT_SOURCE_DIR}/bin/shaders/
)

#######################################################################################
# Hide the console window in visual studio projects
#######################################################################################
if(ENABLE_HIDECONSOLE_BUILD)
	if(MSVC)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
		set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
		set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
		set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
		message(STATUS "Hide console on windows msvc.")
	endif()
endif()
