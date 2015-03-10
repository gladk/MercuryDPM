
#Part 1 : All cpp files will be made to exec files
#########################################
#Collect all the names of the cpp, note at the moment limited to demos, but will be fixed later
file(GLOB CPPFILES  "*.cpp")
#for every cpp found
foreach(CPPFILE ${CPPFILES})
	#extract the actually file name
	get_filename_component(FILENAME ${CPPFILE} NAME)
	#extract the filename minus the cpp. This will be the name of exe file
	get_filename_component(EXECNAME ${CPPFILE} NAME_WE)
	#Make the exe
	add_executable(${EXECNAME} ${FILENAME})
	#All cpp folder and linked agaist DPMBase
	target_link_libraries(${EXECNAME}  DPMBase)
endforeach()


#Part 2 : Make run test for each of the demo files
##################################################

file(GLOB TESTS "*demo.cpp")
#for each demo add a test with the same name
foreach (TEST ${TESTS})
	get_filename_component(EXECNAME ${TEST} NAME_WE)
	add_test(${EXECNAME} ${EXECNAME})
endforeach()


include(${CMAKE_SOURCE_DIR}/Configuration/MasterMake.cmake)



