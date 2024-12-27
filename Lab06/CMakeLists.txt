set(GAMENAME Lab06)
# This grabs all files in this directory that end with .cpp, .h, and .hpp
file(GLOB source_files CONFIGURE_DEPENDS "*.cpp" "*.h" "*.hpp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${source_files})

# Don't change this
add_executable(${GAMENAME} ${source_files})

# Link libraries and copy DLL files
if (WIN32)
	target_link_libraries(${GAMENAME} SDL2 SDL2main SDL2_image SDL2_mixer SDL2_ttf glew32 opengl32)
	
	file(GLOB_RECURSE MYDLLS "${PROJECT_SOURCE_DIR}/Libraries/*.dll")
	foreach(CurrentDllFile IN LISTS MYDLLS)
		add_custom_command(TARGET ${GAMENAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${CurrentDllFile}" "${CMAKE_CURRENT_BINARY_DIR}"
			COMMENT "Copy dll file to ${CMAKE_CURRENT_BINARY_DIR} directory" VERBATIM
		)
	endforeach()
elseif (APPLE)
	target_link_libraries(${GAMENAME} SDL2 SDL2_image SDL2_mixer SDL2_ttf ogg vorbis vorbisfile GLEW ${COREFOUNDATION_LIBRARY} ${OPENGL_LIBRARY} ${COREGRAPHICS_LIBRARY} ${COREAUDIO_LIBRARY} ${AUDIOTOOLBOX_LIBRARY} ${COREVIDEO_LIBRARY} ${FORCEFEEDBACK_LIBRARY} ${IOKIT_LIBRARY} ${APPKIT_LIBRARY} ${METAL_LIBRARY} ${CARBON_LIBRARY})
	set_target_properties(
		${GAMENAME}
		PROPERTIES
		XCODE_ATTRIBUTE_CLANG_WARN_SUSPICIOUS_IMPLICIT_CONVERSION YES
		XCODE_ATTRIBUTE_GCC_WARN_SHADOW YES
		XCODE_ATTRIBUTE_GCC_WARN_ABOUT_RETURN_TYPE YES
		XCODE_ATTRIBUTE_GCC_WARN_SIGN_COMPARE YES
		XCODE_ATTRIBUTE_GCC_WARN_UNINITIALIZED_AUTOS YES
		XCODE_ATTRIBUTE_GCC_WARN_UNUSED_FUNCTION NO
	)
	
	# Use this to figure out Xcode or not
	get_cmake_property(is_multi GENERATOR_IS_MULTI_CONFIG)
	if (is_multi)
		set(COPY_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug")
	else()
		set(COPY_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
	endif()

	file(GLOB_RECURSE MYDLLS "${PROJECT_SOURCE_DIR}/Libraries/*.dylib")
	foreach(CurrentDllFile IN LISTS MYDLLS)
		add_custom_command(TARGET ${GAMENAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${CurrentDllFile}" "${COPY_DESTINATION}"
			COMMENT "Copy dylib file to ${COPY_DESTINATION} directory" VERBATIM
		)
	endforeach()

	add_custom_command(TARGET ${GAMENAME}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/${GAMENAME}/Assets" "${COPY_DESTINATION}/Assets"
		COMMENT "Copy Assets directory" VERBATIM
	)
elseif (EMSCRIPTEN)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file \"${PROJECT_SOURCE_DIR}/${GAMENAME}/Assets@/Assets\"")
endif()
