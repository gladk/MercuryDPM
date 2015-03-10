cmake_minimum_required (VERSION 2.8)
project (Mercury C CXX)

set(Mercury_VERSION_MAJOR 0)
set(Mercury_VERSION_MINOR 10)

#Directory were the external cmake modules are stored
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


enable_testing()

if (UNIX)
    add_definitions( -DUNIX )
endif(UNIX)


#if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  #if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7.0)
   # message(FATAL_ERROR "The code requires at least GNU g++ 4.7.0,  please update, current version is ${CMAKE_CXX_COMPILER_VERSION}")
   #endif()
#endif()

#Here is the check for CX11 support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

#Add the flag to compiler options if the current compiler is found
if (COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()	
	message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. We require a compiler that has support for CXX0X, for example GCC 4.3 or later")
endif()

#If uses has not given build type set it to Release
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE 
	STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
		FORCE)
endif(NOT CMAKE_BUILD_TYPE)

add_subdirectory(Drivers)
add_subdirectory(Kernel)
add_subdirectory(XBalls)

add_custom_target(fullTest
        COMMAND make rebuild_cache	
	COMMAND make clean
	COMMAND make
	COMMAND make test
	COMMENT "This will clean the project, rebuild and test." )

#This section generates a configuration file that contains the CMake variables#
#############################################

configure_file(Configuration/CMakeDefinitions.conf
		${PROJECT_BINARY_DIR}/Kernel/CMakeDefinitions.hpp)


#This section is about the build of documentation#
##################################################

option(Mercury_BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF)


if(Mercury_BUILD_DOCUMENTATION)
    FIND_PACKAGE(Doxygen)
    if (NOT DOXYGEN_FOUND)
        message(FATAL_ERROR 
            "Doxygen is needed to build the documentation. Please install it correctly or turn the option")
    endif()

  if (DOXYGEN_FOUND)
    
    #This is the configure file for normal doxygen builds	
    configure_file(Configuration/doxygen.conf 
        	 	${PROJECT_BINARY_DIR}/Configuration/doxygen.conf  @ONLY IMMEDIATE)

    #The next four and for website doxygen builds. The should be hinded from the public cmake at some point
    configure_file(Configuration/web_doxygen.conf
	    		${PROJECT_BINARY_DIR}/Configuration/web_doxygen.conf @ONLY IMMEDIATE)

    configure_file(Configuration/hpg.css
	    		${PROJECT_BINARY_DIR}/Configuration/hpg.css @ONLY IMMEDIATE)

    configure_file(Configuration/new_footer.html
	    		${PROJECT_BINARY_DIR}/Configuration/new_footer.html @ONLY IMMEDIATE)

    configure_file(Configuration/new_header.html
	    		${PROJECT_BINARY_DIR}/Configuration/new_header.html @ONLY IMMEDIATE)

    #-- Add custom targets to both make and clean (delete) the documentation
    add_custom_target 	(doc
		    COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Configuration/doxygen.conf
            SOURCES ${PROJECT_BINARY_DIR}/Configuration/doxygen.conf)

	add_custom_target	(docClean
            COMMAND mkdir -p ${PROJECT_BINARY_DIR}/docs
            COMMAND rm -r ${PROJECT_BINARY_DIR}/docs/*
			COMMENT "Cleaning (deleting) the documentation"	) 

	add_custom_target	(docWeb
			COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Configuration/web_doxygen.conf
			SOURCES ${PROJECT_BINARY_DIR}/Configuration/web_doxygen.conf)

		#add_custom_target	(docPublishAlpha
		#	COMMAND rsync -vr docs/html/* WebAdmin@einder.ewi.utwente.nl:/home/www/html/hpGEM/assets/doxygen/alpha/
		#			COMMENT "Publishing (uploading) the current documentation as ALPHA realese" )
  endif()
endif()

option(Mercury_Include_Xballs_Support "Use the xballs of Stefan Luding for visualisation" ON)

if (Mercury_Include_Xballs_Support)
	FIND_PACKAGE(X11)
        configure_file(${PROJECT_SOURCE_DIR}/XBalls/xballs.txt ${PROJECT_BINARY_DIR}/XBalls/xballs.txt)
    if (NOT X11_FOUND)
        message(FATAL_ERROR "X11 is needed, please install it")
    endif()
endif()

