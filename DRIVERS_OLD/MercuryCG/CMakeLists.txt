include_directories(${Mercury_SOURCE_DIR}/src
	                    ${Mercury_BINARY_DIR}/src)


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
	target_link_libraries(${EXECNAME}  MercuryBase)
endforeach()


#Part 2 : Make run test for each of the demo files
##################################################

file(GLOB TESTS "*demo.cpp")
#for each demo add a test with the same name
foreach (TEST ${TESTS})
	get_filename_component(EXECNAME ${TEST} NAME_WE)
	add_test(${EXECNAME} ${EXECNAME})
endforeach()

#Part 3 : Make tests for each of the selftest_data files
########################################################


file(GLOB TESTDATAFILES "${CMAKE_CURRENT_SOURCE_DIR}/selftest_data/*.*")
#for each file in the selftest_data folder create a test. Which checks the data agaist this old datai. The actually testing is done my the script self_test.
foreach(TESTFILE ${TESTDATAFILES})
	get_filename_component(TESTNAME ${TESTFILE} NAME)
	add_test(${TESTNAME} ${CMAKE_SOURCE_DIR}/scripts/self_test ${TESTFILE} ${TESTNAME})
endforeach()


