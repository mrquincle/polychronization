##########################################################################################
# Part of PhD thesis of Anne van Rossum, concerning action-perception loops. This makefile
# is part of a cmake build system. 
# The code falls under the Lesser General Public License (LGPL GNU v3).

# Author:	 Anne C. van Rossum (Almende B.V.)
# Date: 	 Apr. 16, 2012
#
# Copyright © 2012 Anne van Rossum <anne@almende.com>
##########################################################################################

IF(WIN32)
cmake_minimum_required(VERSION 2.6)
ELSE(WIN32)
cmake_minimum_required(VERSION 2.4)
ENDIF(WIN32)

# The directory with all the FindXXX modules
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# The macro below forces the build directory to be different from source directory:
INCLUDE( MacroEnsureOutOfSourceBuild )
MACRO_ENSURE_OUT_OF_SOURCE_BUILD(
	"${PROJECT_NAME} requires an out of source build."
)

SET(PROJECT_NAME "NeuralNetwork")

# Start a project.
PROJECT(${PROJECT_NAME})

# Find packages
#FIND_PACKAGE(Boost REQUIRED COMPONENTS filesystem serialization program_options system)
FIND_PACKAGE(PLplot REQUIRED)

# Header files
#INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${PLplot_INCLUDE_DIR})

# Shared libraries
#SET(LIBS ${LIBS} ${Boost_LIBRARIES})
SET(LIBS ${LIBS} ${PLplot_cxx_LIBRARY})

# Some debug information
MESSAGE("${PROJECT_NAME} is using CXX flags: ${CMAKE_CXX_FLAGS}")
MESSAGE ("Libraries included: ${LIBS}")

# Search for source code.
FILE(GLOB folder_source src/*.cpp src/*.cc src/*.c)
FILE(GLOB folder_header inc/*.h test/*.h)

SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

SET(folder_source ${folder_source} test/TestNetwork.cpp)

# Automatically add include directories if needed.
FOREACH(header_file ${folder_header})
  GET_FILENAME_COMPONENT(p ${header_file} PATH)
  INCLUDE_DIRECTORIES(${p})
ENDFOREACH(header_file ${folder_header})

# Testing
# enable_testing()
#add_subdirectory(test)

# Set up our main executable.
IF (folder_source)
   ADD_EXECUTABLE(${PROJECT_NAME} ${folder_source} ${folder_header})
   TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBS})
   install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)   
ELSE (folder_source)
  MESSAGE(FATAL_ERROR "No source code files found. Please add something")
ENDIF (folder_source)

