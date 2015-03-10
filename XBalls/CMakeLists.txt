include_directories(${X11_X11_INCLUDE_PATH} ${X11_Xt_INCLUDE_PATH})
#include_directories(${X11_X11_INCLUDE_PATH} ${X11_Xt_INCLUDE_PATH} ${X11_XRes_INCLUDE_PATH})


if (Mercury_Include_Xballs_Support)

	add_executable(xballs xballs.c)

	set_target_properties(xballs PROPERTIES LANGUAGE C)
	SET_TARGET_PROPERTIES(xballs PROPERTIES LINKER_LANGUAGE C)
	target_link_libraries(xballs ${X11_Xt_LIB} ${X11_X11_LIB} ${X11_XRes_LIB} m)
	#target_link_libraries(xballs ${X11_Xt_LIB} ${X11_X11_LIB} m)

endif()
