cmake_minimum_required(VERSION 3.0)

SET(LSL_BINDIR)
SET(LSL_LIBRARY)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")	
	IF(${CMAKE_C_SIZEOF_DATA_PTR} EQUAL 8)
		SET(LSL_BINDIR ${CMAKE_CURRENT_LIST_DIR}/../Binaries/Win64)
		SET(LSL_LIBRARY
			optimized "${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/liblsl/bin/liblsl64.lib"
			debug "${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/liblsl/bin/liblsl64-debug.lib")
	ELSE()
		SET(LSL_BINDIR ${CMAKE_CURRENT_LIST_DIR}/../Binaries/Win32)
		SET(LSL_LIBRARY
			optimized "${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/liblsl/bin/liblsl32.lib" 
			debug "${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/liblsl/bin/liblsl32-debug.lib")
	ENDIF()
ELSE(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	SET(LSL_BINDIR ${CMAKE_CURRENT_LIST_DIR}/../Binaries/Mac)
	SET(LSL_LIBRARY
		optimized "${LSL_BINDIR}/liblsl64.dylib"
		debug "${LSL_BINDIR}/liblsl64-debug.dylib")
ENDIF()
include_directories(${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/liblsl/include)

#
# TEST OUTLET
#
add_executable(test_outlet ${CMAKE_CURRENT_LIST_DIR}/test_outlet.cpp)
target_link_libraries(test_outlet ${LSL_LIBRARY})

#
# TEST INLET
#
add_executable(test_inlet ${CMAKE_CURRENT_LIST_DIR}/test_inlet.cpp)
target_link_libraries(test_inlet ${LSL_LIBRARY})
