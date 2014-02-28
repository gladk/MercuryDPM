cmake_minimum_required (VERSION 2.8)
project (Mercury C CXX)

set(Mercury_VERSION_MAJOR 0)
set(Mercury_VERSION_MINOR 9)

#Directory were the external cmake modules are stored
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


enable_testing()

if (UNIX)
    add_definitions( -DUNIX )
	message(STATUS "This is unix")
endif(UNIX)

add_subdirectory(DRIVERS)
add_subdirectory(src)
add_subdirectory(xballs)

add_custom_target(fulltest 
			COMMAND make clean
			COMMAND make
			COMMAND make test
			COMMENT "This will clean the project, rebuild and test." )

#This section generates a configuration file that contains the CMake varibles#
#############################################

configure_file(conf/CMakeDefinitions.conf
		${PROJECT_BINARY_DIR}/src/CMakeDefinitions.hpp)


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
    configure_file(conf/doxygen.conf 
        	 	${PROJECT_BINARY_DIR}/conf/doxygen.conf  @ONLY IMMEDIATE)

    #The next four and for website doxygen builds. The should be hinded from the public cmake at some point
    configure_file(conf/web_doxygen.conf
	    		${PROJECT_BINARY_DIR}/conf/web_doxygen.conf @ONLY IMMEDIATE)

    configure_file(conf/hpg.css
	    		${PROJECT_BINARY_DIR}/conf/hpg.css @ONLY IMMEDIATE)

    configure_file(conf/new_footer.html
	    		${PROJECT_BINARY_DIR}/conf/new_footer.html @ONLY IMMEDIATE)

    configure_file(conf/new_header.html
	    		${PROJECT_BINARY_DIR}/conf/new_header.html @ONLY IMMEDIATE)





    #-- Add custom targets to both make and clean (delete) the documentation
        add_custom_target 	(doc
                            COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/conf/doxygen.conf
                            SOURCES ${PROJECT_BINARY_DIR}/conf/doxygen.conf)

	add_custom_target	(docClean
                        COMMAND rm -r ${PROJECT_BINARY_DIR}/docs/*
			COMMENT "Cleaning (deleting) the documentation"	) 

	add_custom_target	(docWeb
			COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/conf/web_doxygen.conf
			SOURCES ${PROJECT_BINARY_DIR}/conf/web_doxygen.conf)

		#add_custom_target	(docPublishAlpha
		#	COMMAND rsync -vr docs/html/* WebAdmin@einder.ewi.utwente.nl:/home/www/html/hpGEM/assets/doxygen/alpha/
		#			COMMENT "Publishing (uploading) the current documentation as ALPHA realese" )
  endif()
endif()

option(Mercury_Include_Xballs_Support "Use the xballs of Stefan Luding for visualisation" ON)

if (Mercury_Include_Xballs_Support)
    FIND_PACKAGE(X11)
    if (NOT X11_FOUND)
        message(FATAL_ERROR 
            "X11 is needed, please install it")
    endif()
endif()



