file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.c)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})

# add the source files
set(SOURCE_FILES
	main.c
	game.c
	fileio.c
	loopDetection.c
	superFastHash.c
)

# Include the header files
include_directories(${CMAKE_SOURCE_DIR}/src)

# Return the source files to the parent CMakeLists.txt
set(SRC_FILES ${SOURCE_FILES}  PARENT_SCOPE)