cmake_minimum_required(VERSION 3.5)
project(node-obs)

################################################################################
# System & Utilities
################################################################################
# Detect Libary Suffix
IF(WIN32)
	SET(libExtension ".dll")
ELSEIF(APPLE)
	SET(libExtension ".dylib")
ELSE()
    SET(libExtension ".so")
ENDIF()

# Detect Architecture (Bitness)
math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")

################################################################################
# Dependencies
################################################################################

# Open Broadcaster Software Studio
SET(OBSSTUDIOPATH "" CACHE PATH "Path to Open Broadcaster Software Studio")
if (OBSSTUDIOPATH STREQUAL "")
	# OBSSTUDIOPATH is empty
	message(FATAL_ERROR "'OBSSTUDIOPATH' is not set!")
	return()
endif()

if (NOT EXISTS "${OBSSTUDIOPATH}/libobs/obs-module.h")
	# OBSSTUDIOPATH is invalid
	message(FATAL_ERROR "'OBSSTUDIOPATH' is invalid, needs to have required headers!")
	return()
endif()

SET(obsPath "${OBSSTUDIOPATH}")
INCLUDE("${OBSSTUDIOPATH}/cmake/external/Findlibobs.cmake")

# We need both 32-bit and 64-bit for a fully working libobs
IF(NOT APPLE)
	IF (EXISTS "${OBSSTUDIOPATH}/build32")
		SET(obsPath32 "${OBSSTUDIOPATH}/build32")
	ENDIF()
	IF (EXISTS "${OBSSTUDIOPATH}/build64")
		SET(obsPath64 "${OBSSTUDIOPATH}/build64")
	ENDIF()
ELSE()
	SET(obsPath32 "${OBSSTUDIOPATH}/build")
	SET(obsPath64 "${OBSSTUDIOPATH}/build")
ENDIF()
IF("${BITS}" EQUAL "64")
	SET(obsBuildPath "${obsPath64}")
	SET(obsBuildPathOpt "${obsPath32}")
	SET(obsBits "64")
	SET(obsBitsOpt "32")
ELSE()
	SET(obsBuildPath "${obsPath32}")
	SET(obsBuildPathOpt "${obsPath64}")
	SET(obsBits "32")
	SET(obsBitsOpt "64")
ENDIF()

################################################################################
# Code
################################################################################

# Includes
include_directories(
	${CMAKE_JS_INC}
	${OBSSTUDIOPATH}/libobs/
	${OBSSTUDIOPATH}/deps_obs-studio_fork/win64/include/
	${OBSSTUDIOPATH}/deps/
	${PROJECT_SOURCE_DIR}/node_modules/nan/
)

# Source
SET(PROJECT_HEADERS
	"${PROJECT_SOURCE_DIR}/source/obspp/obspp-index.hpp"
	"${PROJECT_SOURCE_DIR}/source/obspp/obspp-manager.hpp"
	"${PROJECT_SOURCE_DIR}/source/audio-encoders.h"
	"${PROJECT_SOURCE_DIR}/source/OBS_API.h"
	"${PROJECT_SOURCE_DIR}/source/OBS_autoConfig.h"
	"${PROJECT_SOURCE_DIR}/source/OBS_audio.h"
	"${PROJECT_SOURCE_DIR}/source/OBS_common.h"
	"${PROJECT_SOURCE_DIR}/source/OBS_content.h"
	"${PROJECT_SOURCE_DIR}/source/OBS_event.h"
	"${PROJECT_SOURCE_DIR}/source/OBS_Display.h"
	"${PROJECT_SOURCE_DIR}/source/OBS_settings.h"
	"${PROJECT_SOURCE_DIR}/source/OBS_service.h"
	"${PROJECT_SOURCE_DIR}/source/obspp/obspp-index.hpp"
	"${PROJECT_SOURCE_DIR}/source/obspp/obspp-manager.hpp"
)
SET(PROJECT_SOURCE
	"${PROJECT_SOURCE_DIR}/source/obspp/obspp-index.cpp"
	"${PROJECT_SOURCE_DIR}/source/obspp/obspp-manager.cpp"
	"${PROJECT_SOURCE_DIR}/source/audio-encoders.cpp"
	"${PROJECT_SOURCE_DIR}/source/OBS_API.cpp"
	"${PROJECT_SOURCE_DIR}/source/OBS_autoConfig.cpp"
	"${PROJECT_SOURCE_DIR}/source/OBS_audio.cpp"
	"${PROJECT_SOURCE_DIR}/source/OBS_content.cpp"
	"${PROJECT_SOURCE_DIR}/source/OBS_event.cpp"
	"${PROJECT_SOURCE_DIR}/source/OBS_Display.cpp"
	"${PROJECT_SOURCE_DIR}/source/OBS_module.cpp"
	"${PROJECT_SOURCE_DIR}/source/OBS_service.cpp"
	"${PROJECT_SOURCE_DIR}/source/OBS_settings.cpp"
	"${PROJECT_SOURCE_DIR}/source/wrapperOBS.cpp"
	"${PROJECT_SOURCE_DIR}/source/obspp/obspp-manager.cpp"
	"${PROJECT_SOURCE_DIR}/source/obspp/obspp-index.cpp"
)
ADD_LIBRARY(${PROJECT_NAME} SHARED ${PROJECT_SOURCE} ${PROJECT_HEADERS})

# Libraries
SET(node-obs_LIBRARIES
)

# Binary Dependencies
SET(node-obs_THIRDPARTY
	"${PROJECT_SOURCE_DIR}/index.js"
	"${PROJECT_SOURCE_DIR}/package.json"
)

################################################################################
# Platform Dependencies
################################################################################
IF(WIN32)
	# Windows
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)

	LIST(APPEND node-obs_THIRDPARTY
		"${obsBuildPath}/libobs/RelWithDebInfo/obs${libExtension}"
		"${obsBuildPath}/libobs/RelWithDebInfo/obs.pdb"
		"${obsBuildPath}/libobs-d3d11/RelWithDebInfo/libobs-d3d11${libExtension}"
		"${obsBuildPath}/libobs-d3d11/RelWithDebInfo/libobs-d3d11.pdb"
		"${obsBuildPath}/libobs-opengl/RelWithDebInfo/libobs-opengl${libExtension}"
		"${obsBuildPath}/libobs-opengl/RelWithDebInfo/libobs-opengl.pdb"
		"${obsBuildPath}/deps/glad/RelWithDebInfo/obsglad${libExtension}"
		"${obsBuildPath}/deps/w32-pthreads/RelWithDebInfo/w32-pthreads${libExtension}"
		"${obsBuildPath}/rundir/RelWithDebInfo/bin/${BITS}bit/avcodec-57${libExtension}"
		"${obsBuildPath}/rundir/RelWithDebInfo/bin/${BITS}bit/avdevice-57${libExtension}"
		"${obsBuildPath}/rundir/RelWithDebInfo/bin/${BITS}bit/avfilter-6${libExtension}"
		"${obsBuildPath}/rundir/RelWithDebInfo/bin/${BITS}bit/avformat-57${libExtension}"
		"${obsBuildPath}/rundir/RelWithDebInfo/bin/${BITS}bit/avutil-55${libExtension}"
		"${obsBuildPath}/rundir/RelWithDebInfo/bin/${BITS}bit/libcurl${libExtension}"
		"${obsBuildPath}/rundir/RelWithDebInfo/bin/${BITS}bit/libogg-0${libExtension}"
		"${obsBuildPath}/rundir/RelWithDebInfo/bin/${BITS}bit/libopus-0${libExtension}"
		"${obsBuildPath}/rundir/RelWithDebInfo/bin/${BITS}bit/libvorbis-0${libExtension}"
		"${obsBuildPath}/rundir/RelWithDebInfo/bin/${BITS}bit/libvorbisenc-2${libExtension}"
		"${obsBuildPath}/rundir/RelWithDebInfo/bin/${BITS}bit/libvorbisfile-3${libExtension}"
		"${obsBuildPath}/rundir/RelWithDebInfo/bin/${BITS}bit/libvpx-1${libExtension}"
		"${obsBuildPath}/rundir/RelWithDebInfo/bin/${BITS}bit/libx264-148${libExtension}"
		"${obsBuildPath}/rundir/RelWithDebInfo/bin/${BITS}bit/swresample-2${libExtension}"
		"${obsBuildPath}/rundir/RelWithDebInfo/bin/${BITS}bit/swscale-4${libExtension}"
		"${obsBuildPath}/rundir/RelWithDebInfo/bin/${BITS}bit/w32-pthreads${libExtension}"
		"${obsBuildPath}/rundir/RelWithDebInfo/bin/${BITS}bit/zlib${libExtension}"
		"${PROJECT_SOURCE_DIR}/resources/roboto.png"
	)
ELSEIF(APPLE)
	# MacOSX	
	LIST(APPEND node-obs_THIRDPARTY
		"${obsBuildPath}/libobs/RelWithDebInfo/libobs.0${libExtension}"		
		"${obsBuildPath}/libobs-opengl/RelWithDebInfo/libobs-opengl.so"
		"${obsBuildPath}/rundir/RelWithDebInfo/bin/libobsglad.0${libExtension}"
	)
ELSEIF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
	# Linux
	LIST(APPEND node-obs_THIRDPARTY
		"${obsBuildPath}/libobs/RelWithDebInfo/libsobs.0${libExtension}"
		"${obsBuildPath}/libobs-opengl/RelWithDebInfo/libobs-opengl${libExtension}"
	)
ELSEIF("${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD")
	# FreeBSD
	LIST(APPEND node-obs_THIRDPARTY
	)
ENDIF()

################################################################################
# Building
################################################################################
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
	${LIBOBS_LIBRARIES}
	${node-obs_LIBRARIES}
	${CMAKE_JS_LIB}
	"${OBSSTUDIOPATH}/deps_obs-studio_fork/win64/bin/avcodec.lib"
	"${OBSSTUDIOPATH}/deps_obs-studio_fork/win64/bin/avdevice.lib"
	"${OBSSTUDIOPATH}/deps_obs-studio_fork/win64/bin/avfilter.lib"
	"${OBSSTUDIOPATH}/deps_obs-studio_fork/win64/bin/avformat.lib"
	"${OBSSTUDIOPATH}/deps_obs-studio_fork/win64/bin/avutil.lib"
	"${obsBuildPath}/deps/libff/RelWithDebInfo/libff.lib"


)

# Distributable Version
ADD_CUSTOM_COMMAND(TARGET node-obs POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	"$<TARGET_FILE:node-obs>"
	"${node-obs_BINARY_DIR}/distribute/node-obs/$<TARGET_FILE_NAME:node-obs>"
)
ADD_CUSTOM_COMMAND(TARGET node-obs POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${node-obs_THIRDPARTY}
	"${node-obs_BINARY_DIR}/distribute/node-obs"
)

IF (NOT APPLE)
	# Plugin
	ADD_CUSTOM_COMMAND(TARGET node-obs POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${obsBuildPathOpt}/rundir/RelWithDebInfo/obs-plugins/${obsBitsOpt}bit"
		"${node-obs_BINARY_DIR}/distribute/node-obs/obs-plugins"
	)
	ADD_CUSTOM_COMMAND(TARGET node-obs POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${obsBuildPath}/rundir/RelWithDebInfo/obs-plugins/${obsBits}bit"
		"${node-obs_BINARY_DIR}/distribute/node-obs/obs-plugins"
	)
	# Plugin Data
	ADD_CUSTOM_COMMAND(TARGET node-obs POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${obsBuildPathOpt}/rundir/RelWithDebInfo/data"
		"${node-obs_BINARY_DIR}/distribute/node-obs/data"
	)
	ADD_CUSTOM_COMMAND(TARGET node-obs POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${obsBuildPath}/rundir/RelWithDebInfo/data"
		"${node-obs_BINARY_DIR}/distribute/node-obs/data"
	)
ELSE()
	ADD_CUSTOM_COMMAND(TARGET node-obs POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${obsBuildPath}/rundir/RelWithDebInfo/obs-plugins"
		"${node-obs_BINARY_DIR}/distribute/node-obs/obs-plugins"
	)
	ADD_CUSTOM_COMMAND(TARGET node-obs POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${obsBuildPath}/rundir/RelWithDebInfo/data"
		"${node-obs_BINARY_DIR}/distribute/node-obs/data"
	)
ENDIF()

# node_modules copy
IF("${NODE_RUNTIME}" MATCHES "node")
	ADD_CUSTOM_COMMAND(TARGET node-obs POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${node-obs_BINARY_DIR}/distribute/node-obs"
		"${PROJECT_SOURCE_DIR}/node-obs"
	)
ENDIF()
